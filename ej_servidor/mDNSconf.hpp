void configurarMDNS(){
  if (!MDNS.begin("aguapotable")) {
    Serial.println("Error configurando mDNS!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS configurado");

  Serial.println("Ingrese desde un navegador web usando la siguiente IP:");
  Serial.println(WiFi.localIP()); //Obtenemos la IP
  MDNS.addService("http", "tcp", 80);
}