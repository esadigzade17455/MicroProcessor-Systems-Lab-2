#include <avr/io.h>
#include <util/delay.h>
const uint8_t cfg[2] = { 0xA5, 0x3C }; // [0] = key, [1] = initial vector
// Cipher state structure
typedef struct {
    uint8_t last;      // Last output byte
    uint8_t position;  // Current counter / input
} CipherState;
#define ROTL8(v)  ( ((v) << 3) | ((v) >> 5) ) // 8-bit rotate left by 3
// Encode one byte using last output and key
static uint8_t codec(uint8_t in, CipherState *st) {
    uint8_t tmp = in ^ st->last;       // XOR with last output
    uint8_t out = ROTL8(tmp) ^ cfg[0]; // Rotate left 3 and XOR with key
    st->last = out;                    // Update last for next byte
    return out;                        // Return encoded byte
}
// Initialize UART for 9600 baud, 8N1
static void uart_begin(void) {
    uint16_t baud = 103;               // 16 MHz / (16*9600) - 1
    UBRR0H = baud >> 8;                // Baud high byte
    UBRR0L = baud & 0xFF;              // Baud low byte
    UCSR0B = (1 << TXEN0);             // Enable transmitter
    UCSR0C = (3 << UCSZ00);            // 8 data bits, 1 stop, no parity
}
// Send one byte via UART
static void uart_push(uint8_t v) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait until data register empty
    UDR0 = v;                          // Load byte to transmit
}
int main(void) {
    CipherState ctx = { .last = cfg[1], .position = 255 }; // Initialize cipher state
    uart_begin();                                        // Init UART
    for (;;) {
        uart_push(codec(ctx.position, &ctx));           // Encode & send byte
        ctx.position--;                                 // Decrement counter (wraps 0→255)
        _delay_ms(100);                                 // Delay 100 ms
    }
}
