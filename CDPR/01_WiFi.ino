//#include <WiFi.h>
//
//// Replace with your network credentials
//const char* ssid = "CDPR";
//const char* password = "12345678";
//
//// Initialize WiFi
//void initWiFi() {
//  WiFi.mode(WIFI_AP);
//  // Remove the password parameter, if you want the AP (Access Point) to be open
//  WiFi.softAP(ssid, password);
//
//
//  IPAddress IP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(IP);
//
////  // Register a callback for client disconnect event
////  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_AP_STACONNECTED);
////  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_AP_STADISCONNECTED);
//
//}
