#!/bin/bash

# Navigate to the project root directory.
cd "$(dirname "$0")"

# Check for command line argument.
if [ $# -ne 1 ]; then
    echo "Usage: $0 <receiver|client>"
    exit 1
fi

if [ $1 == "receiver" ]; then
    # Stop and remove the receiver container.
    docker stop receiver && docker rm receiver
    # remove VNC server instance.
    vncserver -kill :1
elif [ $1 == "client" ]; then
    # Stop and remove the client container.
    docker stop client && docker rm client
else
    echo "Invalid argument: $1"
    echo "Please specify 'receiver' or 'client'."
    exit 1
fi