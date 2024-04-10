// Name: Thomas Solecki
// Date: 4/3/2024 (the day you have lab)
// Title: Lab1 – Step 2
// Description: This program copies a file using the system functions from fcntl

// COEN 146L : Lab 1 - template to use for file transfer (steps 1, 2, and 3)
// You need to adapt this template according to the requirements of each of the
// steps

#include <errno.h>    // errno
#include <fcntl.h>    // open(), close()
#include <pthread.h>  // pthread_create()
#include <stdio.h>    // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>   // malloc(), free()
#include <sys/mman.h> // mmap()
#include <unistd.h>   // read(), write()

#define BUF_SIZE 2048 // buffer size

// copies a files form src_filename to dst_filename using syscalls open(),
// read(), write(), close()
int syscall_copy(char *src_filename, char *dst_filename) {

    int src_fd = open(src_filename, O_RDONLY); // opens a file for reading
    if (src_fd < 0) {                          // open() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", src_filename,
                errno);
        close(src_fd);
        exit(0);
    }
    // O_WRONLY - write only
    // O_CREAT - create file if it doesn't exist
    // O_TRUNC - truncates file to length 0 if it already exists (deletes
    // existing file) opens a file for writing; erases old file/creates a new
    // file
    int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (dst_fd < 0) { // open() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n",
                dst_filename, errno);
        close(dst_fd);
        exit(0);
    }

    // read/ write loop
    char *buf =
        malloc((size_t)BUF_SIZE); // allocate a buffer to store read data
    size_t bytes_read;

    // reads up to BUF_SIZE bytes from src_filename until end of file
    while ((bytes_read = read(src_fd, buf, BUF_SIZE)) > 0) {
        // writes bytes_read to dst_filename
        if (write(dst_fd, buf, bytes_read) != bytes_read) {
            // handles error
            fprintf(stderr, "Error writing to %s\n", dst_filename);
            close(src_fd);
            close(dst_fd);
            free(buf);
            exit(1);
        }
    }

    // closes src_fd file descriptor
    close(src_fd);
    // closes dst_fd file descriptor
    close(dst_fd);
    // frees memory used for buf
    free(buf);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // check correct usage
        fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
        exit(1);
    }
    // get the source and destination files from the command line arguments
    char *src_filename = argv[1];
    char *dst_filename = argv[2];
    // call the check copy times function
    syscall_copy(src_filename, dst_filename);
    return 0;
}
