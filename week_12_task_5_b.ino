#include <avr/io.h>
#include <util/delay.h>
#define my_add 0x20  // Slave address
#define led_on  0x01
#define led_off 0x00
// Wait for TWI interrupt and return status
uint8_t twi_wait(void) {
    while (!(TWCR & (1<<TWINT)));       // Wait for TWINT flag
    return (TWSR & 0xF8);               // Mask status bits
}
int main(void) {
    // Enable internal pull-ups for I2C pins
    PORTC |= (1 << PC4) | (1 << PC5);
    DDRB |= (1 << PB5);                  // LED pin as output
    DDRD &= ~(1 << PD2);                 // Button pin as input
    PORTD |= (1 << PD2);                 // Enable pull-up for button
    TWAR = (my_add << 1);                // Set slave address
    TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWINT); // Enable TWI and ACK
    while (1) {
        uint8_t status = twi_wait();
        // SLA+W received
        if (status == 0x60) {
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        }
        // Data received from master
        else if (status == 0x80) {
            uint8_t msg = TWDR;
            if (msg == led_on)  PORTB |= (1<<PB5);
            else                PORTB &= ~(1<<PB5);
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        }
        // SLA+R received (master requests data)
        else if (status == 0xA8) {
            uint8_t btn = !(PIND & (1<<PD2)); // Active low button
            TWDR = btn ? led_on : led_off;    // Load data to send
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        }
        // Default case: just re-enable TWI
        else {
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
        }
    }
}
