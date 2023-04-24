// Automated AT check
#include <SoftwareSerial.h>

SoftwareSerial loraSerial(2, 3); // RX, TX pins for the LoRaWAN module
int baudRate = 9600;

void setup()
{
  Serial.begin(9600);
  loraSerial.begin(baudRate);
}
char cmd[] = "AT\r\n";
void loop()
{
  // Send AT commands to the LoRaWAN module
  for (int i=0; i<strlen(cmd); i++) {
    loraSerial.write(cmd[i]);
    Serial.write(cmd[i]);
  }
  //loraSerial.println("AT");
  //Serial.println("AT");

  delay(10000);
  // Read response from the LoRaWAN module
  while (loraSerial.available() > 0)
  {
    Serial.write(loraSerial.read());
  }
}

// User inputted AT check (useful for AT+ID)
/*
#include<SoftwareSerial.h>

SoftwareSerial LoraSerial(2, 3); // (RX, TX)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  LoraSerial.begin(9600);
}

void loop() {
      while (Serial.available() > 0) {
        LoraSerial.write(Serial.read());
    }

    while (LoraSerial.available() > 0) {
        Serial.write(LoraSerial.read());
    }
}
*/
