#include <avr/io.h>
#include <util/delay.h>
#define KEY 0xA5
uint8_t counter = 0;
// Rotate 8-bit value left by r bits
uint8_t rotl8(uint8_t x, uint8_t r) {
    return (x << r) | (x >> (8 - r));
}
// Simple encryption: XOR with key + rotate
uint8_t encrypt(uint8_t p) {
    return rotl8(p ^ KEY, 3);
}
// Initialize UART at 9600 baud, 8N1
void uart_init(void) {
    uint16_t ubrr = 103;                  // 16MHz / (16*9600) - 1
    UBRR0H = (ubrr >> 8);                 // Set baud high byte
    UBRR0L = ubrr;                        // Set baud low byte
    UCSR0B = (1 << TXEN0);                // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits, 1 stop, no parity
}
// Send one byte via UART
void uart_send(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));     // Wait until data register empty
    UDR0 = data;                          // Load data into transmit register
}
int main(void) {
    uart_init();                           // Initialize UART
    // Sending loop
    while (1) {
        uint8_t cipher = encrypt(counter); // Encrypt counter value
        uart_send(cipher);                 // Send encrypted byte
        counter++;                         // Increment counter
        _delay_ms(100);                    // Delay 100 ms
    }
}
