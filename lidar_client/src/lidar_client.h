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

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

using json = nlohmann::json;

class LidarClient {
  public:
  /**
   * Constructs a LidarClient with a playback handle, sensor metadata, and a distance threshold.
   * @param handle A shared pointer to the playback handle for reading lidar data.
   * @param meta Sensor metadata containing information like the lidar's resolution.
   * @param distance_threshold The maximum distance from the sensor for points to be considered.
   */
  LidarClient(std::shared_ptr<ouster::sensor_utils::playback_handle> handle, const ouster::sensor::sensor_info& meta, double distance_threshold);
  
  /**
   * Destructor for LidarClient.
   * Ensures proper cleanup of resources.
   */
  ~LidarClient();

  /**
   * Establishes a connection to a lidar receiver over WebSocket.
   * @param host The hostname or IP address of the lidar receiver.
   * @param port The port number on which the lidar receiver is listening.
   */
  void connect(const std::string& host, int port);

  /**
   * Processes lidar scan data and sends it to the connected receiver.
   * @param packet_buffer_size The size of the buffer for reading packets.
   * @param points_per_batch The number of points to send in each batch.
   */
  void process_scan(int packet_buffer_size, int points_per_batch);

  /**
   * Closes the WebSocket connection to the lidar receiver.
   */
  void disconnect();

  private:
  std::shared_ptr<ouster::sensor_utils::playback_handle> handle_;
  ouster::sensor::sensor_info meta_;
  double distance_threshold_;
  net::io_context ioc_;
  tcp::resolver resolver_;
  websocket::stream<tcp::socket> ws_;
  bool is_connected_;

  /**
   * Serialize and write message to websocket.
   * @param message The JSON to serialize and send.
  */
  void write_message(const json& message);

  /**
   * Sends the total number of points in the lidar scan to the receiver.
   * @param num_points The total number of points in the lidar scan.
   */
  void send_num_points(size_t num_points);

  /**
   * Checks if a point is within the specified distance threshold from the origin.
   * @param x The x-coordinate of the point.
   * @param y The y-coordinate of the point.
   * @param z The z-coordinate of the point.
   * @return True if the point is within the distance threshold, false otherwise.
   */
  bool is_within_distance(double x, double y, double z) const;

  /**
   * Processes the coordinates from a lidar scan and sends them in batches to the receiver.
   * @param scan The lidar scan containing the points to process.
   * @param points_per_batch The number of points to include in each batch.
   */
  void process_coords(const ouster::LidarScan& scan, int points_per_batch);
};

#endif