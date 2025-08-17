#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin (-1 jika menggunakan reset bawaan)
#define SCREEN_ADDRESS 0x3C  // I2C address untuk OLED 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'HEART2', 20x19px
const unsigned char epd_bitmap_HEART2[] PROGMEM = {
  0x1c, 0x38, 0x3e, 0x7c, 0x73, 0xfe, 0xe3, 0xff, 0xc7, 0xff, 0xc7, 0xff, 0xef, 0xff, 0x6f, 0xfe,
  0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80
};

const unsigned char epd_bitmap_lock[] PROGMEM = {
  0x1f, 0x00, 0x31, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x7f, 0xc0, 0xf1, 0xe0, 0xe0, 0xe0,
  0xe0, 0xe0, 0xf1, 0xe0, 0xf1, 0xe0, 0xf1, 0xe0, 0x7f, 0xc0
};

const unsigned char epd_bitmap_lock2[] PROGMEM = {
  0x1f, 0x00, 0x31, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x7f, 0xc0, 0xff, 0xe0, 0xf1, 0xe0,
  0xf1, 0xe0, 0xfb, 0xe0, 0xfb, 0xe0, 0xff, 0xe0, 0x7f, 0xc0
};




int health = 100;

void setup() {
  Serial.begin(9600);
  Wire.begin(8, 18);  // Sesuaikan pin SDA dan SCL sesuai kebutuhan

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Berhenti jika inisialisasi gagal
  }

  delay(2000);  // Tunggu 2 detik sebelum menampilkan teks
  display.clearDisplay();

  // Set ukuran dan warna teks
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Tampilkan teks "HP:" dan nilai health
  display.setCursor(40, 0);
  display.print("HP:");
  display.print(health);

  // Gambar ikon hati pada koordinat (100, 5)
  display.drawBitmap(112, 0, epd_bitmap_HEART2, 16, 15, SSD1306_WHITE);
  display.drawBitmap(0, 0, epd_bitmap_lock, 11, 13, SSD1306_WHITE);
  display.display();
}

void loop() {
  // Tidak ada proses pada loop
}
