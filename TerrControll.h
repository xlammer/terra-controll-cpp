#ifndef TerrControll_H_
#define TerrControll_H_
#include "Arduino.h"
#include "DHT.h"
#include "LiquidCrystal.h"
#include <Wire.h>
#include "RTClib.h"

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
void printLCD(int t, int h);
void switchHeatingOn();
void switchHeatingOff();
void sendSerialMessage(float h, float t);
#ifdef __cplusplus
} // extern "C"
#endif

#endif
