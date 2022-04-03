
#include <TinyWireM.h>

#include <Tiny4kOLED.h>
#include "font16x24.h"

#include <SoftSerial.h>
#include <TinyPinChange.h>

SoftSerial mySerial(4, 1); // RX, TX

uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
unsigned char co2SensorResponse[9];

unsigned int oldPPM = 0;

// ----------------------------------------------------------------------------

void setup() {
  // This example is for a 128x64 screen
  //oled.begin(128, 64, sizeof(tiny4koled_init_128x64r), tiny4koled_init_128x64r);
  oled.begin();
  oled.setFont(FONT16X24);
  oled.clear();
  oled.on();

  mySerial.begin(9600);
}


void loop() {
  mySerial.txMode();

  for (byte i = 0; i < 9; i++)
  {
    mySerial.write(cmd[i]);
  }

  mySerial.rxMode();

  bool received = false;
  int ls = 0;
  int hs = 0;
  int i = 0;
  int statusByte = 231;
  int temperature = 0;

  while (mySerial.available() > 0) {

    received = true;

    int val = mySerial.read();

    co2SensorResponse[i] = val;

    switch (i) {
      case 2:
        ls = val;
        break;
      case 3:
        hs = val;
        break;
      case 4:
        temperature = val - 40;
        break;
      case 5:
        statusByte = val;
        break;
    }
    i++;
  }

  if (received) {

    if (validateCo2Response()) {

      unsigned int ppm = (256 * ls) + hs;

      //New text at center position
      if (oldPPM != ppm) {
        if (ppm > 999) {
          oled.setCursor(32, 0);
        } else { 
          if (oldPPM > 999) {
            oled.clear();
          }
          oled.setCursor(40, 0);
        }

        oldPPM = ppm;
        oled.print(ppm);
      }
    }
  }

  delay(10000);
}

bool validateCo2Response() {
  byte crc = 0;
  for (int i = 1; i < 8; i++) {
    crc += co2SensorResponse[i];
  }
  crc = 256 - crc;
  //crc++;
  bool valid = co2SensorResponse[0] == 0xFF && co2SensorResponse[1] == 0x86 && co2SensorResponse[8] == crc;
  /* if (!valid) {
    Serial.println("CRC error: " + String(crc) + "/" + String(co2SensorResponse[8]));
    } */
  return valid;
}
