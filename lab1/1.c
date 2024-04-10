// Name: Thomas Solecki
// Date: 4/3/2024 (the day you have lab)
// Title: Lab1 – Step 1
// Description: This program copies a file using functions from fcntl

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

// copies a files from src_filename to dst_filename using functions fopen(),
// fread(), fwrite(), fclose()
int func_copy(char *src_filename, char *dst_filename) {

    FILE *src = fopen(src_filename, "r"); // opens a file for reading
    if (src == NULL) {                    // fopen() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", src_filename,
                errno);
        exit(0);
    }
    FILE *dst = fopen(
        dst_filename,
        "w"); // opens a file for writing; erases old file/creates a new file
    if (dst == NULL) { // fopen() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n",
                dst_filename, errno);
        exit(0);
    }

    char *buf =
        malloc((size_t)BUF_SIZE); // allocate a buffer to store read data

    // reads content of file is loop iterations until end of file
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, BUF_SIZE, src)) > 0) {
        // writes bytes_read to dst_filename
        fwrite(buf, 1, bytes_read, dst);
    }

    // closes src file pointer
    fclose(src);
    // closes dst file pointer
    fclose(dst);
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
    func_copy(src_filename, dst_filename);
    return 0;
}
