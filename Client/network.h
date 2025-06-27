// Network constants and defines

#ifndef NETWORK_H
#define NETWORK_H

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel
#define BUFFER_SIZE 100
#define SEND_BUFFER_SIZE 4

#define PACKET_MOVE   1
#define PACKET_UPDATE 128

void connect_server();
void read_network();

#endif /* NETWORK_H */
