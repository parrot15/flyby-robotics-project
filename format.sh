#!/bin/bash

# Navigate to the project root directory.
cd "$(dirname "$0")"

# Format all C++ source files in lidar_receiver/src and lidar_client/src directories using Google's styling convention.
find ./lidar_receiver/src ./lidar_client/src -iname *.cpp -o -iname *.h | xargs clang-format -i -style=google

echo "Formatting complete."