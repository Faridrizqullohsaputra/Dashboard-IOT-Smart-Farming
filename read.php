<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *'); // Allow CORS for development
include "db.php";

// Fetch latest sensor data
$sql = mysqli_query($koneksi, "SELECT esp_suhu, esp_kelembapan, waktu FROM datasensor ORDER BY id DESC LIMIT 1");
$data = mysqli_fetch_assoc($sql);

if ($data) {
  echo json_encode([
    'temperature' => $data['esp_suhu'] . '°C',
    'humidity' => $data['esp_kelembapan'] . '%',
    'timestamp' => $data['waktu']
  ]);
} else {
  echo json_encode([
    'error' => 'No data found'
  ]);
}
