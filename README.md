# STM32F446 Sensor Test and Hardware Interface

This repository contains firmware for testing sensors and hardware interfaces on the STM32F446 microcontroller. It is designed to validate analog and digital inputs, communication protocols, and signal conditioning circuits used in embedded hardware development.

## 🚀 Features

- Sensor input testing (ADC, GPIO, UART)
- CLI interface for debugging and validation
- Support for galvanically isolated communication
- STM32CubeIDE project structure
- Integration-ready with bootloader and IAP systems

## 📁 Project Structure

- `Core/`: Sensor test logic and hardware interface code
- `Drivers/`: STM32 LL drivers
- `.ioc`: STM32CubeMX configuration file
- `STM32F446RETX_FLASH.ld`: Linker script for standalone execution

## 🔗 Related Repositories

- [STM32F446-Bootloader](https://github.com/Vojtese/STM32F446-Bootloader)

## 📜 License

This project is licensed under the GNU General Public License v3.0.
