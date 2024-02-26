#include "lidar_client.h"

LidarClient::LidarClient(std::shared_ptr<ouster::sensor_utils::playback_handle> handle, const ouster::sensor::sensor_info& meta, double distance_threshold)
    : handle_(handle), meta_(meta), distance_threshold_(distance_threshold), resolver_(ioc_), ws_(ioc_), is_connected_(false) {
}

LidarClient::~LidarClient() {
    if (handle_) {
        ouster::sensor_utils::replay_uninitialize(*handle_);
    }
}

void LidarClient::connect(const std::string& host, int port) {
    try {
        // Connect to websocket receiver.
        auto const results = resolver_.resolve(host, std::to_string(port));
        net::connect(ws_.next_layer(), results);
        ws_.handshake(host, "/");

        std::cout << "Successfully connected to lidar receiver." << std::endl;
        is_connected_ = true;
    } catch (const std::exception& e) {
        std::cout << "Failed to connect: " << e.what() << std::endl;
        is_connected_ = false;
    }
}

void LidarClient::process_scan(int packet_buffer_size, int points_per_batch) {
    // Create a LidarScan object.
    size_t w = meta_.format.columns_per_frame;
    size_t h = meta_.format.pixels_per_column;
    size_t num_points = w * h;
    ouster::LidarScan scan(w, h, meta_.format.udp_profile_lidar);

    // Send total number of points to lidar receiver.
    send_num_points(num_points);
    std::cout << "Sent number of points (" << num_points << ") to lidar receiver." << std::endl;

    // Buffer to store raw packet data.
    auto packet_buf = std::make_unique<uint8_t[]>(packet_buffer_size);
    ouster::sensor_utils::packet_info packet_info;

    // Read lidar scan data frame by frame.
    int first_frame_id = 0;
    auto packet_format = ouster::sensor::get_format(meta_);
    ouster::ScanBatcher batch_to_scan(w, packet_format);
    while (ouster::sensor_utils::next_packet_info(*handle_, packet_info)) {
        auto packet_size = ouster::sensor_utils::read_packet(*handle_, packet_buf.get(), packet_buffer_size);

        // Ensure that packet size and destination port match.
        if (packet_size != packet_format.lidar_packet_size || packet_info.dst_port != meta_.udp_port_lidar) {
            continue;
        }

        // Try to scan packet. If failed, just skip to next packet.
        if (!batch_to_scan(packet_buf.get(), scan)) {
            continue;
        }

        // If end of first frame, just assume it's incomplete and skip.
        if (first_frame_id == 0 || first_frame_id == scan.frame_id) {
            first_frame_id = scan.frame_id;
            continue;
        }

        // If new frame, then prepare and send data to receiver.
        if (first_frame_id != scan.frame_id) {
            process_coords(scan, points_per_batch);
        }
    }
}

void LidarClient::disconnect() {
    if (is_connected_) {
        try {
            ws_.close(websocket::close_code::normal);
            is_connected_ = false;
            std::cout << "Connection closed." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Failed to close connection: " << e.what() << std::endl;
        }
    }
}

void LidarClient::write_message(const json& message) {
    ws_.write(net::buffer(message.dump()));
}

void LidarClient::send_num_points(size_t num_points) {
    json num_points_msg = {
        {"num_points", num_points}
    };
    write_message(num_points_msg);
}

bool LidarClient::is_within_distance(double x, double y, double z) const {
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2)) < distance_threshold_;
}

void LidarClient::process_coords(const ouster::LidarScan& scan, int points_per_batch) {
    // Extract coordinates and convert them to cartesian form.
    ouster::XYZLut lut = ouster::make_xyz_lut(meta_);
    auto coords = ouster::cartesian(scan.field(ouster::sensor::ChanField::RANGE), lut);

    int total_points = coords.rows();
    for (int i = 0; i < total_points; i += points_per_batch) {
        bool is_last_batch = (i + points_per_batch >= total_points);
        json batch_json = {
            {"frameId", scan.frame_id},
            {"isLastBatch", is_last_batch},
            {"points", json::array()}
        };
        // Add points to batch.
        for (int j = i; j < i + points_per_batch && j < total_points; ++j) {
            double x = coords(j, 0);
            double y = coords(j, 1);
            double z = coords(j, 2);
            if (is_within_distance(x, y, z)) {
                batch_json["points"].push_back({
                    {"x", x},
                    {"y", y},
                    {"z", z},
                });
            }
        }
        if (!batch_json["points"].empty()) {
            write_message(batch_json);
        }
    }
}