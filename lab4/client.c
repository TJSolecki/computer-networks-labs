// Name: Thomas Solecki
// Date: 4/24/2024
// Title: Lab4 – Step 2
// Description: This program makes a client request to download a file from a
// server over a UDP socket

// COEN 146L : Lab4, step 2: UDP client (peer) that sends a file to a server
// (peer)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Declare a Header structure that holds length of a packet
typedef struct {
    int len;
} Header;

// Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

// Printing received packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"", packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen,
                Packet packet) {
    while (1) {
        // send the packet
        printf("Client sending packet\n");
        printPacket(packet);
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        // receive an ACK from the server
        Packet recvpacket;
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0,
                 (struct sockaddr *)address, &addrlen);

        // print received packet
        printPacket(recvpacket);
        break;
    }
}

int main(int argc, char *argv[]) {
    // Get from the command line, server IP, Port and src file
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <srcfile>\n", argv[0]);
        exit(0);
    }
    char *ip = argv[1];

    // Declare socket file descriptor.
    int sockfd;

    // Open a UDP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Declare server address and get host to connect to
    struct sockaddr_in server_addr;
    struct in_addr in_address;

    // clear memory segment holding server_addr
    memset(&server_addr, 0, sizeof(server_addr));

    // Set the server address to send using socket addressing structure
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;
    in_address.s_addr = inet_addr(ip);
    server_addr.sin_addr = in_address;

    // Open file given by argv[3]
    int fp = open(argv[3], O_RDWR);
    if (fp < 0) {
        perror("Failed to open file\n");
        exit(1);
    }

    // send file contents to server
    socklen_t addr_len = sizeof(server_addr);
    Packet packet;
    while ((packet.header.len = read(fp, packet.data, sizeof(packet.data))) >
           0) {
        clientSend(sockfd, (struct sockaddr *)&server_addr, addr_len, packet);
    }
    // sending zero-length packet (final) to server to end connection
    Packet final;
    final.header.len = 0;
    clientSend(sockfd, (struct sockaddr *)&server_addr, addr_len, final);

    // close file and socket
    close(fp);
    close(sockfd);
    return 0;
}
