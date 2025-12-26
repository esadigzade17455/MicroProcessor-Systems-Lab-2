#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// 7-segment digit patterns (a–g)
const uint8_t digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};
// Output digit to 7-segment display
void displayDigit(uint8_t num) {
    if(digits[num][0]) PORTB |=  (1<<PB1); else PORTB &= ~(1<<PB1);
    if(digits[num][1]) PORTB |=  (1<<PB0); else PORTB &= ~(1<<PB0);
    if(digits[num][2]) PORTD |=  (1<<PD7); else PORTD &= ~(1<<PD7);
    if(digits[num][3]) PORTD |=  (1<<PD6); else PORTD &= ~(1<<PD6);
    if(digits[num][4]) PORTD |=  (1<<PD5); else PORTD &= ~(1<<PD5);
    if(digits[num][5]) PORTD |=  (1<<PD3); else PORTD &= ~(1<<PD3);
    if(digits[num][6]) PORTD |=  (1<<PD4); else PORTD &= ~(1<<PD4);
}
volatile uint8_t paused_flag = 0;
// Pin Change Interrupt on PB3
ISR(PCINT0_vect) {
    paused_flag ^= 1;
}
int main(void)
{
    // Segment pins as output
    DDRB |= (1<<PB0) | (1<<PB1);
    DDRD |= (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
    // Button input with pull-up
    DDRB  &= ~(1<<PB3);
    PORTB |=  (1<<PB3);
    // Enable PCINT on PB3
    PCICR  |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT3);
    sei();
    uint8_t counter = 0;
    while(1) {
        if(!paused_flag) {
            displayDigit(counter);
            counter = (counter + 1) % 10;
        }
        _delay_ms(500);
    }
}
