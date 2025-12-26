// SPI Master: sends 85 → 170 → 255 repeatedly
// MOSI → PB3, MISO → PB4, SCK → PB5, SS → PB2
// Initialize SPI in master mode
void SPI_MasterInit(void) {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2); // MOSI, SCK, SS as output
    PORTB |= (1 << PB2);                          // SS inactive (high)

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, master, fosc/16
}
// Transmit one byte via SPI
void SPI_MasterTransmit(uint8_t data) {
    PORTB &= ~(1 << PB2);              // Select slave (SS low)
    SPDR = data;                       // Start transmission
    while (!(SPSR & (1 << SPIF)));     // Wait for completion
    PORTB |= (1 << PB2);               // Deselect slave
}
void setup(void) {
    SPI_MasterInit();
}
void loop(void) {
    SPI_MasterTransmit(85);
    delay(1000);
    SPI_MasterTransmit(170);
    delay(1000);
    SPI_MasterTransmit(255);
    delay(1000);
}
