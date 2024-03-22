#!/bin/bash

# Check if required arguments are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

filesdir=$1
searchstr=$2

# Check if filesdir exists and is a directory
if [ ! -d "$filesdir" ]; then
    echo "$filesdir is not a directory or does not exist"
    exit 1
fi

# Find matching lines and count them
num_files=$(find "$filesdir" -type f | wc -l)
num_matches=$(grep -r "$searchstr" "$filesdir" | wc -l)

echo "The number of files are $num_files and the number of matching lines are $num_matches"

