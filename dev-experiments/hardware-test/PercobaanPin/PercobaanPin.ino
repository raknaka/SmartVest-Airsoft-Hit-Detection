const int analogPins[] = {14, 13, 12, 11, 10, 9, 17, 3}; // Daftar pin yang tersedia untuk diuji

int selectedPin = -1;

void setup() {
    Serial.begin(115200);
    Serial.println("Pilih pin yang ingin diuji (masukkan nomor pin):");
}

void loop() {
    if (Serial.available() > 0) {
        int pin = Serial.parseInt();
        bool validPin = false;
        for (int i = 0; i < sizeof(analogPins) / sizeof(analogPins[0]); i++) {
            if (analogPins[i] == pin) {
                validPin = true;
                selectedPin = pin;
                Serial.print("Pin ");
                Serial.print(selectedPin);
                Serial.println(" dipilih untuk pengujian.");
                break;
            }
        }
        if (!validPin) {
            Serial.println("Pin tidak valid, coba lagi.");
        }
    }
    
    if (selectedPin != -1) {
        int nilai = analogRead(selectedPin);
        Serial.print("Pin ");
        Serial.print(selectedPin);
        Serial.print(" : ");
        Serial.println(nilai);
        Serial.println("---------------------------");
    }
    delay(100); // Tunggu 1 detik sebelum membaca kembali
}
