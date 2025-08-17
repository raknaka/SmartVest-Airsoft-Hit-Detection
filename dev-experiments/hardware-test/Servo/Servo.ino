#include <ESP32Servo.h>

// Objek servo
Servo myServo;

// Pin yang digunakan
const int servoPin = 2;

// Sudut awal servo
int angle = 0;

void setup() {
  // Inisialisasi servo
  myServo.attach(servoPin);

  // Pindahkan servo ke posisi awal (0 derajat)
  myServo.write(angle);

  // Serial monitor untuk debugging
  Serial.begin(115200);
  Serial.println("Servo initialized on pin 2");
}

void loop() {
  // Gerakkan servo dari 0° ke 180°
  for (angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(15); // Delay untuk pergerakan servo
  }

  // Gerakkan servo kembali dari 180° ke 0°
  for (angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15); // Delay untuk pergerakan servo
  }
}
