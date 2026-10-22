<?php
require "db.php";

$data = json_decode(file_get_contents('php://input'), true);

$suhu = $data['esp_suhu'] ?? null;
$kelembapan = $data['esp_kelembapan'] ?? null;

$query = "INSERT INTO datasensor (esp_suhu, esp_kelembapan) VALUES ('$suhu', '$kelembapan')";

if ($koneksi->query($query)) {
  echo json_encode("✅ Data masuk bro - Suhu: $suhu, Kelembapan: $kelembapan");
} else {
  echo json_encode("❌ Error DB: " . $koneksi->error);
}
