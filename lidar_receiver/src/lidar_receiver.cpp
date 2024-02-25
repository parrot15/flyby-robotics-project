#include "lidar_receiver.h"

LidarReceiver::LidarReceiver()
    : is_connected_(false), num_points_(0) {
}

LidarReceiver::~LidarReceiver() {
    this->stop();
}

void LidarReceiver::listen(const std::string& host, int port) {
    acceptor_ = std::make_unique<tcp::acceptor>(
        ioc_,
        tcp::endpoint{net::ip::make_address(host), static_cast<unsigned short>(port)}
    );
    tcp_socket_ = std::make_unique<tcp::socket>(ioc_);
    std::cout << "Listening for lidar client." << std::endl;
}

void LidarReceiver::accept() {
    // Accept a connection.
    acceptor_->accept(*tcp_socket_);

    // Upgrade TCP connection to websocket.
    ws_ = std::make_unique<websocket::stream<tcp::socket>>(std::move(*tcp_socket_));
    ws_->accept();
    is_connected_ = true;
    std::cout << "Successfully connected to lidar client." << std::endl;
}

void LidarReceiver::start_visualization(const std::string& visualization_title) {
    if (!is_connected_) {
        return;
    }

    // Get total number of points from the client.
    this->receive_num_points();

    // Initialize visualizer.
    viz_ = std::make_unique<ouster::viz::PointViz>(visualization_title);
    ouster::viz::add_default_controls(*viz_);   // Add keyboard/mouse callbacks.
    viz_->target_display().enable_rings(true);  // Display distance rings.

    // Create a point cloud and register it with the visualizer.
    auto cloud = std::make_shared<ouster::viz::Cloud>(num_points_);
    viz_->add(cloud);

    // Start message handler in separate thread.
    message_thread_ = std::thread([this, cloud](){ this->message_handler(cloud); });

    // Run visualizer in main thread.
    viz_->run();
}

void LidarReceiver::stop() {
    // Join message handler thread before exiting.
    if (message_thread_.joinable()) {
        message_thread_.join();
    }
}

json LidarReceiver::read_message() {
    beast::flat_buffer buffer;
    ws_->read(buffer);
    auto msg = beast::buffers_to_string(buffer.data());
    return json::parse(msg);
}

void LidarReceiver::receive_num_points() {
    if (!is_connected_) {
        return;
    }

    // Get total number of points from client.
    auto received_json = read_message();
    num_points_ = received_json["num_points"];
    std::cout << "Received number of points (" << num_points_ << ") from lidar client." << std::endl;
}

void LidarReceiver::message_handler(std::shared_ptr<ouster::viz::Cloud> cloud) {
    try {
        // Flattened list to hold all coordinates in a frame,
        // as per Ouster SDK expectations.
        // Initialize with 0 to ensure all memory is allocated.
        std::vector<float> current_frame_points(3 * num_points_, 0);
        int current_frame_id = -1;
        size_t points_added = 0;
        for (;;) {
            // Check if the window was closed.
            if (!viz_->running()) {
                break;
            }

            auto received_json = read_message();
            int frame_id = received_json["frameId"];
            bool is_last_batch = received_json["isLastBatch"];
            auto points_batch = received_json["points"];

            // If first batch of a frame, then set current frame and
            // reset everything.
            if (frame_id != current_frame_id) {
                current_frame_id = frame_id;
                std::fill(current_frame_points.begin(), current_frame_points.end(), 0);
                points_added = 0;
            }

            // Add all points in batch.
            for (auto& point : points_batch) {
                size_t idx_base = points_added;
                current_frame_points[idx_base] = point["x"];
                current_frame_points[idx_base + num_points_] = point["y"];
                current_frame_points[idx_base + 2 * num_points_] = point["z"];
                ++points_added;
            }

            // If last batch of current frame, then the frame is ready.
            // So render the frame and reset everything.
            if (is_last_batch) {
                // Update visualizer's point cloud.
                cloud->set_xyz(current_frame_points.data());
                // Re-render visualizer.
                viz_->update();
                // Reset for the next frame.
                std::fill(current_frame_points.begin(), current_frame_points.end(), 0);
                points_added = 0;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Message handler error: " << e.what() << std::endl;
    }
}