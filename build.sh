#!/bin/bash

# Initialize variables.
TARGET=""
DOCKER_BUILD=false

# Function to display usage.
usage() {
    echo "Usage: $0 -t [client|receiver] [-d]"
    echo "  -t    Target to build: 'client' or 'receiver'"
    echo "  -d    Optional flag to build Docker images for the specified target"
    exit 1
}

# Parse command-line arguments.
while getopts ":t:d" opt; do
    case ${opt} in
        t )
            TARGET=${OPTARG}
            ;;
        d )
            DOCKER_BUILD=true
            ;;
        \? )
            usage
            ;;
    esac
done

# Validate target option.
if [ "$TARGET" != "client" ] && [ "$TARGET" != "receiver" ]; then
    echo "Error: Invalid target specified."
    usage
fi

# Function to build projects.
build_project() {
    local build_dir="build"
    mkdir -p $build_dir && cd $build_dir
    
    case $TARGET in
        client)
            cmake .. -DBUILD_CLIENT=ON
            cmake --build . --target lidar_client
            ;;
        receiver)
            cmake .. -DBUILD_RECEIVER=ON
            cmake --build . --target lidar_receiver
            ;;
    esac
    
    cd ..
}

# Function to build Docker images.
build_docker() {
    local project_name=$1

    echo "Building Docker image for $project_name..."
    docker build -f $project_name/Dockerfile -t $project_name $project_name
}

echo "***** Build script started. *****"

# Build projects.
echo "Building target: $TARGET"
build_project

# Build Docker images based on flags.
if $DOCKER_BUILD; then
    build_docker "$TARGET"
fi

echo "***** Build script completed. *****"
