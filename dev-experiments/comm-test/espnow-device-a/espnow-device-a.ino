#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int value;
} struct_message;

struct_message outgoingStruct;

uint8_t receiverAddress[] = {0xD8, 0x3B, 0xDA, 0x13, 0x68, 0x54}; // MAC receiver

unsigned long previousMillis = 0;
const unsigned long interval = 1000; // interval 1 detik (dalam ms)

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Peer added successfully");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    outgoingStruct.value = 1;
    esp_now_send(receiverAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
    Serial.println("Sent value: 1 to receiver");
  }
}
