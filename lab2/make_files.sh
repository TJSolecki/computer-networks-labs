#!/bin/bash

# Loop through numbers from 1 to 10
for i in {1..10}; do
    # Generate file name
    filename="$i.txt"
    
    # Generate 1MB of random data
    dd if=/dev/urandom of=$filename bs=1M count=1
    
    echo "File $filename created."
done

# for i in {11..20}; do
#     # Generate file name
#     filename="$i.txt"
    
#     touch $filename
    
#     echo "File $filename created."
# done
