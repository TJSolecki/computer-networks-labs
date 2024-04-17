// Name: Thomas Solecki
// Date: 4/17/2024
// Title: Lab3 – Step 2
// Description: This program makes a client request to download a file from a
// server

// COEN 146L : Lab3, step 1: TCP client requesting a file transfer from the
// server.
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 10

int main(int argc, char *argv[]) {
    // Get from the command line, server IP, src and dst files.
    if (argc != 5) {
        printf("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",
               argv[0]);
        exit(0);
    }
    char *server_ip = argv[1];
    int port_num = atoi(argv[2]);
    char *src_file = argv[3];
    char *dst_file = argv[4];

    // Declare socket file descriptor and buffer
    int sockfd;
    char buff[BUFF_SIZE];
    bzero(buff, BUFF_SIZE);

    // Open a socket, if successful, returns
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set the server address to send using socket addressing structure
    struct sockaddr_in addr;
    struct in_addr in_adress;
    in_adress.s_addr = inet_addr(server_ip);
    addr.sin_addr = in_adress;
    addr.sin_port = htons(port_num);
    addr.sin_family = AF_INET;

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Could not connect to server");
        exit(1);
    }

    // Client sends the name of the file to retrieve from the server
    write(sockfd, src_file, strlen(src_file));

    // Client begins to write and read from the server
    int dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC);
    if (dst_fd < 0) { // open() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_file,
                errno);
        close(dst_fd);
        exit(0);
    }

    char *buf =
        malloc((size_t)BUFF_SIZE); // allocate a buffer to store read data
    bzero(buf, BUFF_SIZE);
    size_t bytes_read;

    // reads up to BUFF_SIZE bytes from src_file until end of file
    while ((bytes_read = read(sockfd, buf, BUFF_SIZE)) > 0) {
        // writes bytes_read to dst_filename
        if (write(dst_fd, buf, bytes_read) != bytes_read) {
            // handles error
            fprintf(stderr, "Error writing to %s\n", dst_file);
            close(sockfd);
            close(dst_fd);
            free(buf);
            exit(1);
        }
    }

    // Close socket descriptor
    close(sockfd);
    close(dst_fd);
    free(buf);

    return 0;
}
