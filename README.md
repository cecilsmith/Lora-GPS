# Lora-GPS
A project for an Undergrad Freshman class where GPS data is collected from a LoraWan device, uploaded to the Cloud, and retrieved by a user through a web-gui. Practical uses include: hiker tracking, car tracking, bike tracking, etc.

<br>

## Technical Overview

The LoRa-GPS device uses a [Seeed LoRa Wio-E5 Module (STM32WLE5JC)](https://wiki.seeedstudio.com/LoRa-E5_STM32WLE5JC_Module/) for communication to [The Things Network (TTN)](https://www.thethingsnetwork.org). 
Data is gathered from a [Grove Air530 GPS Module](https://wiki.seeedstudio.com/Grove-GPS-Air530/) and uplinked to the Cloud using a variety of `AT+` commands. 
Both modules use `Serial` to communicate with the Arduino Nano, so the `SoftwareSerial` library was used.


<img width=75% alt="Screenshot 2023-04-25 at 6 00 48 PM" src="https://user-images.githubusercontent.com/19243227/234425601-886b474a-1d34-4fb5-99b8-84d941261870.png">

The data is then stored in the Cloud, to be used whenever the user sends his/her next request.

<br>

## Diagram

TBA

<br>

## Examples

Due to the early nature of LoRaWAN technologies, there are not as many hotspots set up in the U.S. as there are in Europe. Thus, it can be difficult to test the early viability of the technology. However, when within 2km of a LoRa Gateway, these were the results:

TBA


