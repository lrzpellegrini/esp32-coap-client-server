# ESP32 CoAP Demo

This project demonstrates a CoAP (Constrained Application Protocol) server and client implementation on ESP32.

## Getting Started

### Prerequisites
1. **Install Visual Studio Code**: Download from [https://code.visualstudio.com/](https://code.visualstudio.com/)

2. **Install PlatformIO Extension**: 
   - Open VS Code
   - Go to Extensions
   - Search for "PlatformIO IDE" and install it

3. **Install Wokwi Extension**:
   - In VS Code Extensions, search for "Wokwi Simulator"
   - Install the official Wokwi extension

4. **Create Wokwi Account**:
   - Visit [https://wokwi.com](https://wokwi.com)
   - Register for a free account
   - In Visual Studio Code, when the Wokwi extension asks for a license, follow the setup steps
   - The license is free - just follow the steps!

### Running the Project
1. **Open Project**: Open this folder in VS Code.
    - Wait for the Platform IO estension: it will automatically install the required tools and dependencies
2. **Build**: Use PlatformIO to build the project (Ctrl+Alt+B)
3. **Simulate**: Open `diagram.json`, the ESP32 and its attached led should appear. Press the play button to start the simulation
    - You can also run the simulation by opening the VSCode command palette (F1 usually), and use `Wokwi: Start Simulator`
    - If it doesn't work, open `wokwi.toml` and adjust the path slashes: `\` for Windows, `/` for other operating systems
    - Wokwi should automatically open the serial monitor

## File Structure

### Main Files
- **`main.cpp`**: Main application entry point, handles WiFi setup and starts the server and client components
  - For Wokwi simulation: WiFi connection is automatically simulated
- **`coap_server.h/cpp`**: CoAP server implementation with resource handlers
- **`coap_client.h/cpp`**: CoAP client implementation for making requests

### Architecture Overview

#### CoAP Server (`CoapServer` class)
- **Purpose**: Handles incoming CoAP requests
- **Resources**:
  - `/hello`: Responds with "world" to GET requests
  - `/led`: Controls ESP32 built-in LED (GET/PUT/POST)
- **Features**:
  - GET `/led`: Returns LED state ("on"/"off")
  - PUT/POST `/led`: Sets LED state (accepts "on"/"off", "1"/"0", "true"/"false")

#### CoAP Client (`CoapClient` class)
- **Purpose**: Makes CoAP requests to test server functionality
- **Behavior**:
  - Sends a one-time GET request to `/hello` after 5 seconds
  - Periodically toggles LED by sending POST requests to `/led` every 2 seconds
  - Handles and displays responses from server

## Key Features

1. **Clean Separation**: Server and client code are completely separated into different classes
2. **Self-Testing**: Client automatically tests server functionality
3. **Resource Management**: Clear REST-like resource handlers
4. **Wokwi Compatible**: Ready for simulation in Wokwi environment (use it in VSCode with the proper extension!)

## Usage

The application will:
1. Connect to WiFi (simulated using Wokwi)
2. Start CoAP server on UDP port 5683
3. Start CoAP client on UDP port 56830
4. Automatically test server functionality with client requests

Note: it also accepts external CoAP requests from tools like `coap-client`, but you need to actually deploy it to a physical hardware to make it work. Not tested!

## Dependencies
- ESP32 Arduino Core
- WiFi library
- CoAP simple library (hirotakaster)
