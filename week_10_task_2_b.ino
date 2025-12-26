#define F_CPU 16000000UL
#include <avr/io.h>
// UART receive only, 9600 baud, 8-bit data
void UART_init(void) {
    UBRR0H = 0;
    UBRR0L = 103;                        // 9600 baud @ 16 MHz
    UCSR0B = (1 << RXEN0);               // Enable receiver only
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}
// Receive one byte (blocking)
uint8_t UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));     // Wait for data
    return UDR0;
}
// Display received value on LEDs
void showLED(uint8_t v) {
    PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)); // Clear LEDs

    if (v == '1') PORTD |= (1<<PD2);
    if (v == '2') PORTD |= (1<<PD3);
    if (v == '3') PORTD |= (1<<PD4);
}
int main(void) {
    UART_init();
    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4);  // LED pins as output
    while (1) {
        uint8_t rx = UART_receive();     // Read UART
        showLED(rx);
    }
}
