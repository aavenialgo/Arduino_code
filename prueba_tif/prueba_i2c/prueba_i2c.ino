#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Iniciando I2C en GPIO6 (SDA), GPIO7 (SCL)");
  Wire.begin(6, 7); // ESP32-C6: SDA=GPIO6, SCL=GPIO7
  delay(100);

  Serial.println("Escaneando bus I2C...");
  int found = 0;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Encontrado dispositivo I2C en 0x");
      Serial.println(address, HEX);
      found++;
      delay(10);
    }
  }
  if (found == 0) {
    Serial.println("No se encontró ningún dispositivo I2C.");
    Serial.println("¿Cableado correcto? ¿Fuente correcta? ¿Pull-ups?");
  } else {
    Serial.print("Total encontrados: ");
    Serial.println(found);
  }
  Serial.println("Fin del escaneo.");
}

void loop() {
  // Nada aquí
}

