#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
TwoWire myWire(0);

// ===== Filtro biquad de orden 2 =====
class BiquadFilter {
  public:
    BiquadFilter(float b0, float b1, float b2, float a1, float a2)
      : b0(b0), b1(b1), b2(b2), a1(a1), a2(a2),
        x1(0), x2(0), y1(0), y2(0) {}

    float process(float x) {
      float y = b0*x + b1*x1 + b2*x2 - a1*y1 - a2*y2;
      x2 = x1; x1 = x;
      y2 = y1; y1 = y;
      return y;
    }

  private:
    float b0, b1, b2, a1, a2;
    float x1, x2, y1, y2;
};

// ===== Butterworth 4º orden pasa banda (0.5–15 Hz @ fs=100 Hz) =====
class ButterworthBandPass4 {
  public:
    ButterworthBandPass4()
      // Stage 1 (Q = 0.54): HP 0.5Hz
      : stage1(0.97803, -1.95606, 0.97803, -1.95558, 0.95654),
      // Stage 2 (Q = 1.31): HP 0.5Hz
        stage2(0.97724, -1.95448, 0.97724, -1.95358, 0.95537),

      // Stage 3 (Q = 0.54): LP 15Hz
        stage3(0.04953, 0.09907, 0.04953, -1.13967, 0.48551),
      // Stage 4 (Q = 1.31): LP 15Hz
        stage4(0.05831, 0.11662, 0.05831, -0.97043, 0.23399) {}

    float process(float x) {
      float y = stage1.process(x);
      y = stage2.process(y);
      y = stage3.process(y);
      return stage4.process(y);
    }

  private:
    BiquadFilter stage1;
    BiquadFilter stage2;
    BiquadFilter stage3;
    BiquadFilter stage4;
};

// ===== Normalización =====
float normalize(float x, float &minVal, float &maxVal) {
  if (x < minVal) minVal = x;
  if (x > maxVal) maxVal = x;
  return (x - minVal) / (maxVal - minVal + 1e-6);
}

ButterworthBandPass4 bpFilterRed;
float minRed = 1e9, maxRed = -1e9;

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(1000);
  myWire.begin(6,7);
  delay(1000);

  byte ledBrightness = 0x3F;
  byte sampleAverage = 4;
  byte ledMode = 2;
  int sampleRate = 400;
  int pulseWidth = 411;
  int adcRange = 16384;

  while (particleSensor.begin(myWire) == false) {
    Serial.println("Error al inicializar el sensor. Reintentando...");
    delay(500);
  }

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

// ===== Loop =====
void loop() {
  int32_t rawRed = -1*(int32_t)particleSensor.getIR();
  float red = (float)rawRed;

  float rawValue = red;
  float filtered = bpFilterRed.process(red);
  float normalized = normalize(filtered, minRed, maxRed);

  Serial.print("Crudo:");
  Serial.print(rawValue);
  Serial.print(",Filtrado:");
  Serial.print(filtered, 4);
  Serial.print(",Normalizado:");
  Serial.println(normalized, 4);
}
