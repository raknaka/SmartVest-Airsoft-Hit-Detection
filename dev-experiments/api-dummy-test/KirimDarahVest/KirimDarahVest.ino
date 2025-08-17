#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Configuration
const char *ssid = "boleh";
const char *password = "mantabjiwa123";
// String ip = "192.168.137.1";
String serverUrl4 = "https://l7xgct6c-3001.asse.devtunnels.ms/api/hitpoint/update-health";

// Pin untuk getaran (contoh: bahu kiri)
int vibratePins1 = 14;  // Sesuaikan dengan pin yang tersedia di ESP32

// Variabel untuk menyimpan MAC Address
String macAddress;

// Fungsi untuk mengirim data ke server
void sendDummyData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl4);
    http.addHeader("Content-Type", "application/json");

    int dummyHealth = random(0, 100);
    int dummyHitpoint = random(0, 8);

    String postData = "{\"mac_address\":\"" + macAddress + "\",\"health\":" + String(dummyHealth) + ",\"hitpoint\":" + String(dummyHitpoint) + "}";

    Serial.println("Sending Data: " + postData);

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Data sent successfully!");
      Serial.println("Server Response: " + response);
    } else {
      Serial.print("HTTP POST failed. Error Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke WiFi
  WiFi.mode(WIFI_STA);  // Pastikan mode WiFi adalah Station
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");

  // Ambil MAC Address ESP32
  macAddress = WiFi.macAddress();
  Serial.println("Device MAC Address: " + macAddress);

  // Inisialisasi pin vibrasi jika diperlukan
  pinMode(vibratePins1, OUTPUT);
}

void loop() {
  sendDummyData();
  delay(100);  // Kirim data setiap 10 detik
}
