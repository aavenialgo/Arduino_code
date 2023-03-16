#if defined(ESP8266)
#pragma message "Se detecto una placa ESP8266"
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
ESP8266WiFiMulti wifiMulti;
#elif defined(ESP32)
#pragma message "Se detecto una placa ESP32"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
WiFiMulti wifiMulti;
#else 
#pragma message "No se detecto una placa ESP32 o ESP8266"
#endif

#include <SPIFFS.h>

#include "datos.h"
#include "serverConf.hpp"
#include "mDNSconf.hpp"
#include "wifiConf.hpp"


unsigned long TiempoActual = 0;
unsigned long TiempoAnterior = 0;
unsigned long TiempoCancelacion = 500;

int pinLed = 2;
boolean Estado = false; 
WiFiServer server(80);

const char* mqtt_server = "";

void setup() {
  Serial.begin(115200);
  delay(100);
  configurarWifi(wifiMulti);

  server.begin();
	Serial.println("Se inicio el servidor...");


  //Configuración  del GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
  configurarMDNS();
    
}

void loop() {
  #if defined(ESP8266)
  MDNS.update();
  #endif
  
  WiFiClient cliente = server.available();

  if (cliente) {
    Serial.println("Nuevo Cliente");
    TiempoActual = millis();
    TiempoAnterior = TiempoActual;
    String LineaActual = "";

    while (cliente.connected() && TiempoActual - TiempoAnterior <= TiempoCancelacion) {
      if (cliente.available()) {
        TiempoActual = millis();
        char Letra = cliente.read();
        if (Letra == '\n') {
          if (LineaActual.length() == 0) {
            digitalWrite(pinLed, Estado);
            ResponderCliente(cliente);
            break;
          } else {
            Serial.println(LineaActual);
            VerificarMensaje(LineaActual);
            LineaActual = "";
          }
        }  else if (Letra != '\r') {
          LineaActual += Letra;
        }
      }
    }

    cliente.stop();
    Serial.println("Cliente Desconectado");
    Serial.println();
  }
}

void VerificarMensaje(String Mensaje) {
  if (Mensaje.indexOf("GET /ihost") >= 0) {
    Serial.println("ihost");
    Estado = true;
  } else if (Mensaje.indexOf("GET /apagar") >= 0) {
    Serial.println("Apagar Led");
    Estado = false;
  }
}

void ResponderCliente(WiFiClient& cliente) {

  cliente.println("<!DOCTYPE HTML>");
  cliente.println("<html>");
  cliente.println("<head><title>Nodemcu server </title>");
  cliente.println("<body>");
  cliente.println("<h1 align='center'>Test </h1>");
  cliente.println("<div style='text-align:center;'>");
  cliente.println("<form action="/action_page.php" target="_blank">");
  cliente.println("<label for="ihost">Ingrese host:</label><br>");
  cliente.println("<input type="text" id="ihost" name="host" value=""><br>");
  cliente.println("<input type="submit" value="Enviar">");
  cliente.println("</form>");
  cliente.println("<body>");
  cliente.print("</html>");
}
  