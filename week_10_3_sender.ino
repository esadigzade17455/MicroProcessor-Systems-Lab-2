// Receive one byte (blocking)
uint8_t uart_read(void) {
    while (!(UCSR0A & (1 << RXC0)));   // Wait for received data
    return UDR0;
}
// Transmit one byte (blocking)
void uart_write(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait until transmit buffer empty
    UDR0 = data;
}
// UART initialization: 9600 baud, 8N1
void setup(void) {
    UBRR0H = 0;
    UBRR0L = 103;                      // 9600 baud @ 16 MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}
void loop(void) {
    uint8_t c = uart_read();

    if (c == '1' || c == '2' || c == '3' || c == '4') {
        uart_write(c);                // Send to Arduino B
    }
}
