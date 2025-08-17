#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>

//Declar LCD
// Lebar dan tinggi OLED (sesuai spesifikasi modul Anda)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
// Inisialisasi OLED dengan alamat I2C (umumnya 0x3C atau 0x3D)
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
//==============================================================================

// WiFi Configuration
const char *ssid = "farisimut";
const char *password = "12345678";
String ip = "192.168.137.1";
String serverUrl1 = "http://" + ip + "/VEST2/api/register_esp32.php";
String serverUrl2 = "http://" + ip + "/VEST2/api/update_status.php";
String serverUrl3 = "http://" + ip + "/VEST2/api/status_esp.php";
String serverUrl4 = "http://" + ip + "/VEST2/api/update_health.php";

// // Sensor PIN:
const int vibratePins[] = { 14, 13, 12, 11, 10, 9, 17, 3 };
const int numVibratePins = sizeof(vibratePins) / sizeof(vibratePins[0]);
int vibrateValue[] = {};

// Pin Definitions
const int buttonPin = 2;
const int BUZZER_PIN = 1;
int buttonPressed = 0;
int game_status;
String macAddress;
uint8_t broadcastAddress[] = { 0xD8, 0x3B, 0xDA, 0x13, 0x68, 0x54 };  //esp32mini
int weapon_update = 0;
int health = 200;


// Struktur untuk komunikasi
typedef struct struct_message {
  int value;  // Nilai yang dikirimkan
} struct_message;

// Membuat struktur untuk mengirim dan menerima data
struct_message outgoingStruct;
struct_message incomingStruct;

//status status
int switchwifi = 0;
int switchespnow = 0;

//logo GAMBAR
// 'HEART2', 20x19px
const unsigned char epd_bitmap_HEART2[] PROGMEM = {
  0x1c, 0x38, 0x3e, 0x7c, 0x73, 0xfe, 0xe3, 0xff, 0xc7, 0xff, 0xc7, 0xff, 0xef, 0xff, 0x6f, 0xfe,
  0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80
};

// 'Halfhearth', 16x15px
const unsigned char epd_bitmap_Halfhearth[] PROGMEM = {
  0x1c, 0x00, 0x3e, 0x00, 0x73, 0x00, 0xe3, 0x00, 0xc7, 0x00, 0xc7, 0x00, 0xef, 0x00, 0x6f, 0x00,
  0x7f, 0x00, 0x3f, 0x00, 0x1f, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00
};

//gambar gembok
const unsigned char epd_bitmap_lock[] PROGMEM = {
  0x1f, 0x00, 0x31, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x7f, 0xc0, 0xf1, 0xe0, 0xe0, 0xe0,
  0xe0, 0xe0, 0xf1, 0xe0, 0xf1, 0xe0, 0xf1, 0xe0, 0x7f, 0xc0
};
//SMARTVEST
//logo
const unsigned char epd_bitmap_VEST[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0xf9, 0x8c, 0x03, 0xc7, 0xe0, 0x20, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x81, 0x9c, 0x62, 0x40, 0x80, 0x23, 0x20, 0x18, 0x40, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0xc1, 0xd6, 0x72, 0x40, 0x80, 0x22, 0x3f, 0xb1, 0xf8, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x61, 0x52, 0x5b, 0xc0, 0x80, 0x26, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x19, 0x72, 0xf9, 0xfc, 0x80, 0x2c, 0x3c, 0x3c, 0x40, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x09, 0x22, 0x89, 0x00, 0x80, 0x38, 0x20, 0x04, 0x60, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x79, 0x02, 0x8d, 0x00, 0x00, 0x30, 0x3c, 0x7c, 0x3c, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xe0, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3e, 0x01, 0xe0, 0x01,
  0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x39, 0xfe, 0xe0, 0x01,
  0x80, 0x1f, 0xff, 0xff, 0xfe, 0x3f, 0xa3, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x0f, 0xff, 0xff, 0x00, 0x3f, 0x82, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x0f, 0xfe, 0x00, 0x00, 0xfc, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x07, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x83, 0x90, 0xce, 0x03, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x82, 0x19, 0x28, 0x02, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x83, 0x91, 0xee, 0x7b, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x91, 0x22, 0x02, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x83, 0x9d, 0x2e, 0x02, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// Function Prototypes
void beepBuzzer(int duration);
void initLCD();
void drawHealthDisplay();
void OnDataRecv();
void switchToWiFi();
void switchToESPNow();
void sendMac();
void sendStatusPlayer(int statusReady);
void gameStatus();
void vibrateRead();
void sendHealth();
void handleGameStart();
void handleGameInProgress();
void handleGameFinished();

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(buttonPin, INPUT_PULLUP);

  // Sensors:
  for (int i = 0; i < numVibratePins; i++) {
    pinMode(vibratePins[i], INPUT);
  }

  Serial.begin(115200);
  initLCD();
  WiFi.mode(WIFI_STA);
  switchToWiFi();
  macAddress = WiFi.macAddress();
  Serial.println("MAC Address: " + macAddress);
  sendMac();
}

