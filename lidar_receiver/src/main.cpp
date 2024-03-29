#include <iostream>

#include "lidar_receiver.h"
#include "utils.h"

const Config config = read_config_file("lidar_receiver/config/config.json");

int main() {
  LidarReceiver lidar_receiver;
  lidar_receiver.listen(config.host, config.port);
  lidar_receiver.accept();
  lidar_receiver.start_visualization(config.visualization_title);
  lidar_receiver.stop();

  return 0;
}