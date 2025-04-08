# 🔐 Seguridad en Comunicaciones MQTT para Entornos Industriales

Este proyecto tiene como objetivo **garantizar la seguridad de las comunicaciones entre dispositivos en un entorno industrial simulado**, centrándose en la protección del servidor que aloja el **broker MQTT**.

## 🏭 Descripción General

Se implementa una infraestructura segura en un **servidor VPS**, que simula un entorno industrial real. Utilizando herramientas modernas, se diseña un flujo de trabajo que:

- Utiliza el protocolo **MQTT** como medio de comunicación entre dispositivos.
- **Limita, controla y monitorea** los accesos al servidor.
- Simula un microcontrolador que se conecta y transmite datos al broker MQTT de forma segura.

## 🔧 Tecnologías y Herramientas

- **MQTT** – Protocolo de mensajería liviano para IoT.
- **VPS** – Simulación de un entorno industrial.
- **Docker**, **Traefik**, **Cloudflare** – Para gestión de servicios y refuerzo de seguridad.
- **Cloudflare Zero Trust** – Para restringir el acceso solo a dispositivos autorizados.
- **Node-RED** – Visualización y gestión de flujos MQTT.
- **ESP32 (simulado)** – Representación de un microcontrolador enviando datos.

## 🎯 Objetivos

- Implementar un entorno MQTT funcional y seguro.
- Evitar accesos no autorizados al broker MQTT.
- Garantizar la **confidencialidad**, **integridad** y **disponibilidad** de las comunicaciones.
- Integrar una interfaz de monitoreo y análisis de datos.

## 📷 Vista Previa
![alt text](/recursos/image.png)

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
