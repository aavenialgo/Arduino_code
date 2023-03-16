void configurarWifi(WiFiMulti & wifiMulti){
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3); 

  while (wifiMulti.run(TiempoEsperaWifi)!=WL_CONNECTED){
    Serial.print(".");
  }
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  }
  Serial.println("Conectando al wifi...");

}
