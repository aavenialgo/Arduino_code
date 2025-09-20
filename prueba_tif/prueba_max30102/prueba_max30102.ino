#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Wire.begin(6,7);
  delay(1000);

  byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384
  // Initialize sensor
  // if (particleSensor.begin() == false) {
  //   Serial.println("not found. Please check wiring/power.");
  //   while (1)
  //     ;
  // }
  particleSensor.begin();
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop() {
  Serial.print(" R[");
  Serial.print(particleSensor.getRed());
  Serial.print("] IR[");
  Serial.print(particleSensor.getIR());
  Serial.println("]");
  
}