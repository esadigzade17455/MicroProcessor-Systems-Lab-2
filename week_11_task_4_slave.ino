// SPI Slave
// MOSI → PB3, MISO → PB4, SCK → PB5, SS → PB2
volatile byte receivedValue = 0;
// Initialize SPI in slave mode
void spi_slave_init(void) {
    DDRB |= (1 << PB4);                          // MISO as output
    DDRB &= ~((1<<PB3) | (1<<PB5) | (1<<PB2));   // MOSI, SCK, SS as input

    SPCR = (1 << SPE);                           // Enable SPI (slave)
}
// SPI transfer complete interrupt
ISR(SPI_STC_vect) {
    receivedValue = SPDR;                        // Read received byte
}
void setup(void) {
    Serial.begin(9600);                          // Serial monitor output
    spi_slave_init();
    SPCR |= (1 << SPIE);                         // Enable SPI interrupt
    sei();                                       // Enable global interrupts
}
void loop(void) {
    Serial.println(receivedValue);
    delay(200);
}
