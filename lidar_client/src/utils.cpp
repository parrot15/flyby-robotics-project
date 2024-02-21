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
  config.pcap_path = config_json["pcapPath"];
  config.json_path = config_json["jsonPath"];
  config.packet_buffer_size = config_json["packetBufferSize"];
  config.points_per_batch = config_json["pointsPerBatch"];
  config.distance_threshold = config_json["distanceThreshold"];

  return config;
}

std::shared_ptr<ouster::sensor_utils::playback_handle> read_playback_data(const std::string& pcap_path) {
  // Initialize playback data from PCAP file.
  auto handle = ouster::sensor_utils::replay_initialize(pcap_path);
  if (!handle) {
      std::stringstream ss;
      ss << "Failed to read PCAP file: " << pcap_path;
      throw std::runtime_error(ss.str());
  }
  std::cout << "Successfully read PCAP file: " << pcap_path << std::endl;
  return handle;
}

ouster::sensor::sensor_info read_playback_metadata(const std::string& json_path) {
  // Initialize playback metadata from JSON file.
  // metadata_from_json() already throws an error if failed to read.
  ouster::sensor::sensor_info meta = ouster::sensor::metadata_from_json(json_path);
  std::cout << "Successfully read JSON file: " << json_path << std::endl;
  return meta;
}