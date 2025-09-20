#include <Wire.h>
#include "MAX30102_PulseOximeter.h"

#define REPORTING_PERIOD_MS 10 // cada cuánto reportar BPM/SpO2
#define PLOT_PERIOD_MS 10      // cada cuánto plotear la onda

PulseOximeter pox;
uint32_t tsLastReport = 0;
uint32_t tsLastPlot = 0;


void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(6,7); // ESP32-C6: SDA=6, SCL=7

    Serial.print("Initializing..");
    delay(3000);

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);

    Serial.println("Ready! Place your finger on the sensor.");
}

void loop() {
    pox.update(); // lee sensor, calcula BPM/SpO2, detecta latidos

    // Reporta BPM y SpO2 cada REPORTING_PERIOD_MS
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.print("%");
        Serial.println();
        tsLastReport = millis();
    }

    // Plotear la onda IR cruda cada PLOT_PERIOD_MS
    if (millis() - tsLastPlot > PLOT_PERIOD_MS) {
        // La función getIR() puede no estar en todas las librerías,
        // pero en tu caso está y funciona:
        Serial.print("IR:");
        Serial.println(pox.getIR()); // ESTE FALTA IMPLEMENTAR
        tsLastPlot = millis();
    }
}