void loop() {
  gameStatus();
  switch (game_status) {
    case 0:
      handleGameStart();
      break;
    case 1:
      handleGameInProgress();
      break;
    case 2:
      handleGameFinished();
      break;
    default:
      Serial.println("Unknown game status");
      break;
  }
  delay(1000);
}

void switchToWiFi() {
  // Matikan ESP-NOW
  esp_now_deinit();
  Serial.println("ESP-NOW disabled");

  // Aktifkan WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Animasi koneksi WiFi
  unsigned long startTime = millis();
  bool isConnected = false;

  while (!isConnected) {
    // Periksa status WiFi
    if (WiFi.status() == WL_CONNECTED) {
      isConnected = true;
      break;
    }
    // Tampilkan animasi "Connecting to WiFi..."
    // display.clearDisplay();
    // display.setTextSize(1);  // Normal 1:1 pixel scale
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0, 0);  // Start at top-left corner
    // display.print("Connecting to WiFi");

    // // Tambahkan titik-titik animasi
    // int elapsed = (millis() - startTime) / 500 % 4;  // Hitung indeks titik (0-3)
    // for (int i = 0; i < elapsed; i++) {
    //   display.print(".");
    // }

    // display.display();  // Update OLED
    // delay(100);         // Tunggu sebentar sebelum iterasi berikutnya
  }

  // // Tampilkan pesan berhasil
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE);
  // display.setCursor(0, 0);
  // display.println(F("@Wifi Connect@"));
  // display.setCursor(0, 10);
  // display.print("IP: ");
  // display.println(WiFi.localIP());  // Tampilkan IP Address
  // display.display();

  // Log ke Serial Monitor
  Serial.println("WiFi connected. IP Address: " + WiFi.localIP().toString());
}

//LOGO DLL
void drawHealthDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Health: ");
  display.print(health);

  display.drawBitmap(0, 12, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);
  display.drawBitmap(20, 12, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);
  display.drawBitmap(40, 12, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);
  display.drawBitmap(60, 12, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);
  display.drawBitmap(80, 12, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);



  switch (weapon_update) {
    case 0:
      display.drawBitmap(117, 0, epd_bitmap_lock, 11, 13, SSD1306_WHITE);
      break;
    case 1:

      break;
  }
  display.display();
}

void switchToESPNow() {
  // Deklarasi broadcastAddress sebagai variabel lokal
  uint8_t receiverAddress[] = { 0xD8, 0x3B, 0xDA, 0x13, 0x68, 0x54 };

  // Matikan WiFi
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);  // Set mode ke Station

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set callback untuk pengiriman dan penerimaan data
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Tambahkan peer (B) sebagai tujuan
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

  // outgoingStruct.value = 1;
  // esp_now_send(receiverAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
}


// Callback saat data diterima
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingStruct, incomingData, sizeof(incomingStruct));
  Serial.print("Data received from B: ");
  Serial.println(incomingStruct.value);
  weapon_update = incomingStruct.value;
}

void SendDataESPNow(const uint8_t *receiverAddress, int weapon) {
  outgoingStruct.value = weapon;
  esp_now_send(receiverAddress, (uint8_t *)&outgoingStruct, sizeof(outgoingStruct));
}

