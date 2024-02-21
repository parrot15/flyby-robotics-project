cmake_minimum_required(VERSION 3.10)
project(lidar_client)

# Define project source files.
set(PROJECT_SOURCES
    src/main.cpp
    src/utils.cpp
    src/lidar_client.cpp
)

# Define executable that will be built.
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link libraries.
target_link_libraries(${PROJECT_NAME} ouster_pcap ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)