# Code

During the development and testing phase of this project, it became clear that the Arduino Nano was not up for the task. In my testing, I was not able to get two simultaneous `SoftwareSerial` instances running at the same time, and when I tried to include the display, there was not enough SRAM for the Nano to operate. Thus, I had to turn to the Arduino Nano 33 IoT board. With this board, I was able to create two custom UART (Serial) interfaces using the leftover SERCOM ports (i.e. I was able to run two hardware Serial ports). Not only did this increase efficiency, it also reduced the overall flash size.


# Useful Sources

## Microcontroller Resources
[Amazing LoRa Datasheet](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20AT%20Command%20Specification_V1.0%20.pdf)

[Grove Seeed Wio-E5 Lora P2P Manual](https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/#grove---wio-e5-p2p-example.)

[Grove Seeed Air530 GPS](https://wiki.seeedstudio.com/Grove-GPS-Air530)

[GitHub Library for Wio-E5](https://github.com/disk91/Disk91_LoRaE5)

## Web (React.js Resources)

* Next.js
 * [MUI](https://github.com/mui/material-ui/tree/master/examples/material-next)
* [Leaflet](https://leafletjs.com) for web mapping
* Possibly [Netlify](netlify.com) for hosting
  * [Netlify API Calls](https://docs.netlify.com/edge-functions/api/)
  * [Netlify .env](https://cli.netlify.com/commands/env/#envimport)
