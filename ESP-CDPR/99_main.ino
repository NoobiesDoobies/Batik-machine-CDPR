void setup(){
  Serial.begin(115200);
  Serial2.begin(115200);
  initWiFi();

  handleGetRoot();

  server.addHandler(&events);
  server.begin();
  Serial.println("Done Setup");
}

void loop(){

}