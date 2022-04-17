// ECRF v1 Tesla Charge Door Opener
// Based on: https://github.com/fredilarsen/TeslaChargeDoorOpener

/*
 * Author: Fred Larsen
 * Github: www.github.com/fredilarsen
 * 
 * Mod: Joel Serna Moreno
 * Github: www.github.com/joelsernamoreno
 * 
*/

#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include <SPIFFSEditor.h>
#include <EEPROM.h>
#include "SPIFFS.h"
#include "SPI.h"
#include "FS.h"

#if defined(ESP8266)
    #define RECEIVE_ATTR ICACHE_RAM_ATTR
#elif defined(ESP32)
    #define RECEIVE_ATTR IRAM_ATTR
#else
    #define RECEIVE_ATTR
#endif

//Pushbutton Pins
int push1 = 34;
int push2 = 35;
int push3 = 32;

//CC1101 Pins
int RXPin = 26;
int RXPin0 = 4;
int TXPin0 = 2;
int Gdo0 = 25;

//BTN Sending Config
int btn_set_int;
String btn_set;
String btn1_frequency;
String btn1_mod;
String btn1_rawdata;
String btn1_deviation;
String btn1_transmission;
String btn2_frequency;
String btn2_mod;
String btn2_rawdata;
String btn2_deviation;
String btn2_transmission;
float tmp_btn1_deviation;
float tmp_btn2_deviation;
float tmp_btn1_frequency;
float tmp_btn2_frequency;
int tmp_btn1_mod;
int tmp_btn2_mod;
int tmp_btn1_transmission;
int tmp_btn2_transmission;
String bindataprotocol;
String bindata_protocol;
int pushbutton1 = 0;
int pushbutton2 = 0;
int pushbutton3 = 0;

// The signal to send
const uint16_t pulseWidth = 400;                // Microseconds
const uint16_t messageDistance = 23;            // Millis
const uint8_t transmissions = 5;                // Number of repeated transmissions
const uint8_t messageLength = 43;

const uint8_t sequence[messageLength] = { 
  0x02,0xAA,0xAA,0xAA,  // Preamble of 26 bits by repeating 1010
  0x2B,                 // Sync byte
  0x2C,0xCB,0x33,0x33,0x2D,0x34,0xB5,0x2B,0x4D,0x32,0xAD,0x2C,0x56,0x59,0x96,0x66,
  0x66,0x5A,0x69,0x6A,0x56,0x9A,0x65,0x5A,0x58,0xAC,0xB3,0x2C,0xCC,0xCC,0xB4,0xD2,
  0xD4,0xAD,0x34,0xCA,0xB4,0xA0};

void sendSignals() {
  pinMode(TXPin0,OUTPUT);
  ELECHOUSE_cc1101.setModul(0);
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setModulation(2);
  ELECHOUSE_cc1101.setMHZ(433.92);
  ELECHOUSE_cc1101.setDeviation(0);
  ELECHOUSE_cc1101.SetTx();
  
  for (uint8_t t=0; t<transmissions; t++) {
    for (uint8_t i=0; i<messageLength; i++) sendByte(sequence[i]);
      digitalWrite(TXPin0, LOW);
      delay(messageDistance);
    }
}

void sendByte(uint8_t dataByte) {
  for (int8_t bit=7; bit>=0; bit--) { // MSB
    digitalWrite(TXPin0, (dataByte & (1 << bit)) != 0 ? HIGH : LOW);
    delayMicroseconds(pulseWidth);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  ELECHOUSE_cc1101.addSpiPin(18, 19, 23, 5, 0);
  ELECHOUSE_cc1101.addSpiPin(18, 19, 23, 27, 1);
  pinMode(TXPin0, OUTPUT);
  digitalWrite(TXPin0, LOW);
  EEPROM.begin(4096);
  delay(2000);
  pinMode(push1, INPUT);
  pinMode(push2, INPUT);
  pinMode(push3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pushbutton1 = digitalRead(push1);
  pushbutton2 = digitalRead(push2);
  pushbutton3 = digitalRead(push3);

  if (pushbutton1 == LOW) {
    Serial.println("Start");
    sendSignals();
  }

  else if (pushbutton2 == LOW) {
    Serial.println("Start");
    sendSignals();
  }

  else if (pushbutton3 == LOW) {
    Serial.println("Start");
    sendSignals();
  }
}
