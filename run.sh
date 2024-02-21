#!/bin/bash

# Initialize variables.
TARGET=""

# Function to display usage.
usage() {
    echo "Usage: $0 -t [client|receiver]"
    echo "  -t    Target to run: 'client' or 'receiver'"
    echo "Note: Make sure to build the desired target using build.sh before running this script."
    exit 1
}

# Parse command-line arguments.
while getopts ":t:" opt; do
    case ${opt} in
        t )
            TARGET=${OPTARG}
            ;;
        \? | : )
            usage
            ;;
    esac
done

# Validate target option.
if [ "$TARGET" != "client" ] && [ "$TARGET" != "receiver" ]; then
    echo "Error: Invalid or missing target specified."
    usage
fi

# Function to run Docker containers.
run_docker() {
    local container_name=$1

    echo "Running Docker container for $container_name..."
    docker run --rm -it "$container_name"
}

echo "***** Run script started. *****"

# Run Docker container based on target.
if [ -n "$TARGET" ]; then
    run_docker "$TARGET"
fi

echo "***** Run script completed. *****"
