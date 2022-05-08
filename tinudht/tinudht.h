/*
 * Tinu DHT - Tinusaur DHT11 Library
 *
 * @file: tinudht.h
 * @created: 2014-07-08
 * @author: neven
 *
 * Source code available at: https://bitbucket.org/tinusaur/tinudht
 *
 */
 
#ifndef TINUDHT_H
#define TINUDHTDHT_H
 
#define TINUDHT_OK              0
#define TINUDHT_ERROR_CHECKSUM  -1
#define TINUDHT_ERROR_TIMEOUT   -2
 
typedef struct {
    uint16_t humidity;
    uint16_t temperature;
} TinuDHT;
 
uint8_t tinudht_read(TinuDHT *ptinudht, uint8_t dht_pin);
 
#endif
