
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "CALVIN-Student";
const char* password = "CITStudentsOnly";

const char* serverName = "http://10.252.242.118/post.php";

String apiKeyValue = "tPmAT5Ab3j7F9";

int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
int pinBuzzer = 2;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;


void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}


String read_AD8232() {
  float temp = analogRead(34);
  if (temp < 100){
    digitalWrite(pinBuzzer,HIGH);
    delay(150);
  }
  digitalWrite(pinBuzzer,LOW);
  
  if (isnan(temp)) {    
    Serial.println("Failed to read from BME280 sensor!");
    return "";
  }
  else {
  //  Serial.println(temp);
    return String(temp);
    Serial.println(temp);
  }
}


String read_BPM() {
  for(;;){
   reading = analogRead(34); 
      // Heart beat leading edge detected.
      if(reading > UpperThreshold && IgnoreReading == false){
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }

      // Heart beat trailing edge detected.
      if(reading < LowerThreshold){
        IgnoreReading = false;
      }  
      BPM = (1.0/PulseInterval) * 60.0 * 1000;
      delay(15);
      Serial.println(BPM);
      Serial.flush();
      return String(BPM);}
      
}



void loop() {

  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&ecg_value=" + String(read_AD8232())
                          + "&bpm_value=" + String(read_BPM())+ "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    

    int httpResponseCode = http.POST(httpRequestData);
     

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(10);  
}
