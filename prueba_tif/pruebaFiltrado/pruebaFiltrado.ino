#include <Wire.h>
#include "MAX30105.h"
#include <Filters.h>

FilterOnePole lowpassFilter(LOWPASS, 0.5);
FilterOnePole highpassFilter(HIGHPASS, 15);

MAX30105 particleSensor;
TwoWire myWire(0);

void setup() {
 
  Serial.begin(115200);
  delay(1000);
  myWire.begin(6,7);
  delay(1000);

  byte ledBrightness = 0x3F; //Options: 0=Off to 255=50mA  0x0F
  byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green No corresponde verde para la max30102
  int sampleRate = 125; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  // Initializar sensor

  while (particleSensor.begin(myWire) == false) {
  Serial.println("Error al inicializar el sensor. Reintentando...");
  delay(500); 
}
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configuracion del sensor

}

void loop() {

  Serial.print("Red:");
  int32_t red = -1 * (int32_t)particleSensor.getRed();
  int32_t ir = -1 * (int32_t)particleSensor.getIR();
  Serial.print(red);
  Serial.print(",IR:");
  Serial.print(ir);
  Serial.print("\r\n");
}