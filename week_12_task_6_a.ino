#include <Wire.h>
#define slave_add 0x20
unsigned long startTime;
unsigned long endTime;
void setup() {
  Serial.begin(9600);  // Initialize serial monitor
  Wire.begin();        // Initialize as I2C master
  TWSR = 0;            // Set prescaler = 1
  TWBR = 72;           // Set SCL frequency ~200 kHz
  delay(500);
}
void loop() {
  startTime = micros();
  for (int i = 0; i < 50; i++) {
    // Send a byte to the slave
    Wire.beginTransmission(slave_add);
    Wire.write(1);
    Wire.endTransmission();
    // Read one byte from slave
    Wire.requestFrom(slave_add, 1);
    while (!Wire.available());  // Wait until byte received
    uint8_t x = Wire.read();    // Read byte
  }
  endTime = micros();
  Serial.print("I2C time for 50 messages = ");
  Serial.print(endTime - startTime);
  Serial.println(" us");
  delay(5000);
}
