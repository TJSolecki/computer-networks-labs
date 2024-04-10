// Name: Thomas Solecki
// Date: 4/10/2024
// Title: Lab1 – Step 4
// Description: This program computes the round trip time in msec for different
// scenarios

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { // check correct usage
        fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
        exit(1);
    }
    // Declare and initialize values
    int n = atoi(argv[1]); // Number of connections
    int RTT0 = 3;          // Local DNS cache RTT delay
    int RTT1 = 20;         // Second DNS server RTT delay
    int RTT2 = 26;         // Third DNS server RTT delay
    int RTTHTTP = 47;      // RTT between local host and Web server

    // a. Time for a single object
    int total_time_single =
        RTT0 + RTT1 + RTT2 + 2 * RTTHTTP; // 2 x RTTHTTP due to TCP handshake
    printf("One object: %d msec\n", total_time_single);

    // b. Time for base object and 6 additional objects
    int total_time_additional = 6 * (2 * RTTHTTP);
    int total_time_b = total_time_single + total_time_additional;
    printf("Non-Persistent 6 objects: %d msec\n", total_time_b);

    // find how many np (parralel connections)
    double packets = (n >= 6) ? 1 : 6.0 / n;
    int np = ceil(packets);

    // No syn ack needed since the connection is left open
    int total_time_persistant = total_time_single + np * (RTTHTTP);
    int total_time_non_persistant = total_time_single + np * (2 * RTTHTTP);
    printf("%d np connection - Persistent: %d msec\n", n,
           total_time_persistant);
    printf("%d np connection - Non-Persistent: %d msec\n", n,
           total_time_non_persistant);

    return 0;
}
