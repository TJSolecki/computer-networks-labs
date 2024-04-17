#!/bin/bash

dd if=/dev/urandom of=src.txt bs=1M count=1
touch dst.txt
# Loop through numbers from 1 to 10
for i in {1..10}; do
    # Generate file name
    filename="$i.txt"
    dst_filename="$i.dst.txt"
    
    # Generate 1MB of random data
    dd if=/dev/urandom of=$filename bs=1M count=1
    touch $dst_filename
done
