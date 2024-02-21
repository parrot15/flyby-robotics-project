cmake_minimum_required(VERSION 3.10)
project(lidar_receiver)

# Define project source files.
set(PROJECT_SOURCES
    src/main.cpp
    src/utils.cpp
    src/lidar_receiver.cpp
)

# Define executable that will be built.
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link libraries.
target_link_libraries(${PROJECT_NAME} ouster_viz ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)