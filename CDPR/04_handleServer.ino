// #include <ArduinoJson.h>
// void handleGetRoot(){
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//         Serial.println("GETTING /");
//         AsyncResponseStream *response = request->beginResponseStream("application/json");
//         DynamicJsonDocument json(1024);
//         json["message"] = "PONG";
//         json["status"] = "ok";
//         json["ip"] = WiFi.softAPIP().toString();
//         serializeJson(json, *response);
//         request->send(response);
//     });
// }