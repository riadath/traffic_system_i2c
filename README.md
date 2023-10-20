# STM32F4 I2C Device Drivers

This repository contains device drivers for I2C (Inter-Integrated Circuit) communication on STM32F4 microcontrollers with ARM Cortex-M4 processors. The drivers allow you to configure and use the I2C interface for various applications.

## Features

- I2C initialization and configuration.
- Master mode for transmitting data.
- Slave mode for receiving data.
- Start and stop conditions.
- Address handling and data transmission.
- Error and event interrupts.

## Files

- `I2C.h`: Header file for I2C driver functions.
- `I2C.c`: Source file containing I2C driver implementation.
- `GPIO.h`, `USART.h`, `SYS_INIT.h`: Header files for GPIO, USART, and system initialization (you might have other drivers/modules as well).
- `main.c`: Example usage of the I2C driver.

## Usage

1. Include the necessary header files in your project.
2. Initialize and configure I2C using `I2C1_Config(mode)`, where `mode` can be 0 for slave mode or 1 for master mode.
3. Use functions like `I2C1_Start`, `I2C1_Write`, and `I2C1_Stop` to control I2C communication.
4. For master mode, use `I2C1_TransmitMaster` to send data to a slave device.
5. For slave mode, use `I2C1_ReceiveSlave` to receive data from a master device.
6. Handle interrupts and errors as needed.

## Example

You can refer to the `main.c` file for an example of how to use the I2C driver for both master and slave modes.

## License

This project is open-source and provided under the [license](LICENSE) included in this repository.
