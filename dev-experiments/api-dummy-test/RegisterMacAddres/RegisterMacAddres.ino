#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "boleh";
const char* password = "mantabjiwa123";

const char* serverUrl = "https://l7xgct6c-3001.asse.devtunnels.ms/api/add/esp32";ok

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  
  Serial.println("Terhubung ke WiFi!");

  // Kirim MAC address ke server
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Ambil MAC address dari ESP32
  String macAddress = WiFi.macAddress();
  String postData = "mac_address=" + macAddress;

  int httpResponseCode = http.POST(postData);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error Code: " + String(httpResponseCode));
  }

  http.end();
}

void loop() {
  // Loop kosong
}
