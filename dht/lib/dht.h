#define MAXTIMINGS 85
#define DHTPIN 21
#define MAX_DHT_LEN 50

typedef struct {
		char temperature[MAX_DHT_LEN];
		char humidity[MAX_DHT_LEN];
} Dht;

