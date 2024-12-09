# DriftKart

This project demonstrates the creation of a robot controlled by a PS4 controller using an **ESP32-DevKitC-S1** and an **NXP FRDM-KL25Z** microcontroller, running on **Keil RTX 5 RTOS**. The robot can move, play tunes, shine LEDs, and perform concurrent tasks using multi-threading.

Check out [this](https://www.youtube.com/watch?v=uCeixj1EkN8) video!

## Overview

### Components:

- **ESP32-DevKitC-S1**: Receives input from the PS4 controller via Bluetooth.
- **NXP FRDM-KL25Z**: Receives data from the ESP32 via UART and controls the robot's motors using PWM.
- **Keil RTX 5 RTOS**: Manages multiple concurrent tasks including:
  - Controlling motor movement.
  - Playing tunes.
  - Shining LEDs.
  - Handling UART communication.

### Features:

- **PS4 Controller Integration**: The controller connects to the ESP32 via Bluetooth, allowing the user to control the movement of the robot.
- **UART Communication**: The ESP32 sends data to the KL25Z over UART, relaying commands for motor control.
- **PWM Motor Control**: The KL25Z drives 4 motors using Pulse Width Modulation (PWM) based on received commands.
- **Multi-threading**: Different tasks (e.g., motor control, tunes, LEDs) run concurrently using RTX RTOS.

## Project Structure

`ESP`:

- Code to connect the Xbox controller to ESP32, process signals, and transmit commands over UART.

`KL25Z`:

- Code for receiving UART data, motor control using PWM, and handling additional tasks (LEDs, tunes).
- RTOS thread configuration and task management setup.
