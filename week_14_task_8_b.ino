#include <avr/io.h>
#include <Arduino.h>   // For serial monitor output
#define KEY 0xA5
// Rotate 8-bit value right by r bits
uint8_t rotr8(uint8_t x, uint8_t r) {
    return (x >> r) | (x << (8 - r));
}
// Decrypt: rotate right then XOR with key
uint8_t decrypt(uint8_t c) {
    return rotr8(c, 3) ^ KEY;
}
// Initialize UART receiver at 9600 baud, 8N1
void uart_init(void) {
    uint16_t ubrr = 103;                  // 16MHz / (16*9600) - 1
    UBRR0H = (ubrr >> 8);                 // Baud rate high byte
    UBRR0L = ubrr;                        // Baud rate low byte
    UCSR0B = (1 << RXEN0);                // Enable receiver
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits, 1 stop, no parity
}
// Receive one byte via UART
uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));      // Wait until data received
    return UDR0;                          // Read byte from data register
}
int main(void) {
    uart_init();                           // Initialize UART
    Serial.begin(9600);                    // For monitor output only
    while (1) {
        uint8_t cipher = uart_receive();   // Receive encrypted byte
        uint8_t plain  = decrypt(cipher);  // Decrypt byte
        Serial.println(plain);             // Print plaintext to monitor
    }
}
