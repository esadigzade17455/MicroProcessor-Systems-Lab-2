#include <Arduino.h>
const uint8_t cfg[2] = { 0xA5, 0x3C }; // [0] = key, [1] = initial vector
// Cipher restore state
typedef struct {
    uint8_t previous;  // Stores previous ciphertext byte
} RestoreState;
#define ROTR8(v)  ( ((v) >> 3) | ((v) << 5) ) // 8-bit rotate right by 3
// Decrypt one byte using previous ciphertext and key
static uint8_t restore(uint8_t ct, RestoreState *st) {
    uint8_t x = ROTR8(ct ^ cfg[0]); // XOR with key, then rotate right
    x ^= st->previous;              // XOR with previous ciphertext
    st->previous = ct;              // Update previous byte
    return x;                       // Return plaintext byte
}
RestoreState state = { cfg[1] };   // Initialize state with initial vector
void setup() {
    Serial.begin(9600);            // Initialize serial monitor
}
void loop() {
    if (Serial.available()) {       // Check if a byte has been received
        uint8_t byteIn = Serial.read();          // Read ciphertext byte
        uint8_t plain = restore(byteIn, &state);// Decrypt it
        Serial.println(plain);                   // Print plaintext to monitor
    }
}
