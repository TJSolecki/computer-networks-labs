// Name: Thomas Solecki
// Date: 4/10/2024
// Title: Lab1 – Step 1
// Description: This program copies n files in concurrently using threads

#include <errno.h>   // errno
#include <fcntl.h>   // open(), close()
#include <pthread.h> // pthread_create()
#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <time.h>    // clock()
#include <unistd.h>  // read(), write()

#define BUF_SIZE 2048 // buffer size

// data structure to hold copy
struct copy_struct {
    int thread_id;
    char *src_filename;
    char *dst_filename;
};

// copies a files from src_filename to dst_filename using functions fopen(),
// fread(), fwrite(), fclose()
int func_copy(char *src_filename, char *dst_filename) {
    // opens a file for reading
    FILE *src = fopen(src_filename, "r");
    if (src == NULL) { // fopen() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", src_filename,

                errno);
        exit(0);
    }
    // opens a file for writing; erases old file/creates a new file
    FILE *dst = fopen(dst_filename, "w");

    if (dst == NULL) { // fopen() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n",
                dst_filename, errno);
        exit(0);
    }

    // allocate a buffer to store read data
    char *buf = malloc((size_t)BUF_SIZE);

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

// thread function to copy one file
void *copy_thread(void *arg) {
    struct copy_struct params =
        *(struct copy_struct *)arg; // cast/dereference void* to copy_struct
    printf("thread[%i] - copying %s to %s\n", params.thread_id,
           params.src_filename, params.dst_filename);
    // call file copy function
    func_copy(params.src_filename, params.dst_filename);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // check correct usage of arguments in command line
    if (argc < 4) {
        fprintf(stderr, "usage: %s <n connections> ........\n", argv[0]);
        exit(1);
    }
    int i;
    int num_threads = atoi(argv[1]);

    if (argc < 2 + num_threads * 2) {
        fprintf(stderr,
                "usage: %s <n connections> <src_file_names>... "
                "<dst_file_names>... \n",
                argv[0]);
        exit(1);
    }

    char *src_filenames[num_threads]; // array of source files
    for (i = 0; i < num_threads; i++) {
        src_filenames[i] = argv[2 + i];
    }

    char *dst_filenames[num_threads]; // array of desintation files
    for (i = 0; i < num_threads; i++) {
        dst_filenames[i] = argv[2 + num_threads + i];
    }
    pthread_t threads[num_threads]; // initialize threads
    int return_value;
    struct copy_struct thread_params[num_threads]; // structure for each thread
    for (i = 0; i < num_threads; i++) {
        // initialize thread parameters
        thread_params[i].thread_id = i + 1;
        thread_params[i].src_filename = src_filenames[i];
        thread_params[i].dst_filename = dst_filenames[i];
        // create each copy thread
        // use pthread_create(.....);
        return_value =
            pthread_create(&threads[i], NULL, copy_thread, &thread_params[i]);
        if (return_value != 0) {
            fprintf(stderr, "pthread_create failed with error code %d\n",
                    return_value);
            exit(1);
        }
    }

    // wait for all threads to finish
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_exit(NULL);
}
