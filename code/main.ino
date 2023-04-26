#include <Arduino.h>
#include "GPS_Air530.h"
#include <Wire.h>
#include <stdio.h>
#include "wiring_private.h"

static char recv_buf[512];
static bool is_exist = false;
static bool is_join = false;

Air530Class Air530;

// Parse the data strings from GPS

double latitude, longitude;

// Comes out of module like this: "$GNGLL,4458.02481,N,09313.50745,W,232623.000,A,A*58"
int parseGLLData(String GLLData)
{
    int latPoint = GLLData.indexOf('.');
    int longPoint = GLLData.indexOf('.', latPoint + 1);

    // Ensure data is there to be gleaned
    if ((latPoint > 0) && (longPoint > 0) && (GLLData.length() > 28))
    {
        // Convert the string of hours, minutes, seconds, to decimal format
        latitude = GLLData.substring(GLLData.indexOf(',') + 1, latPoint - 2).toDouble() + GLLData.substring(latPoint - 2, latPoint).toDouble() / 60.0 + GLLData.substring(latPoint, GLLData.indexOf(',', latPoint)).toDouble() / 36.0;

        // Check if 'South' (negative latitude)
        if (GLLData[GLLData.indexOf(',', latPoint) + 1] == 'S')
        {
            latitude *= -1;
        }

        // Convert the string of hours, minutes, seconds to decimal format
        longitude = GLLData.substring(longPoint - 5, longPoint - 2).toDouble() + GLLData.substring(longPoint - 2, longPoint).toDouble() / 60.0 + GLLData.substring(longPoint, GLLData.indexOf(',', longPoint)).toDouble() / 36.0;

        // Check if 'West' (negative longitude)
        if (GLLData[GLLData.indexOf(',', longPoint) + 1] == 'W')
        {
            longitude *= -1;
        }

        Serial.print(latitude, 5);
        Serial.print(", ");
        Serial.println(longitude, 5);
        return 1;
    }
    else
        return 0;
}

// --------------------------

static int at_send_check_response(char *p_ack, uint16_t timeout_ms, char *p_cmd, ...)
{
    int ch;
    int index = 0;
    uint32_t startMillis = 0;
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    Serial1.print(p_cmd);
    Serial.print(p_cmd);
    va_end(args);
    delay(200);
    startMillis = millis();

    if (p_ack == NULL)
        return 0;

    do
    {
        while (Serial1.available() > 0)
        {
            ch = Serial1.read();
            recv_buf[index++] = ch;
            Serial.print((char)ch);
            delay(2);
        }

        if (strstr(recv_buf, p_ack) != NULL)
            return 1;

    } while ((millis() - startMillis) < timeout_ms);
    Serial.println();
    return 0;
}

static void recv_prase(char *p_msg)
{
    if (p_msg == NULL)
    {
        return;
    }
    char *p_start = NULL;
    int data = 0;
    int rssi = 0;
    int snr = 0;

    p_start = strstr(p_msg, "RX");
    if (p_start && (1 == sscanf(p_start, "RX: \"%d\"\r\n", &data)))
    {
        Serial.println(data);
    }

    p_start = strstr(p_msg, "RSSI");
    if (p_start && (1 == sscanf(p_start, "RSSI %d,", &rssi)))
    {
        Serial.print("rssi: ");
        Serial.println(rssi);
    }
    p_start = strstr(p_msg, "SNR");
    if (p_start && (1 == sscanf(p_start, "SNR %d", &snr)))
    {
        Serial.print("snr: ");
        Serial.println(snr);
    }
    p_start = strstr(p_msg, "TX");
    Serial.println(p_start);
    if (p_start && (1 == sscanf(p_start, "+TEST: TX %s", &data)))
    {
        Serial.print("data: ");
        Serial.println(data);
    }
}

// ----------------------------------------------------------

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600); // Serial for debugging

    Serial1.begin(9600); // Serial for LoRa

    Air530.begin();
    Air530.reset();
    Air530.setmode(MODE_GPS_GLONASS); // was commented out

    String sampleGLL = "$GNGLL,4458.02481,N,09313.50745,W,232623.000,A,A*58";
    if (parseGLLData(sampleGLL))
    {
        Serial.print("Lat: ");
        Serial.println(latitude);
        Serial.print("Long : ");
        Serial.println(longitude);
    }

    String NMEA = Air530.getGSA();
    Air530.setNMEA(NMEA_GLL | NMEA_GSV | NMEA_VTG); // was commented out  and had  (NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);
    Serial.println(NMEA);

    if (at_send_check_response("+AT: OK", 100, "AT\r\n"))
    {
        is_exist = true;
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
        at_send_check_response("+DR: US915", 1000, "AT+DR=US915\r\n");
        at_send_check_response("+CH: NUM,1-8", 1000, "AT+CH=NUM,8-15\r\n");
        at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"D657BA1D676CE69E428C57259FAB7B96\"\r\n");
        at_send_check_response("+CLASS: A", 1000, "AT+CLASS=A\r\n");
        at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
        delay(200);
        is_join = true;
    }
    else
    {
        is_exist = false;
        Serial.print("No E5 module found.\r\n");
    }
}

void loop(void)
{
    // Code for GPS data
    String NMEA = Air530.getGLL();
    if (parseGLLData(NMEA))
    {
        /*display.clearDisplay();
        display.setTextSize(1);              // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE); // Draw white text
        display.setCursor(0, 0);             // Start at top-left corner
        display.print("Lat: ");
        display.println(latitude, 5);
        display.print("Long: ");
        display.println(longitude, 5);
        display.display();*/
        Serial.print("Lat: ");
        Serial.println(latitude);
        Serial.print("Long : ");
        Serial.println(longitude);
    }

    if (NMEA != "0")
    {
        Serial.println(NMEA);
    }

    // -----------------------------------------------

    if (is_exist)
    {
        int ret = 0;
        if (is_join)
        {
            // TURN OFF THE BUILT-IN LED
            digitalWrite(LED_BUILTIN, LOW);

            ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
            if (ret)
            {
                is_join = false;
            }
            else
            {
                at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
                Serial.print("JOIN failed!\r\n\r\n");
                delay(5000);
            }
        }
        else
        {
            // TURN ON THE BUILT-IN LED ONCE JOINED
            digitalWrite(LED_BUILTIN, HIGH);

            char cmd[128];
            sprintf(cmd, "AT+CMSGHEX=\"%04A%04A\"\r\n", (double)latitude, (double)longitude);
            ret = at_send_check_response("Done", 5000, cmd);
            if (ret)
            {
                recv_prase(recv_buf);
            }
            else
            {
                Serial.print("Send failed!\r\n\r\n");
            }
            delay(5000);
        }
    }
}
