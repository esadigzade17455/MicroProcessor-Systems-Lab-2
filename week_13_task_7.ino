#include <avr/io.h>
#include <util/delay.h>
#define SEGMENT_SIZE 1024
#define TARGET_PRIMES 2000
#define TIME_BIT PB0   // Timing pin = Arduino pin 8
uint16_t basePrimes[64];
uint8_t baseCount = 0;
uint8_t segment[(SEGMENT_SIZE / 8) + 1];
// Mark a number as composite in the segment
static inline void setComposite(uint16_t idx) {
    segment[idx >> 3] |= (1 << (idx & 7));
}
// Check if a number is composite
static inline uint8_t isComposite(uint16_t idx) {
    return segment[idx >> 3] & (1 << (idx & 7));
}
// Generate base primes < 200
void generateBasePrimes(void) {
    uint8_t small[200];
    for (uint16_t i = 2; i < 200; i++) small[i] = 1;
    for (uint16_t p = 2; p * p < 200; p++)
        if (small[p])
            for (uint16_t m = p * p; m < 200; m += p)
                small[m] = 0;
    for (uint16_t i = 2; i < 200; i++)
        if (small[i])
            basePrimes[baseCount++] = i;
}
int main(void) {
    DDRB |= (1 << PB5) | (1 << TIME_BIT);  // LED + timing pin as output
    generateBasePrimes();
    PORTB |= (1 << TIME_BIT);   // HIGH → start timing
    uint16_t primeCount = 0;
    uint32_t low = 2, high = low + SEGMENT_SIZE;
    while (primeCount < TARGET_PRIMES) {
        // Clear segment
        for (uint16_t i = 0; i < sizeof(segment); i++)
            segment[i] = 0;
        // Mark multiples of base primes as composite
        for (uint8_t i = 0; i < baseCount; i++) {
            uint16_t p = basePrimes[i];
            uint32_t startVal = (low + p - 1) / p * p;
            if (startVal < (uint32_t)p * p)
                startVal = (uint32_t)p * p;
            for (uint32_t m = startVal; m < high; m += p)
                setComposite(m - low);
        }
        // Count primes in segment
        for (uint16_t i = 0; i < SEGMENT_SIZE && primeCount < TARGET_PRIMES; i++) {
            if (!isComposite(i)) {
                primeCount++;
                PINB = (1 << PB5);  // toggle LED
            }
        }
        low += SEGMENT_SIZE;
        high += SEGMENT_SIZE;
    }
    PORTB &= ~(1 << TIME_BIT);  // LOW → stop timing
    while (1);  // End
}
