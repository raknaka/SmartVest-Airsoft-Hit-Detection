#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int value;
} struct_message;

struct_message incomingStruct;

// Variabel timestamp
unsigned long timestampPrev = 0;
unsigned long timestampNow = 0;
bool firstPacket = true;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingStruct, incomingData, sizeof(incomingStruct));

  // Serial.println("Data received");

  timestampNow = micros();

  if (!firstPacket) {
    unsigned long elapsedTime = timestampNow - timestampPrev;

    const unsigned long baseDelayMicroseconds = 999000;  // Basis baru: 998 ms

    // if (elapsedTime >= baseDelayMicroseconds) {
    //   unsigned long pureLatencyMicro = elapsedTime - baseDelayMicroseconds;
    //   float pureLatencyMilli = pureLatencyMicro / 1000.0;

    //   if (pureLatencyMilli < 1.0) {
    //     Serial.println("0-1ms");
    //   } else {
    //     Serial.print(pureLatencyMilli, 3);
    //     Serial.println("ms");
    //   }
    // } else {
    //   Serial.println("0ms");
    // }
    unsigned long pureLatencyMicro = elapsedTime - baseDelayMicroseconds;
    float pureLatencyMilli = pureLatencyMicro / 1000.0;
    Serial.println(pureLatencyMilli, 3);
    // Serial.println("ms");
    // Serial.println(elapsedTime);

  } else {
    firstPacket = false;
  }


  timestampPrev = timestampNow;  // Update timestamp lama
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver ready");
}

void loop() {
  // Kosong
}
