#include "utils.h"

Config read_config_file(const std::string& config_path) {
  // Read config file.
  std::ifstream config_file(config_path);
  if (!config_file.is_open()) {
    std::stringstream ss;
    ss << "Could not open the config file: " << config_path << "\n";
    throw new std::runtime_error(ss.str());
  }
  json config_json;
  config_file >> config_json;
  config_file.close();

  // Populate config struct.
  Config config;
  config.host = config_json["host"];
  config.port = config_json["port"];
  config.visualization_title = config_json["visualizationTitle"];

  return config;
}