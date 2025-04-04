# ESP32 MQTT Simulation with Wokwi and PlatformIO

This project simulates an ESP32 device communicating via MQTT. It can be used to test and validate MQTT-based interactions in IoT environments without requiring physical hardware.

## Features

- Simulates an ESP32 device.
- Connects to an MQTT broker.
- Publishes and subscribes to MQTT topics.
- Uses [Wokwi](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) to simulate the ESP32 behavior.
- Managed using [PlatformIO](https://platformio.org).

Use [Wokwi](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) to simulate this project.

## Building

This is a [PlatformIO](https://platformio.org) project. To build it, [install PlatformIO](https://docs.platformio.org/en/latest/core/installation/index.html), and then run the following command:

```
pio run
```

## Simulate the project

To simulate this project, install [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode). Open the project directory in Visual Studio Code, press **F1** and select "Wokwi: Start Simulator".
