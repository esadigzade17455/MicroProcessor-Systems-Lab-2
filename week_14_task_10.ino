#include <Arduino.h>
const uint8_t encrypted[] = {
  87, 26, 72, 13, 67, 95, 89, 23, 72, 22, 73, 11, 87, 30, 73
};                                   // Encrypted message bytes
const uint8_t len = sizeof(encrypted); // Number of encrypted bytes
void setup() {
  Serial.begin(9600);
  while (!Serial) {;}                // Wait for serial monitor
  Serial.println("Searching for printable messages\n");
  unsigned long startTime = millis(); // Start timing
  // Brute-force all possible XOR keys
  for (uint16_t key_even = 0; key_even < 256; key_even++) {
    for (uint16_t key_odd = 0; key_odd < 256; key_odd++) {
      char msg[16] = {0};             // Buffer for decrypted message
      bool valid = true;              // Tracks printable ASCII
      // Decrypt each byte
      for (uint8_t i = 0; i < len; i++) {
        uint8_t key = (i % 2 == 0) ? key_even : key_odd; // Even/odd key
        uint8_t p = encrypted[i] ^ key;                 // XOR decrypt
        if (p < 32 || p > 126) {       // Check printable ASCII
          valid = false;
          break;
        }
        msg[i] = p;                   // Store decrypted byte
      }
      // Print only meaningful messages
      if (valid && strchr(msg, ' ') != nullptr) {
        Serial.print("Keys (even: ");
        Serial.print(key_even);
        Serial.print(", odd: ");
        Serial.print(key_odd);
        Serial.print("): ");
        Serial.println(msg);
        Serial.println();
      }
    }
  }
  unsigned long endTime = millis();          // End timing
  unsigned long elapsedTime = endTime - startTime;
  Serial.print("Total time taken: ");
  Serial.print(elapsedTime);
  Serial.println(" ms");
  Serial.println("Done.");
}
void loop() {}
