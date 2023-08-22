#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Create an Event Source on /events
AsyncEventSource events("/events");
