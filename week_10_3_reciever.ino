#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
// Receive one byte via UART (blocking)
uint8_t UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));     // Wait for received data
    return UDR0;
}
// Turn on LED according to value (1–3)
void ledOn(uint8_t v) {
    PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)); // Clear LEDs

    if (v == 1) PORTD |= (1<<PD2);
    if (v == 2) PORTD |= (1<<PD3);
    if (v == 3) PORTD |= (1<<PD4);
}
// Read one byte from EEPROM
uint8_t read_eeprom_byte(uint8_t addr) {
    while (EECR & (1 << EEPE));          // Wait for completion
    EEAR = addr;
    EECR |= (1 << EERE);                 // Start read
    return EEDR;
}
// Write one byte to EEPROM
void eeprom_write(uint8_t addr, uint8_t data) {
    while (EECR & (1 << EEPE));          // Wait for completion
    EEAR = addr;
    EEDR = data;
    uint8_t sreg = SREG;
    cli();                               // Disable interrupts
    EECR |= (1 << EEMPE);                // Master write enable
    EECR |= (1 << EEPE);                 // Start write
    SREG = sreg;                         // Restore interrupts
}
int main(void) {
    // UART init: receive only, 9600 baud, 8N1
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << RXEN0);                // Enable RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    // LED pins as output
    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4);
    uint16_t addr = 0;
    while (1) {
        uint8_t rx = UART_receive();
        // Store received values 1–3 in EEPROM
        if (rx == '1' || rx == '2' || rx == '3') {
            if (addr < 1024) {
                eeprom_write(addr, rx - '0');
                addr++;
            }
        }
        // Read EEPROM and display values on LEDs
        else if (rx == '4') {
            for (uint16_t i = 0; i < addr; i++) {
                uint8_t v = read_eeprom_byte(i);
                ledOn(v);
                _delay_ms(500);
            }
            PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)); // Clear LEDs
            addr = 0;
        }
    }
}
