#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include "ouster/os_pcap.h"
#include "ouster/pcap.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Config {
  std::string host;        // Hostname that websocket should run on.
  int port;                // Port that websocket should run on.
  std::string pcap_path;   // Path to LIDAR data file.
  std::string json_path;   // Path to LIDAR metadata file.
  int packet_buffer_size;  // 
  int points_per_batch;    // # of points to send per websocket msg.
  int distance_threshold;  // 
};

Config read_config_file(const std::string& config_path);
std::shared_ptr<ouster::sensor_utils::playback_handle> read_playback_data(const std::string& pcap_path);
ouster::sensor::sensor_info read_playback_metadata(const std::string& json_path);

#endif