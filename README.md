# Simple FreeRTOS application
Simple FreeRTOS application on Arduino Mega 2560 with DS1820 temperature sensor + DFRobot LCD Keypad Shield V1.1.
The application is a final assignment for RTOS basics programming course. Atmel Studio 7 was used as the IDE.

## How it works
On the main screen, the LCD shield shows time, date and current temperature.
User can push on the shield buttons to change time and date + look for the maximum
and minimum measured temperature values.

Temperature data can also be viewed and plotted to an external app from the serial port,
currently sending temperature value every second.

## Buttons
- Up or Down: shows the max and min screen
- Select: change date or time (chosen with Up or Down buttons)
Display will go back to main view if no buttons are pressed in 5 seconds

## Schematic
- GND -> ground
- VDD -> 5V
- DQ -> A7 on 2560
- also DQ pin to VDD with 4.7k ohm pull-up resistor in between.

