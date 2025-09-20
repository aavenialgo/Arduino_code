#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

// ... variables, coeficientes, funciones auxiliares como iirFilter() ...

void setup() {
  Wire.begin(6, 7);  // para ESP32-C6 DevKitM-1

  Serial.begin(115200);

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 no encontrado. Verifique conexiones.");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setSampleRate(125);
  particleSensor.setPulseWidth(411);
  particleSensor.setADCRange(4096);
}

void loop() {
  long irValue = particleSensor.getIR();

  Serial.print(irValue);
  Serial.print(",");

  delay(8); // 125 Hz
}
