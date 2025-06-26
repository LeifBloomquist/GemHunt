// Network constants and defines

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel
#define BUFFER_SIZE 100

const char addr[] = "tcp://192.168.7.113:6420";
static byte buffer[BUFFER_SIZE];
