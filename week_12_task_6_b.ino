#include <Wire.h>
uint8_t lastByte = 0; // Store last received byte
void setup() {
  Wire.begin(0x20);            // Initialize as I2C slave with address 0x20
  Wire.onReceive(receiveEvent); // Triggered when master sends data
  Wire.onRequest(requestEvent); // Triggered when master requests data
}
// Called when master sends data
void receiveEvent(int howMany) {
  if (Wire.available())
    lastByte = Wire.read();     // Read and store last byte
}
// Called when master requests data
void requestEvent() {
  Wire.write(lastByte);        // Send back last received byte
}
void loop() {
  // Nothing to do, everything handled in ISR callbacks
}
