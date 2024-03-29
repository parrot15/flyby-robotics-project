#!/bin/bash

# Navigate to the project root directory.
cd "$(dirname "$0")"

# Check for command line argument.
if [ $# -ne 1 ]; then
    echo "Usage: $0 <receiver|client>"
    exit 1
fi

if [ $1 == "receiver" ]; then
    echo "Building receiver Docker container..."
    docker build -f lidar_receiver/Dockerfile -t lidar_receiver .
elif [ $1 == "client" ]; then
    echo "Building client Docker container..."
    docker build -f lidar_client/Dockerfile -t lidar_client .
else
    echo "Invalid argument: $1"
    echo "Please specify 'receiver' or 'client'."
    exit 1
fi