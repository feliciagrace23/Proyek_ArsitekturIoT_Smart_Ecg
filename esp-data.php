<!DOCTYPE html>
<html><body>
<?php

$servername = "localhost";

$dbname = "SmartECG";

$username = "root";

$password = "iee3031";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, ecg_value, bpm_value, reading_time FROM SensorData ORDER BY id DESC";

echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <td>ID</td> 
        <td>ecg_value</td> 
        <td>bpm_value</td> 
  
        <td>Timestamp</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_ecg_value = $row["ecg_value"];
        $row_bpm_value = $row["bpm_value"];
  
        $row_reading_time = $row["reading_time"];

        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_ecg_value . '</td> 
                <td>' . $row_bpm_value . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
</body>
</html>