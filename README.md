# BG22HwPeripheralsGatt
Created by Claudio Filho - Silicon Labs Staff FAE

This demonstrate how to use some of the Hardware Peripherals available for the EFR32BG22 and tying them into GATT Services, so you can enable and disable them with your smart Phone App.

Peripheral usage Demonstrated here Includes:

PWM Usage
ADC Usage
GPIOs Usage (Buttons and Leds)

The BG22HwPeripheralsGatt.isc has all the Gatt definitions and services that are tied to the Peripherals.

From a SmartPhone App Prospective you can use the EFR Connect App from Silicon Labs or any other Gatt Based Scanner App, like LightBlue from Punchthrough.

If Power Consumption is an issue, please disable the Debug Log feature at app.h. This should decrease significantly the power consumption and also disable the PWM. The PWM module only operates on EM1, so the lower currents (EM2, or Deep Sleep) can not be achieved if the PWM Duty Cycle is higher than 0.


Tools and SDKs:

This was tested with:

Silicon Labs Bluetooth Stack 2.13.3.0

GNU GCC Compiler: arm-none-eabi-gcc GNU ARM 7.2.1

Development Board: Silicon Labs BRD4182 (EFR32MG22A224F512IM40) Radio Board and  and Silabs Wireless Starter Kit (WSTK)

Usage:

Please clone this to where your Simplicity Studio Workspace is located.

Then to import it, please do:

File -> Import -> More Import Options -> General -> Existing Projects Into Workspace -> Select Workspace directory -> Select Project -> Finish


The IADC portion of this code was based on the following Silabs GitHub Example:

https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/iadc/iadc_single_em2

Hardware Connections:

PWM Pin - PC0 - WSTK EXP4 Header
ADC Pin - PC2 - WSTK EXP8 Header
Button0 - WSTK PB0
Button1 - WSTK PB1
LED0 - WSTK LED0
LED1 - WSTK LED1
