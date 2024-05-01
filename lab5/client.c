// Name: Thomas Solecki
// Date: 5/1/2024
// Title: Lab5 – Step 1
// Description: This program makes a client request to upload a file to a
// server over a UDP socket with a rdt 2.2 protocol

// COEN 146L: Lab5, UDP client with packets that have checksum, sequence number,
// acknowledgement number
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

// Declare a Header structure that holds, sequence/ acknowledgement number,
// checksum, and length of a packet
typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

// Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

// Calculating the Checksum
/*The simplest checksum algorithm is the so-called longitudinal parity check,
which breaks the data into "bytes" with a fixed number 8 bits, and then
computes the exclusive or (XOR) of all those bytes.
The result is appended to the message as an extra byte*/
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

// Printing received packet, sequence/acknowledgement number, and checksum
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
           packet.header.seq_ack, packet.header.len, packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

// client sending packet with checksum and sequence number and waits for
// acknowledgement
void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen,
                Packet packet) {
    while (1) {
        // verify rdt2.2 protocol by simulate a packet corruption
        int corrupt;

        // you may use user input for deliberate errors:
        // printf("Would you like to simulate packet corruption? Enter 1 for
        // yes, 0 for no\n"); scanf("%d",&corrupt); or use a random function
        // instead to generate 0 and 1 randomly:
        printf("Would you like to simulate packet corruption? Enter 1 for yes, "
               "0 for no\n");
        scanf("%d", &corrupt);

        // send checksum = 0 (to simulate packet corruption)
        if (corrupt) {
            packet.header.cksum = 0;
        } else {
            packet.header.cksum = getChecksum(packet);
        }
        // send the packet
        printPacket(packet);
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // receive an ACK from the server
        Packet recvpacket;
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0,
                 (struct sockaddr *)address, &addrlen);

        // print received packet (ACK) and checksum
        printf("Client received ACK %d, checksum %d - \n",
               recvpacket.header.seq_ack, recvpacket.header.cksum);

        // calculate checksum of received packet (ACK)
        int e_cksum = getChecksum(recvpacket);

        // check the checksum
        if (recvpacket.header.cksum != e_cksum) {
            // if bad checksum, resend packet
            printf("Client: Bad checksum, expected checksum was: %d\n",
                   e_cksum);
            continue;

            // check the sequence number
        } else if (packet.header.seq_ack != recvpacket.header.seq_ack) {
            // if incorrect sequence number, resend packet
            printf("Client: Bad seqnum, expected sequence number was: %d\n",
                   packet.header.seq_ack);
            continue;
        } else {
            // good ACK, we're done
            printf("Client: Good ACK\n");
            // if the ckechsum and sequence numbers are correct, break and
            // return to the main to get the next packet to send
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // Get from the command line, server IP, Port and src file
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <srcfile>\n", argv[0]);
        exit(0);
    }

    // Declare socket file descriptor.
    int sockfd;

    // Open a UDP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Declare server address to connect to
    struct sockaddr_in servAddr;
    struct hostent *host;
    host = (struct hostent *)gethostbyname(argv[1]);

    /// Set the server address to send using socket addressing structure
    memset(&servAddr, 0, sizeof(servAddr));

    // initialize servAddr structure
    servAddr.sin_port = htons(atoi(argv[2]));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // open file using argv[3]
    int fp = open(argv[3], O_RDWR);
    if (fp < 0) {
        perror("Failed to open file\n");
        exit(1);
    }

    // send file contents to server packet by packet
    int seq = 0;
    socklen_t addr_len = sizeof(servAddr);
    Packet packet;
    while ((packet.header.len = read(fp, packet.data, sizeof(packet.data))) >
           0) {
        // assign seq and checksum to packet and send
        if (packet.header.len == 0) {
            break;
        }
        packet.header.seq_ack = seq;
        packet.header.cksum = getChecksum(packet);
        clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, packet);
        seq = (seq + 1) % 2;
        memset(packet.data, 0, sizeof(packet.data));
    }

    // send zero-length packet to server to end connection
    Packet final;
    final.header.seq_ack = seq;
    final.header.len = 0;
    memset(final.data, 0, sizeof(final.data));
    clientSend(sockfd, (struct sockaddr *)&servAddr, addr_len, final);

    // close file and socket
    close(fp);
    close(sockfd);
    return 0;
}
