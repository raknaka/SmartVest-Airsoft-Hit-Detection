#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "boleh";
const char *password = "mantabjiwa123";

const char* serverUrl = "https://l7xgct6c-3001.asse.devtunnels.ms/api/gameplay/status-by-mac";
String macAddress;

int buttonPin = 2;
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(buttonPin, INPUT_PULLUP);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }

  Serial.println("Terhubung ke WiFi!");
  macAddress = WiFi.macAddress();
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    int status_ready = buttonState == LOW ? 1 : 0; // LOW = ditekan

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Kirim JSON sesuai kebutuhan backend
    String postData = "{\"mac_address\":\"" + macAddress + "\",\"status_ready\":" + String(status_ready) + "}";
    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error Code: " + String(httpResponseCode));
    }
    http.end();
  }

  lastButtonState = buttonState;
  delay(100);
}