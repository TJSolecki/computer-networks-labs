// Name: Thomas Solecki
// Date: 4/17/2024
// Title: Lab3 – Step 4
// Description: This program is a server that returns requested files to clients
// concurrently using threads to handle requests

// COEN 146L : Lab3, step 3: concurrent TCP server that accepts and responds to
// multiple client connection requests, each requesting a file transfer
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct request_data {
    struct sockaddr_in client_addr;
    int client_fd;
};

// Define the number of clients/threads that can be served
#define BUFF_SIZE 10
#define N 5
int threadCount = 0;
pthread_t clients[N]; // declaring N threads

// Declare server address to which to bind for receiving messages and client
// address to fill in sending address
struct sockaddr_in server_addr;
struct in_addr in_address;

// Connection handler (thread function) for servicing client requests for file
// transfer
void *connectionHandler(void *req_data) {
    // declate buffer holding the name of the file from client
    char requested_file[BUFF_SIZE];
    memset(requested_file, 0, BUFF_SIZE);
    // get the connection descriptor
    int client_fd = ((struct request_data *)req_data)->client_fd;
    struct sockaddr_in client_addr =
        ((struct request_data *)req_data)->client_addr;

    // Connection established, server begins to read and write to the
    // connecting client
    printf("Connection Established with client IP: %s and Port: %d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // receive name of the file from the client
    if (read(client_fd, requested_file, BUFF_SIZE) < 0) {
        perror("Could not recieve requested file name from client");
        exit(1);
    }

    int src_fd = open(requested_file, O_RDONLY); // opens a file for reading
    if (src_fd < 0) {                            // open() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", requested_file,
                errno);
        exit(0);
    }

    // open file and send to client
    size_t bytes_read;
    char buff[BUFF_SIZE];

    sleep(2);
    // reads up to BUF_SIZE bytes from src_fd until end of file
    while ((bytes_read = read(src_fd, buff, BUFF_SIZE)) > 0) {
        // writes bytes_read to client tcp socket
        if (write(client_fd, buff, bytes_read) != bytes_read) {
            // handles error
            fprintf(stderr, "Error writing to client tcp socket\n");
            free(req_data);
            close(src_fd);
            close(client_fd);
            exit(1);
        }
    }

    printf("File transfer complete\n");

    free(req_data);

    // close file
    close(src_fd);

    // Close connection descriptor
    close(client_fd);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Get from the command line, server IP, src and dst files.
    if (argc != 2) {
        printf("Usage: %s <port #> \n", argv[0]);
        exit(0);
    }
    int port = atoi(argv[1]);

    // Declare socket and connection file descriptors.
    int socket_fd;
    // Open a TCP socket, if successful, returns a descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // Setup the server address to bind using socket addressing structure
    struct sockaddr_in server_addr;
    struct in_addr in_address;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    in_address.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_addr = in_address;

    // bind IP address and port for server endpoint socket
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        perror("Failed to bind");
        exit(1);
    }

    // Server listening to the socket endpoint, and can queue 5 client requests
    if (listen(socket_fd, 5) < 0) {
        perror("Listening on the socket failed");
        exit(1);
    }
    printf("Server listening/waiting for client at port %d\n", port);

    // Server accepts the connection and call the connection handler
    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr_length);
    while (1) {
        // Server accepts the connection and call the connection handler
        int new_socket = accept(socket_fd, (struct sockaddr *)&client_addr,
                                &client_addr_length);
        if (new_socket < 0) {
            perror("Could not accept request");
            exit(1);
        }

        struct request_data *req_data = malloc(sizeof(struct request_data));
        req_data->client_addr = client_addr;
        req_data->client_fd = new_socket;

        if (pthread_create(&clients[threadCount % N], NULL, connectionHandler,
                           (void *)req_data) < 0) {
            perror("Unable to create a thread");
            exit(0);
        } else
            printf("Thread %d has been created to service client request\n",
                   ++threadCount);
    }

    close(socket_fd);
    for (int i = 0; i < threadCount; i++) {
        pthread_join(clients[i], NULL);
    }
    return 0;
}
