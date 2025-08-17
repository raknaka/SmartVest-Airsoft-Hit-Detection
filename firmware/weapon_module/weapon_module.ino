#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>  // Gunakan pustaka ESP32Servo

// Struktur untuk komunikasi
typedef struct struct_message {
  int value;  // Nilai yang dikirimkan
} struct_message;

// Membuat struktur untuk mengirim dan menerima data
struct_message outgoingStruct;
struct_message incomingStruct;

// Alamat MAC dari ESP32 pengirim (A)
uint8_t senderAddress[] = { 0x30, 0x30, 0xF9, 0x77, 0x0E, 0x24 };  // Ganti dengan MAC Address perangkat A

// Deklarasi servo
Servo myServo;
const int servoPin = 2;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingStruct, incomingData, sizeof(incomingStruct));
  Serial.print("Data received from VEST: ");
  int recieveVest = incomingStruct.value;
  Serial.println(recieveVest);

  switch (recieveVest) {
    case 0:
      // Gerakkan servo ke sudut 0 derajat
      myServo.write(0);
      Serial.println("Servo moved to 0 degrees");
      // Kirim nilai 3 ke perangkat A
      outgoingStruct.value = 3;
      esp_now_send(senderAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
      Serial.println("Sent value: 3 to A");
      break;
    case 1:
      // Gerakkan servo ke sudut 180 derajat
      myServo.write(180);
      Serial.println("Servo moved to 180 degrees");
      // Kirim nilai 1 ke perangkat A
      outgoingStruct.value = 1;
      esp_now_send(senderAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
      Serial.println("Sent value: 1 to VEST");
      break;
  }
}

// Callback saat data dikirim
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi WiFi di mode Station
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set callback untuk pengiriman dan penerimaan data
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Tambahkan peer (A) sebagai tujuan
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Peer added successfully");

  // Inisialisasi servo
  myServo.attach(servoPin, 500, 2400);  // Sesuaikan pulsa minimum & maksimum jika perlu
  Serial.println("Servo initialized");
}

void loop() {
  // Loop kosong, logika dijalankan melalui callback
}

