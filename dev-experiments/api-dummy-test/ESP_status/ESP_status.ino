#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "boleh";
const char *password = "mantabjiwa123";
const char *serverUrl3 = "https://l7xgct6c-3001.asse.devtunnels.ms/api/gameplay/status"; 

int game_status = -1;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  gameStatus();
  Serial.println(game_status);
  delay(100); // Ambil status setiap 5 detik

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
