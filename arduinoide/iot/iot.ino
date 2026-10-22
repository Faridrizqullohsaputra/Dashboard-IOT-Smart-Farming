#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define PIN_DHT 19
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);
float temp, humid;

#define PIN_BUZZER 18

const char* ssid = "ANOMALI";           //ganti sesuaikan
const char* password = "sadardiriaja";  //ganti sesuaikan

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_BUZZER, HIGH);
  delay(100);
  digitalWrite(PIN_BUZZER, LOW);

  delay(1000);
  Serial.println("🚀 Mulai koneksi WiFi...");


  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("\n❌ Gagal konek WiFi. Cek SSID/Password lo bro.");
      return;
    }
  }


  Serial.println("\n✅ WiFi Terkoneksi!");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());
  // bunyi notifikasi singkat
  // Menyalakan buzzer
  digitalWrite(PIN_BUZZER, HIGH);
  // Delay 70 milidetik
  delay(70);
  // Mematikan buzzer
  digitalWrite(PIN_BUZZER, LOW);
  // Delay 70 milidetik
  delay(70);
  // Menyalakan buzzer
  digitalWrite(PIN_BUZZER, HIGH);
  // Delay 70 milidetik
  delay(70);
  // Mematikan buzzer
  digitalWrite(PIN_BUZZER, LOW);
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    temp = dht.readTemperature();
    // Membaca nilai kelembapan dari sensor DHT22 dalam persen
    humid = dht.readHumidity();
    // Mengecek apakah pembacaan sensor gagal (NaN = Not a Number)
    if (isnan(temp) || isnan(humid)) {
      // Mengatur nilai suhu menjadi 0 jika pembacaan gagal
      temp = 0;
      // Mengatur nilai kelembapan menjadi 0 jika pembacaan gagal
      humid = 0;
    }


    // Buat JSON-nya pake ArduinoJson
    StaticJsonDocument<128> doc;
    doc["esp_suhu"] = temp;
    doc["esp_kelembapan"] = humid;


    String jsonString;
    serializeJson(doc, jsonString);


    String server = "http://192.168.18.13/iot-dashboard/insert.php";
    http.begin(server);
    http.addHeader("Content-Type", "application/json");


    Serial.print("📡 Kirim POST ke: ");
    Serial.println(server);
    Serial.print("📦 Payload JSON: ");
    Serial.println(jsonString);


    int httpCode = http.POST(jsonString);


    if (httpCode > 0) {
      Serial.print("✅ HTTP Response: ");
      Serial.println(httpCode);
      String response = http.getString();
      Serial.print("📥 Server Response: ");
      Serial.println(response);
    } else {
      Serial.print("❌ Gagal kirim. Error: ");
      Serial.println(http.errorToString(httpCode));
    }


    http.end();
  } else {
    Serial.println("⚠️ WiFi putus, nyoba reconnect...");
    WiFi.reconnect();
  }


  delay(1000);
}
