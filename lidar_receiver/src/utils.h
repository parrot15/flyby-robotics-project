#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Config {
  std::string host;        // Hostname that websocket should run on.
  int port;                // Port that websocket should run on.
  std::string visualization_title;
};

Config read_config_file(const std::string& config_path);

#endif