#ifndef LIDAR_CLIENT_H
#define LIDAR_CLIENT_H

#include <iostream>
#include <memory>
#include "nlohmann/json.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "ouster/os_pcap.h"
#include "ouster/pcap.h"
#include "ouster/lidar_scan.h"

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = net::ip::tcp;                // from <boost/asio/ip/tcp.hpp>

using json = nlohmann::json;

class LidarClient {
  public:
  LidarClient(std::shared_ptr<ouster::sensor_utils::playback_handle> handle, const ouster::sensor::sensor_info& meta, double distance_threshold);
  ~LidarClient();
  void connect(const std::string& host, int port);
  void process_scan(int packet_buffer_size, int points_per_batch);
  void disconnect();

  private:
  std::shared_ptr<ouster::sensor_utils::playback_handle> handle_;
  ouster::sensor::sensor_info meta_;
  double distance_threshold_;
  net::io_context ioc_;
  tcp::resolver resolver_;
  websocket::stream<tcp::socket> ws_;
  bool is_connected_;

  bool is_within_distance(double x, double y, double z);
  void process_coords(const ouster::LidarScan& scan, int points_per_batch);
};

#endif