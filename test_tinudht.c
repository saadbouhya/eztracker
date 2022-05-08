#include "tinudht/tinudht.h"
#define TINUDHT_PIN PB0

int main() {
	TinuDHT tinudht;
    tinudht_read(&tinudht, TINUDHT_PIN);
    debug_print_dec(tinudht.humidity);
    debug_print_dec(tinudht.temperature);
}

