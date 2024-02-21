#ifndef LIDAR_RECEIVER_H
#define LIDAR_RECEIVER_H

#include <iostream>
#include <memory>
#include <thread>
#include "nlohmann/json.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "ouster/point_viz.h"

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = net::ip::tcp;                // from <boost/asio/ip/tcp.hpp>

using json = nlohmann::json;

class LidarReceiver {
  public:
  LidarReceiver();
  ~LidarReceiver();
  void listen(const std::string& host, int port);
  void accept();
  void start_visualization(const std::string& visualization_title);
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
  json read_message();
  void receive_num_points();
  void message_handler(std::shared_ptr<ouster::viz::Cloud> cloud);
};

#endif