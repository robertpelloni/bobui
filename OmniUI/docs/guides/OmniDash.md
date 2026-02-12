# OmniDash User Guide

OmniDash is an IoT dashboard demonstrating mobile sensor access and cloud connectivity.

## Features
- **Mobile Sensors**: Displays Accelerometer and GPS data using `OmniMobile`.
- **Data Visualization**: Real-time charts using `OmniCharts`.
- **Cloud Control**: Publishes MQTT messages via `OmniCloud`.

## Architecture
- **Connectivity**: `OmniPubSub` handles real-time messaging.
- **Hardware**: `OmniSensors` and `OmniLocation` provide device data.
