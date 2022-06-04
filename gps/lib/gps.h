#define MAX_POSITION_LEN 50
#define MODEM "/dev/ttyUSB0"
#define BAUDRATE 115200 

typedef struct {
		char latitude[MAX_POSITION_LEN];
		char longitude[MAX_POSITION_LEN];
} Position;

int init_gps();
void get_position(Position *p);
void close_gps();
void get_lon(char lon_string[100], char longitude[50]);
void get_lat(char lat_string[100], char latitude[50]);
void format_position();
