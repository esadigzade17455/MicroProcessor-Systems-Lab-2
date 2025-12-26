#include <avr/io.h>
#include <util/delay.h>
#define slave_address 0x20
#define led_on  0x01
#define led_off 0x00
// Generate I2C start condition
void twi_start(void) {
    TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT); // Start condition
    while (!(TWCR & (1<<TWINT)));               // Wait for completion
}
// Generate I2C stop condition
void twi_stop(void) {
    TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT); // Stop condition
}
// Write one byte to TWI
void twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWEN) | (1<<TWINT);             // Start transmission
    while (!(TWCR & (1<<TWINT)));              // Wait for completion
}
// Read one byte from TWI (NACK after read)
uint8_t twi_read_nack(void) {
    TWCR = (1<<TWEN) | (1<<TWINT);             // Enable TWI to receive
    while (!(TWCR & (1<<TWINT)));              // Wait for completion
    return TWDR;
}
int main(void) {
    // Enable internal pull-ups for I2C pins
    PORTC |= (1 << PC4) | (1 << PC5);
    DDRB |= (1 << PB5);       // LED pin as output
    DDRD &= ~(1 << PD2);      // Button pin as input
    PORTD |= (1 << PD2);      // Enable pull-up for button
    // Configure TWI clock
    TWSR &= ~(1 << TWPS1) & ~(1 << TWPS0); // Prescaler = 1
    TWBR = 32;                              // SCL frequency ~200 kHz
    TWCR = (1 << TWEN);                     // Enable TWI
    while (1) {
        uint8_t buttonState = !(PIND & (1 << PD2)); // 1 when pressed, 0 when released
        // Send button state to slave
        twi_start();
        twi_write((slave_address << 1) | 0);  // Write mode
        if (buttonState) twi_write(led_on);
        else             twi_write(led_off);
        twi_stop();
        _delay_ms(10);
        // Read response from slave
        twi_start();
        twi_write((slave_address << 1) | 1);  // Read mode
        uint8_t rx = twi_read_nack();
        twi_stop();
        // Set LED based on slave response
        if (rx == led_on) PORTB |= (1 << PB5);
        else              PORTB &= ~(1 << PB5);
        _delay_ms(50);
    }
}
