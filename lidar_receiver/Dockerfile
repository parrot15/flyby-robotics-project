# Use NVIDIA OpenGL base image
FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu20.04
# FROM ubuntu:latest

# Set working directory.
WORKDIR /usr/src/app

# Avoid interactive dialog during build.
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary system-wide dependencies for LIDAR receiver and Ouster SDK.
RUN apt-get update && apt-get install -y \
  # Boost websocket dependencies.
  libboost-system-dev \
  libboost-thread-dev \
  # OpenGL dependencies (for visualization).
  # libglvnd0 \
  # libgl1 \
  # libglx0 \
  # libegl1 \
  # libxext6 \
  # libx11-6 \
  # mesa-utils \
  # libgl1-mesa-glx \
  # libgl1-mesa-dri \
  # Ouster SDK sub-dependencies.
  build-essential \
  cmake \
  libjsoncpp-dev \
  libeigen3-dev \
  libcurl4-openssl-dev \
  libtins-dev \
  libpcap-dev \
  libglfw3-dev \
  libglew-dev \
  libspdlog-dev \
  && rm -rf /var/lib/apt/lists/*

# Reset DEBIAN_FRONTEND to its default value.
ENV DEBIAN_FRONTEND=

# Copy everything into working directory.
COPY . .

# Ouster SDK needs to be built from source.
RUN mkdir -p /usr/src/app/external/ouster_example/build && \
  cd /usr/src/app/external/ouster_example/build && \
  cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON && \
  cmake --build .

# Build the LIDAR receiver.
RUN mkdir -p /usr/src/app/build && \
  cd /usr/src/app/build && \
  cmake .. -DBUILD_RECEIVER=ON && \
  cmake --build . --target lidar_receiver

# Make port 8080 available to the outside world.
EXPOSE 8080

# Set the binary as the container's entrypoint.
ENTRYPOINT [ "/usr/src/app/build/lidar_receiver/lidar_receiver" ]