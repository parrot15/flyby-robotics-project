#!/bin/bash

# Navigate to the project root directory.
cd "$(dirname "$0")"

# Check for command line argument.
if [ $# -ne 1 ]; then
    echo "Usage: $0 <receiver|client>"
    echo "Note: If running client, make sure to run local_setup.sh and build.sh before running this script."
    exit 1
fi

if [ $1 == "receiver" ]; then  # On Linux VM (AWS EC2 instance).
    # --- Commands to run the receiver container. ---
    # Set display number for VNC server.
    export DISPLAY=:1
    # Allow Docker container to connect to X display manager for VNC server.
    xhost +local:docker
    # Run Docker container with specific flags:
    #     -e: Pass display number environment variable.
    #     -v: Allow visualization to display through X11 forwarding onto host.
    docker run --gpus all --rm -it -p 8080:8080 -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --network="host" --name receiver lidar_receiver
    # Finished running, so release.
    xhost -local:docker
elif [ $1 == "client" ]; then  # Local.
    # --- Command to run the client container. ---
    docker run --rm -it -v "$(pwd)/lidar_client/data:/usr/src/app/data" --name client lidar_client
else
    echo "Invalid argument: $1"
    echo "Please specify 'receiver' or 'client'."
    exit 1
fi