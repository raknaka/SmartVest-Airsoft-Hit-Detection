#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = {0xD8, 0x3B, 0xDA, 0x13, 0x68, 0x54}; // MAC Address ESP32-Mini-C3_A

// Variabel untuk menyimpan nilai yang akan dikirim
int weapon_status = 0; // Default nilai awal 0 (tidak ditekan)

// Struktur untuk komunikasi
typedef struct struct_message {
  int status; // Nilai integer untuk weapon_status
} struct_message;

// Membuat struktur untuk mengirim data
struct_message outgoingStruct;

// Pin tombol
const int buttonPin = 11; // Ganti dengan pin yang Anda gunakan

// Callback saat data terkirim
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi WiFi di mode station
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set callback untuk pengiriman
  esp_now_register_send_cb(OnDataSent);

  // Tambahkan peer (receiver)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Inisialisasi pin tombol
  pinMode(buttonPin, INPUT_PULLUP); // Gunakan INPUT_PULLUP untuk mendeteksi tombol ditekan
}

void loop() {
  // Baca status tombol
  int buttonState = digitalRead(buttonPin);

  // Ubah weapon_status berdasarkan status tombol
  if (buttonState == LOW) { // LOW berarti tombol ditekan
    weapon_status = 1;
  } else {
    weapon_status = 0;
  }

  // Siapkan data untuk dikirim
  outgoingStruct.status = weapon_status;

  // Kirim data ke receiver
  esp_now_send(broadcastAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
  Serial.println("Data sent: " + String(weapon_status));

  // Tunggu 100ms sebelum pengiriman berikutnya
  delay(100);
}
