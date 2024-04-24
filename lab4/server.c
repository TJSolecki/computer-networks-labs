// Name: Thomas Solecki
// Date: 4/24/2024
// Title: Lab4 – Step 1
// Description: This program recives client requests to download a file from a
// and then sends the specified file over a UDP socket

// COEN 146L : Lab4, step 1: UDP server (peer) that receives a file from a
// client (peer)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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

// Declare client address to fill in address of sender
struct sockaddr_in server_addr, client_addr;
struct in_addr in_address;

// Printing received packet
void printPacket(Packet packet) {
    printf("Packet{ header: { len: %d }, data: \"", packet.header.len);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

// Server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen) {
    Packet packet;
    // sending acknowledgement packet
    strcpy(packet.data, "Acknowled");
    packet.header.len = sizeof(packet.data);

    // send packet
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
    printf("\t Server sending %s \n", packet.data);
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen) {
    Packet packet;
    while (1) {
        // recv packets from the client
        if (recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen) <
            0) {
            perror("ERROR: Could not read message from client\n");
            exit(1);
        }

        // print received packet
        printPacket(packet);

        // send acknowldgement
        serverSend(sockfd, address, *addrlen);
        break;
    }
    return packet;
}

int main(int argc, char *argv[]) {
    // Get from the command line, server IP and dst file
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }
    int port_number = atoi(argv[1]);
    char *dst_file = argv[2];

    // Declare socket file descriptor.
    int sockfd;

    // Open a UDP socket, if successful, returns a descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Setup the server address to bind using socket addressing structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    in_address.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_addr = in_address;

    // bind IP address and port for server endpoint socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        perror("Failed to bind");
        exit(1);
    }

    // open file given by argv[2], create if it does not exist
    int fp = open(argv[2], O_CREAT | O_RDWR, 0666);
    if (fp < 0) {
        perror("File failed to open\n");
        exit(1);
    }

    // get file contents (as packets) from client
    Packet packet;
    socklen_t addr_len = sizeof(client_addr);
    printf("Waiting for packets to come.....\n");
    do {
        packet =
            serverReceive(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        write(fp, packet.data, packet.header.len);
    } while (packet.header.len != 0);

    // close file and socket
    close(fp);
    close(sockfd);

    return 0;
}
