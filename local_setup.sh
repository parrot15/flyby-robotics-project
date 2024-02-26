#!/bin/bash

# Navigate to the project root directory.
cd "$(dirname "$0")"

# Configuration variables.
EC2_IP="34.221.199.148"  # IP address of EC2 instance.
PEM_FILE="~/.ssh/aws-lidar-receiver.pem"  # Path to AWS key.
SSH_USER="ubuntu"  # Default EC2 login username.

# Setup SSH tunnel for the receiver's VNC visualization.
ssh -N -Y -f -i "${PEM_FILE}" -L 5902:localhost:5901 "${SSH_USER}@${EC2_IP}"
echo "Connect your VNC viewer to 0.0.0.0:5902."

# Setup SSH tunnel for the client's communication.
ssh -L 8080:localhost:8080 -N -f -i "${PEM_FILE}" "${SSH_USER}@${EC2_IP}"