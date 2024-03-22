#!/bin/bash

# Check if required arguments are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

writefile=$1
writestr=$2

# Create the directory path if it doesn't exist
mkdir -p "$(dirname "$writefile")"

# Write the content to the file
echo "$writestr" > "$writefile"

# Check if the file was created successfully
if [ $? -ne 0 ]; then
    echo "Error: Unable to create file $writefile"
    exit 1
fi

echo "Content successfully written to $writefile"

