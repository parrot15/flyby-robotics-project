#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "nlohmann/json.hpp"
#include "ouster/os_pcap.h"
#include "ouster/pcap.h"

using json = nlohmann::json;

/**
 * Configuration structure for the lidar receiver.
 */
struct Config {
  std::string host;        // Hostname that websocket should run on.
  int port;                // Port that websocket should run on.
  std::string pcap_path;   // Path to LIDAR data file.
  std::string json_path;   // Path to LIDAR metadata file.
  int packet_buffer_size;  // The size of the buffer for reading packets.
  int points_per_batch;    // # of points to send per websocket msg.
  int distance_threshold;  // The max distance from the sensor for points to be
                           // considered.
};

/**
 * Reads the configuration file and returns a Config struct.
 * @param config_path The path to the configuration file.
 * @return A Config struct populated with the settings from the file.
 * @throw std::runtime_error Throws if couldn't open config file.
 */
Config read_config_file(const std::string& config_path);

/**
 * Initializes and returns a playback handle for reading LIDAR data.
 * @param pcap_path The path to the LIDAR data file (.pcap).
 * @return A shared pointer to the initialized playback handle.
 */
std::shared_ptr<ouster::sensor_utils::playback_handle> read_playback_data(
    const std::string& pcap_path);

/**
 * Reads and returns the sensor metadata from a JSON file.
 * @param json_path The path to the LIDAR metadata file (.json).
 * @return A sensor_info struct populated with the metadata from the file.
 */
ouster::sensor::sensor_info read_playback_metadata(
    const std::string& json_path);

#endif