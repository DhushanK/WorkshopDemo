#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte MPU_ADDRESS = 0x68;
const byte PWR_MGMT_1 = 0x6B;
const byte ACCEL_XOUT_H = 0x3B;
const byte OLED_ADDRESS = 0x3C;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDRESS, (byte)6);

  int16_t xRaw = (Wire.read() << 8) | Wire.read();
  int16_t yRaw = (Wire.read() << 8) | Wire.read();
  int16_t zRaw = (Wire.read() << 8) | Wire.read();

  float x = xRaw / 16384.0;
  float y = yRaw / 16384.0;
  float z = zRaw / 16384.0;

  bool level =
    abs(x) < 0.20 &&
    abs(y) < 0.20 &&
    z > 0.80 &&
    z < 1.20;

  Serial.print("X: ");
  Serial.print(x, 2);

  Serial.print("  Y: ");
  Serial.print(y, 2);

  Serial.print("  Z: ");
  Serial.print(z, 2);

  Serial.print("  STATUS: ");
  Serial.println(level ? "LEVEL" : "TILTED");

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("X: ");
  display.println(x, 2);

  display.print("Y: ");
  display.println(y, 2);

  display.print("Z: ");
  display.println(z, 2);

  display.println();

  display.print("STATUS: ");
  display.println(level ? "LEVEL" : "TILTED");

  display.display();

  delay(500);
}
