# STM32F446 Sensor Test and Hardware Validation

This repository contains firmware for validating the analog and digital interfaces of the rainwater signal acquisition unit. It was used during development to test signal conditioning circuits, galvanic isolation, and sensor connectivity.

## 🚀 Features

- ADC channel testing for pressure, flow, and voltage sensors
- UART CLI interface for debugging and logging
- RS-485 and 1-Wire communication validation
- PWM signal generation and timer-based measurement
- Galvanic isolation verification for ultrasonic modules
- Real-time feedback via terminal and oscilloscope

## 🧠 CMSIS LL Driver Usage

- `LL_ADC_StartCalibration()`, `LL_ADC_REG_ReadConversionData32()` – ADC testing
- `LL_USART_ReceiveData8()`, `LL_USART_IsActiveFlag_RXNE()` – UART CLI
- `LL_TIM_SetPrescaler()`, `LL_TIM_EnableCounter()` – PWM and timing
- `LL_GPIO_SetOutputPin()` – toggling test signals

## 📁 Project Structure

- `Core/`: Sensor test routines and CLI
- `Drivers/`: STM32 LL drivers
- `.ioc`: STM32CubeMX configuration

## 🔗 Related Projects

- [STM32F446-Bootloader](https://github.com/Vojtese/STM32F446-Bootloader)
- [STM32F446-APP1](https://github.com/Vojtese/STM32F446-APP1)

## 📜 License

This project is licensed under the GNU General Public License v3.0.