// Callback saat data dikirim
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void beepBuzzer(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
  delay(duration);
}

//Void inisialisasi LCD
void initLCD() {
  // Inisialisasi I2C dengan pin custom
  Wire.begin(8, 18);  // SDA = 8, SCL = 9

  // Inisialisasi layar OLED
  if (!display.begin(SSD1306_BLACK, OLED_ADDRESS)) {
    Serial.println(F("OLED tidak ditemukan! Pastikan koneksi benar."));
    while (true)
      ;  // Berhenti jika OLED tidak ditemukan
  }
}


void sendMac() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl1);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "mac_address=" + macAddress;
    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error Code: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected, cannot send MAC address.");
  }
}

void sendStatusPlayer(int statusReady) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl2);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"mac_address\":\"" + macAddress + "\",\"status_ready\":" + String(statusReady) + "}";
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.println("POST request sent successfully");
      Serial.println("Response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error in POST request");
    }
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}

void gameStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl3);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      int index = payload.indexOf("\"game_status\":");
      if (index != -1) {
        String value = payload.substring(index + 14);
        value = value.substring(0, value.indexOf('}'));
        game_status = value.toInt();
      }
    } else {
      Serial.println("Error in HTTP request: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
void vibrateRead() {
  for (int i = 0; i < numVibratePins; i++) {
    vibrateValue[i] = analogRead(vibratePins[i]);
  }

  bool detectVibration = false;
  for (int i = 0; i < numVibratePins; i++) {
    if (vibrateValue[i] >= 1000000) {  // Sesuaikan threshold
      detectVibration = true;
      Serial.print("Sensor ");
      Serial.print(i + 1);
    }
  }

  // Jika ada getaran kuat, kurangi health
  if (detectVibration && health > 0) {
    health = max(0, health - 25);  // Pastikan tidak negatif
    sendHealth();
  }
  if (health == 0) {
    switchToESPNow();  // Ganti ke protokol ESP-NOW
    SendDataESPNow(broadcastAddress, 0);
  }
}


void sendHealth() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl4);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "mac_address=" + macAddress + "&health=" + String(health);
    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Data sent successfully!");
      Serial.println("Server Response: " + response);
    } else {
      Serial.println("HTTP POST failed. Error Code: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send health data.");
  }
}


void handleGameStart() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonPressed == 0) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(20, 13);
    display.print("PRESS THE BUTTON");
    display.display();
    // Kondisi jika tombol ditekan
    if (buttonState == HIGH) {
      buttonPressed = 1;
      sendStatusPlayer(1);
      Serial.println("Wait Button To Press...");

      // Menampilkan teks "Press Button" di OLED
      display.clearDisplay();
      display.setCursor(45, 13);
      display.println("SUCCESS");
      display.display();

      delay(200);  // Debounce singkat
    }
  }

  // Kondisi jika tombol dilepaskan
  else if (buttonState == LOW && buttonPressed == 1) {
    display.clearDisplay();
    for (int i = 0; i < 3; i++) {  // Animasi sebanyak 3 siklus
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.drawBitmap(0, 0, epd_bitmap_VEST, 128, 32, SSD1306_WHITE);
      display.setCursor(90, 20);
      display.print("Wait");
      for (int j = 0; j <= 2; j++) {
        display.print(".");
        display.display();  // Update tampilan
        delay(300);         // Tunggu 300ms per titik
      }

      display.clearDisplay();  // Bersihkan layar sebelum siklus berikutnya
    }
  }
}

void handleGameInProgress() {

  drawHealthDisplay();

  switch (weapon_update) {
    case 0:
      switch (switchespnow) {
        case 0:
          switchToESPNow();
          switchespnow = 1;
          break;
        case 1:
          SendDataESPNow(broadcastAddress, 1);
          break;
      }
      break;
    case 1:
      switch (switchwifi) {
        case 0:
          switchToWiFi();
          switchwifi = 1;
          break;  // Tambahkan break di sini
        case 1:
          // vibrateRead();
          break;
      }
      break;
  }
}



void handleGameFinished() {
  // if (weapon) {
  //   sendData();
  //   weapon = false;
  // }
  Serial.println("Finish...");
}
