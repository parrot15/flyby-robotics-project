#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * Configuration structure for the lidar receiver.
 */
struct Config {
  std::string host;                 // Hostname that websocket should run on.
  int port;                         // Port that websocket should run on.
  std::string visualization_title;  // The title for the visualization window.
};

/**
 * Reads the configuration file and returns a Config struct.
 * @param config_path The path to the configuration file.
 * @return A Config struct populated with the settings from the file.
 * @throw std::runtime_error Throws if couldn't open config file.
 */
Config read_config_file(const std::string& config_path);

#endif