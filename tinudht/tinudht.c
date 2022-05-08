/*
 * Tinu DHT - Tinusaur DHT11 Library
 *
 * @file: tinudht.c
 * @created: 2014-07-08
 * @author: neven
 *
 * Source code available at: https://bitbucket.org/tinusaur/tinudht
 *
 */
 
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
 
#include "tinudht.h"
 
#define TINUDHT_RCV_TIMEOUT 255
#define TINUDHT_RCV_DELAY 10
#define TINUDHT_RCV_LENGTH 2
 
uint8_t tinudht_read(TinuDHT *ptinudht, uint8_t dht_pin) {
 
    // Buffer to received data
    uint8_t data[5];
    // Empty the buffer
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
 
    // Send request
    DDRB |= (1 << dht_pin); // Set port as output
    PORTB &= ~(1 << dht_pin);   // Set to 0
    _delay_ms(18);  // Wait 18 ms
    PORTB |= (1 << dht_pin);    // Set to 1
    _delay_us(40);  // Wait 40 us
 
    // Receive response
    DDRB &= ~(1 << dht_pin);    // Set port as input
 
    uint8_t timeout;
 
    // Acknowledge
    timeout = TINUDHT_RCV_TIMEOUT;
    while(bit_is_clear(PINB, dht_pin))  // Wait for 1
        if (timeout-- == 0)
            return TINUDHT_ERROR_TIMEOUT;
 
    timeout = TINUDHT_RCV_TIMEOUT;
    while(bit_is_set(PINB, dht_pin))    // Wait for 0
        if (timeout-- == 0)
            return TINUDHT_ERROR_TIMEOUT;
 
    uint8_t bit_index = 7;
    uint8_t byte_index = 0;
    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for (uint8_t i = 0; i < 40; i++)
    {
        // Wait for start
        timeout = TINUDHT_RCV_TIMEOUT;
        while(bit_is_clear(PINB, dht_pin))  // Wait for 1
            if (timeout-- == 0)
                return TINUDHT_ERROR_TIMEOUT;
 
        // Determine the bit value
        uint8_t len = 0;
        while(bit_is_set(PINB, dht_pin)) {  // Wait for 0
            _delay_us(TINUDHT_RCV_DELAY);
            if (len++ == TINUDHT_RCV_TIMEOUT)
                return TINUDHT_ERROR_TIMEOUT;
            }
 
        if (len >= TINUDHT_RCV_LENGTH) data[byte_index] |= (1 << bit_index);
 
        if (bit_index == 0) // next byte?
        {
            bit_index = 7;  // restart at MSB
            byte_index++;   // next byte!
        }
        else bit_index--;
    }
 
    uint8_t checksum = data[0] + data[2];
    if (data[4] != checksum) return TINUDHT_ERROR_CHECKSUM;
 
    // On DHT11 data[1],data[3] are always zero so not used.
    ptinudht->humidity = data[0];
    ptinudht->temperature = data[2];
 
    return TINUDHT_OK;
}
