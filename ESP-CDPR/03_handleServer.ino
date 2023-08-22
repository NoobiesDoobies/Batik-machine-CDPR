#include <ArduinoJson.h>
void handleGetRoot(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("GETTING /");
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(1024);
        json["message"] = "PONG";
        json["status"] = "ok";
        json["ip"] = WiFi.softAPIP().toString();
        serializeJson(json, *response);
        request->send(response);
    });

    server.on("/coords", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("GETTING /coords");
      int params = request->params();
      float x, y, z;

      AsyncWebParameter* p = request->getParam(0);
      x = p->value().toFloat();
      p = request->getParam(1);
      y = p->value().toFloat();
      p = request->getParam(2);
      z = p->value().toFloat();

      // Serial.println("x: " + String(x) + " y: " + String(y) + " z: " + String(z));
      Serial.println(String(x) + " " + String(y) + " " + String(z));

      AsyncResponseStream *response = request->beginResponseStream("application/json");
      DynamicJsonDocument json(1024);
      json["status"] = "ok";
      json["ssid"] = WiFi.SSID();
      json["ip"] = WiFi.softAPIP().toString();
      serializeJson(json, *response);
      request->send(response);
    });
}