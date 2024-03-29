#include <iostream>

#include "lidar_client.h"
#include "utils.h"

const Config config = read_config_file("lidar_client/config/config.json");

int main() {
  auto handle = read_playback_data(config.pcap_path);
  auto meta = read_playback_metadata(config.json_path);

  LidarClient lidar_client(handle, meta, config.distance_threshold);
  lidar_client.connect(config.host, config.port);
  lidar_client.process_scan(config.packet_buffer_size, config.points_per_batch);
  lidar_client.disconnect();

  return 0;
}