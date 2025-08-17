#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "whatdee";
const char* password = "apacoba12";

const char* registerUrl = "http://192.168.232.10/VEST2/api/esp32.php"; // URL untuk registrasi MAC
const char* healthUrl = "http://192.168.232.10/VEST2/api/health.php"; // URL untuk pengiriman health

String macAddress;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  
  Serial.println("Terhubung ke WiFi!");
  
  // Ambil MAC address dari ESP32
  macAddress = WiFi.macAddress();
  Serial.println("MAC Address ESP32: " + macAddress);
  
  // Kirim MAC address ke server hanya sekali saat setup
  HTTPClient http;
  http.begin(registerUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String postData = "mac_address=" + macAddress;
  
  int httpResponseCode = http.POST(postData);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response from esp32.php: " + response);
  } else {
    Serial.println("Error Code: " + String(httpResponseCode));
  }
  
  http.end();
}

void loop() {
  // Simulasi pengiriman data health ke server
  int health = random(50, 100); // Simulasi data health
  
  HTTPClient http;
  http.begin(healthUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String postData = "mac_address=" + macAddress + "&health=" + String(health);
  
  int httpResponseCode = http.POST(postData);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response from health.php: " + response);
  } else {
    Serial.println("Error Code: " + String(httpResponseCode));
  }
  
  http.end();
  delay(1000); // Kirim data setiap 10 detik
}
