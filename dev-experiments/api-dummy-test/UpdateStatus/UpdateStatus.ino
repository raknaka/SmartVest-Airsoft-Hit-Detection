#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "mamaika2024";
const char* password = "@pacoba12";

const char* serverUrl = "http://192.168.1.8/VEST2/api/update_status.php";


const int buttonPin = 11;  // Pin untuk tombol
bool buttonPressed = false;  // Status apakah tombol sudah ditekan atau belum

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(115200);
  delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

  // Set pin tombol sebagai input
  pinMode(buttonPin, INPUT_PULLUP);  // Tombol dengan pull-up internal
}

void loop() {
  // Cek apakah tombol ditekan
  if (digitalRead(buttonPin) == LOW && !buttonPressed) {  // LOW jika tombol ditekan (karena menggunakan pull-up)
    buttonPressed = true;  // Menandakan tombol sudah ditekan
    
    String macAddress = WiFi.macAddress();  // Ambil MAC Address ESP32
    int statusReady = 1;  // Status ready = 1 (ditekan)

    // Buat payload JSON untuk dikirim
    String payload = "{\"mac_address\":\"" + macAddress + "\",\"status_ready\":" + String(statusReady) + "}";

    // Mengirim data ke server menggunakan HTTP POST
    HTTPClient http;
    http.begin(serverUrl);  // URL server PHP
    http.addHeader("Content-Type", "application/json");  // Menambahkan header Content-Type

    int httpResponseCode = http.POST(payload);  // Kirim data dengan POST
    
    // Menampilkan respon server
    if (httpResponseCode > 0) {
      Serial.println("POST request sent successfully");
      Serial.println("Response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error in POST request");
    }

    http.end();  // Akhiri koneksi HTTP
  }
  
  // Reset jika tombol tidak ditekan (untuk deteksi penekanan berikutnya)
  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = false;
  }

  delay(100);  // Delay untuk menghindari pembacaan tombol terlalu cepat
}
