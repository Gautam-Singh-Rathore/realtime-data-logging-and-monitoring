# Realtime Data Logging and Monitoring

## Overview

**Realtime Data Logging and Monitoring** is a system designed for collecting, logging, and monitoring sensor data in real-time using the MQTT protocol. The project integrates an ESP8266 microcontroller that publishes data to an MQTT broker. The server subscribes to this data, logs it, and provides a web dashboard for interactive monitoring and control.

### Key Features:
- **Real-time Data Logging**: Logs sensor data as it’s published.
- **Two-Way Communication**: Enables control of sensors directly from the frontend.
- **Interactive Web Dashboard**: View and manage data through a responsive, easy-to-use interface.
- **MQTT Protocol**: Efficiently handles messaging between devices and the server.

## Architecture

The system consists of the following components:

1. **ESP8266 Microcontroller (Publisher)**:
   - Collects sensor data and publishes it via MQTT to the broker.
  
2. **MQTT Broker (Mosquitto)**:
   - Manages communication between the ESP8266 and the server.

3. **Server (Subscriber)**:
   - Subscribes to MQTT topics to receive data, process it, and store it for logging.
  
4. **Frontend Dashboard**:
   - A web-based interface to visualize and control real-time data from the sensors.

## Technologies Used:
- **MQTT** (for message transport)
- **ESP8266** (for data collection and publishing)
- **Mosquitto** (MQTT broker)
- **Node.js** (for server-side logic and API)
- **React** (for frontend dashboard)
- **Chart.js**(for data visualization)

