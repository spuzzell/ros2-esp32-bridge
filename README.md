# ros2-esp32-bridge

A bridge for integrating ROS2 (Robot Operating System) with ESP32-based embedded systems. This project enables bidirectional communication between ROS2 nodes and ESP32 microcontrollers, supporting robotics applications such as sensor data acquisition, motor control, and real-time feedback.

## Features

- **Bidirectional communication** between ROS2 and ESP32
- Encoder and motor driver support
- Differential PID controller implementation
- Serial command parsing for flexible control
- Unit tests for reliable development

## Directory Structure
├── include/ # Header files 
├── lib/ # Libraries (controllers, drivers, utilities) 
├── src/ # Main application source code 
├── test/ # Unit and integration tests 
├── platformio.ini # PlatformIO project configuration 
└── README.md # Project documentation


## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed in your IDE (e.g., VS Code)
- ESP32 development board
- ROS2 installed on your host machine

### Building and Uploading

1. Clone this repository:
    ```sh
    git clone https://github.com/yourusername/ros2-esp32-bridge.git
    cd ros2-esp32-bridge
    ```

2. Open the project in VS Code with PlatformIO extension.

3. Connect your ESP32 board and build/upload the firmware:
    ```sh
    pio run --target upload
    ```

### Running Tests

Run unit tests using PlatformIO:
```sh
pio test
