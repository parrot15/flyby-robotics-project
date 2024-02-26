# Flyby Robotics Project
Eashan Soni

## Running the application
**Prerequisites**
To run the LIDAR receiver, you must be within a Linux VM, specifically a AWS EC2 GPU instance.
The g5.xlarge instance type is recommended (and is what was used in the screen recording).
Additionally, you must have a VNC server set up, and the X Display Manager `LightDM` installed.
Finally, you should have NVIDIA Docker installed and configured on your VM (the AWS EC2 GPU instance should automatically have this installed and configured).

To run the LIDAR client, you must be on your local machine. You should have Docker installed and setup on your machine. The LIDAR receiver must always be run before the LIDAR client.

**To run the receiver**
1. Clone the repository.
    ```
    git clone https://github.com/parrot15/flyby-robotics-project.git
    cd flyby_robotics_project
    ```
2. Build the Docker image.
    ```
    bash build.sh receiver
    ```
3. Run the Docker image.
    ```
    bash run.sh receiver
    ```

**To run the client**
1. Clone the repository.
    ```
    git clone https://github.com/parrot15/flyby-robotics-project.git
    cd flyby_robotics_project
    ```
2. Set up SSH tunnels for VNC viewer and data communication.
    ```
    bash local_setup.sh
    ```
3. Build the Docker image.
    ```
    bash build.sh client
    ```
4. Run the Docker image.
    ```
    bash run.sh client
    ```

## Misc
To clean up the instances, run `bash cleanup.sh <receiver|client>`.
To format the code, run `bash format.sh`.

## High-level overview
My solution meets all of the listed requirements:
* Both the client and receiver are containerized (run within Docker containers).
* The receiver runs on a Linux VM (AWS EC2 instance), while the client runs locally.
* The client and receiver communicate over a websocket.
* The client parses the lidar data, and for each frame: finds all points 15m away, and sends them as cartesian coordinates to the receiver.
* The receiver renders the 3D visualization dynamically as the points are received.
* Scripts are provided to easily build, start, and stop the containers.

Other information:
* CMake is used as the build system.
* External dependencies are managed by storing them in the `/external` directory, and tracking them through git submodules.
* The receiver's visualization is rendered through the VNC.