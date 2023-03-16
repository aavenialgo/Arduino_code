#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>

//------------Reemplazar las variables con SSID/Password ----------------------------------

const char* ssid = "NOMBRE-WIFI";
const char* password = "CONTRASEÑA-WIFI";
const char* mqtt_server = "IP-BROKER";
//---------------------------------------------------------
//---------------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
//---------------------------------------------------------
//---------------------------------------------------------
float temperature = 0;
float conductivity = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Temperature in Celsius
  temperature = random(15, 27);
  

  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);
  client.publish("tanque_001/temperatura", tempString);

  conductivity = random(50, 3000);


  //char condString[8];
  //dtostrf(conductivity, 1, 2, conString);
  //Serial.print("conductivity: ");
  //Serial.println(condString);
  //client.publish("tanque_001/conductividad", conductivity);
}