// THIS EXAMPLE ALSO USES A SMALL OLED TO VIEW GPS DATA

#include <Arduino.h>
#include "GPS_Air530.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEBUG 1            // set to 0 to disable debugging features (more efficient code)
#define EXTERNAL_DISPLAY 1 // set to 0 to disable this feature

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// SoftwareSerial SoftSerial(2, 3);
// SoftwareSerial GPSSerial(2, 3);

#if EXTERNAL_DISPLAY == 1
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

Air530Class Air530;

double latitude, longitude;

int parseGLLData(String GLLData);

void setup()
{
#if DEBUG == 1
  Serial.begin(9600);
  delay(100);
  Serial.println("Serial started");
#endif

#if EXTERNAL_DISPLAY == 1
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
#if DEBUG == 1
    Serial.println(F("SSD1306 allocation failed"));
#endif
  }
  else
  {
    display.clearDisplay();
    display.display();
  }
#endif

  Air530.begin();
  Air530.reset();

  /* The modes supported are:
   * GPS        :    MODE_GPS - this works
   * GPS+BEIDOU :    MODE_GPS_BEIDOU this works
   * GPS+GLONASS:    MODE_GPS_GLONASS this works
   * GPS+GALILEO:    MODE_GPS_GALILEO this does not work
   * GPS+BEIDOU+GLONASS+GALILEO:   MODE_GPS_MULTI   this does not work
   * default mode is GPS+BEIDOU.
   */

  Air530.setmode(MODE_GPS_GLONASS); // was commented out
#if (EXTERNAL_DISPLAY == 1) && (DEBUG == 1)
  String sampleGLL = "$GNGLL,4458.02481,N,09313.50745,W,232623.000,A,A*58";
  if (parseGLLData(sampleGLL))
  {
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.print("Lat: ");
    display.println(latitude, 5);
    display.print("Long: ");
    display.println(longitude, 5);
    display.display();
  }
#endif

  String NMEA = Air530.getGSA();

  /*supported nmea sentence :
   * GLL, RMC, VTG, GGA, GSA, GSV
   */
  Air530.setNMEA(NMEA_GLL | NMEA_GSV | NMEA_VTG); // was commented out  and had  (NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);

  /* set PPS mode and pulse width.
   *  void setPPS(uint8_t mode, uint16_t pulse_width = 500);
   *  mode : 0, pps off;
   *         1, first fixed;
   *         2, 3D fixed;
   *         3, 2D/3D fixed;
   *         4, always on;
   *  width : max value is 998 ms, default value is 500ms;
   */
  // Air530.setPPS(3, 500); // Was commented out
#if DEBUG == 1
  Serial.println(NMEA);
#endif
}

void loop()
{
  /*get nmea sentence
   * GPS.getNMEA() to get an any kind of nmea sentence;
   * GPS.getRMC() to get RMC sentence;
   * GPS.getGGA() to get GGA sentence;
   * GPS.getGSA() to get GSA sentence;
   * GPS.getGSV() to get GSV sentence;
   * GPS.getGLL() to get GLL sentence;
   * GPS.getVTG() to get VTG sentence;
   */
  // String NMEA = Air530.getNMEA();
  String NMEA = Air530.getGLL();

  if (parseGLLData(NMEA))
  {
#if EXTERNAL_DISPLAY == 1
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.print("Lat: ");
    display.println(latitude, 5);
    display.print("Long: ");
    display.println(longitude, 5);
    display.display();
#endif
  }

#if DEBUG == 1
  if (NMEA != "0")
  {
    Serial.println(NMEA);
  }
#endif
}

int parseGLLData(String GLLData)
{
  int latPoint = GLLData.indexOf('.');
  int longPoint = GLLData.indexOf('.', latPoint + 1);

  // Ensure data is there to be gleaned
  if ((latPoint > 0) && (longPoint > 0) && (GLLData.length() > 28))
  {
    latitude = GLLData.substring(GLLData.indexOf(',') + 1, latPoint - 2).toDouble() + GLLData.substring(latPoint - 2, latPoint).toDouble() / 60.0 + GLLData.substring(latPoint, GLLData.indexOf(',', latPoint)).toDouble() / 36.0;

    if (GLLData[GLLData.indexOf(',', latPoint) + 1] == 'S')
    {
      latitude *= -1;
    }

    longitude = GLLData.substring(longPoint - 5, longPoint - 2).toDouble() + GLLData.substring(longPoint - 2, longPoint).toDouble() / 60.0 + GLLData.substring(longPoint, GLLData.indexOf(',', longPoint)).toDouble() / 36.0;

    if (GLLData[GLLData.indexOf(',', longPoint) + 1] == 'W')
    {
      longitude *= -1;
    }

#if DEBUG == 1
    Serial.print(latitude, 5);
    Serial.print(", ");
    Serial.println(longitude, 5);
#endif
    return 1;
  }
  else
    return 0;
}
