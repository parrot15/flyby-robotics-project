#ifndef LIDAR_RECEIVER_H
#define LIDAR_RECEIVER_H

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <memory>
#include <thread>

#include "nlohmann/json.hpp"
#include "ouster/point_viz.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

using json = nlohmann::json;

class LidarReceiver {
 public:
  /**
   * Constructor for LidarReceiver.
   * Initializes the receiver with default values.
   */
  LidarReceiver();

  /**
   * Destructor for LidarReceiver.
   * Ensures proper resource cleanup.
   */
  ~LidarReceiver();

  /**
   * Starts listening for incoming connections on the specified host and port.
   * @param host The IP address to listen on.
   * @param port The port number to listen on.
   */
  void listen(const std::string& host, int port);

  /**
   * Accepts an incoming connection and upgrades it to a WebSocket connection.
   */
  void accept();

  /**
   * Starts the visualization process with a given title.
   * @param visualization_title The title for the visualization window.
   */
  void start_visualization(const std::string& visualization_title);

  /**
   * Stops the receiver and joins the message handling thread.
   */
  void stop();

 private:
  net::io_context ioc_;
  std::unique_ptr<tcp::acceptor> acceptor_;
  std::unique_ptr<tcp::socket> tcp_socket_;
  std::unique_ptr<websocket::stream<tcp::socket>> ws_;
  bool is_connected_;
  std::thread message_thread_;
  int num_points_;
  std::unique_ptr<ouster::viz::PointViz> viz_;

  /**
   * Read and de-serialize message from websocket.
   * @return The de-serialized JSON.
   */
  json read_message() const;

  /**
   * Receives the total number of points from the connected client.
   * This function should be called before starting visualization.
   */
  void receive_num_points();

  /**
   * Handles incoming messages and updates the visualization accordingly.
   * @param cloud A shared pointer to the cloud visualization object.
   */
  void message_handler(std::shared_ptr<ouster::viz::Cloud> cloud) const;
};

#endif