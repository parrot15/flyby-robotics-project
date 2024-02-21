cmake_minimum_required(VERSION 3.10)
project(flyby_robotics_project)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost libraries installed system-wide.
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

# Include Ouster SDK from the external directory.
add_subdirectory(external/ouster_example)

# Include JSON library from the external directory.
add_subdirectory(external/json)

# Set flags for building.
option(BUILD_CLIENT "Build LIDAR client." OFF)
option(BUILD_RECEIVER "Build LIDAR receiver." OFF)

# Build client and receiver based on flags.
if(BUILD_CLIENT)
  add_subdirectory(lidar_client)
endif()
if(BUILD_RECEIVER)
  add_subdirectory(lidar_receiver)
endif()