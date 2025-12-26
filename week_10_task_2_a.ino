#define F_CPU 16000000UL
#include <avr/io.h>
// Initialize UART: 9600 baud, 8 data bits, no parity, 1 stop bit
void UART_init(void) {
    UBRR0H = 0;
    UBRR0L = 103;                         // 9600 baud @ 16 MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}
// Receive one byte (blocking)
uint8_t UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));      // Wait for received data
    return UDR0;
}
// Transmit one byte (blocking)
void UART_send(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));     // Wait until buffer empty
    UDR0 = data;
}
int main(void) {
    UART_init();

    while (1) {
        uint8_t c = UART_receive();       // Read from UART

        if (c == '1' || c == '2' || c == '3') {
            UART_send(c);                 // Echo valid input
        }
    }
}
