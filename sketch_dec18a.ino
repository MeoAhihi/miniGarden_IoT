#include <WiFi.h>
#include "WebSocketsClient.h"

#define LightPin 14
#define FanPin 27
#define PumpPin 16

bool light = false;
bool fan = false;
bool pump = false;

const char* ssid = "vivo Y33s";
const char* password = "moeahihi";
const char* webSocketServer = "192.168.107.1";
const int webSocketPort = 3000;  // Adjust the port as needed

WebSocketsClient webSocket;

void setup() {
  pinMode(LightPin, OUTPUT);
  pinMode(FanPin, OUTPUT);
  pinMode(PumpPin, OUTPUT);


  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");

  // Setup WebSocket event handler
  webSocket.begin(webSocketServer, webSocketPort);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Your main code goes here
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket connected");
      break;
    case WStype_TEXT:
      String signal = String((char*)payload);   
      Serial.println("Received message: " + signal);
      Control(signal[0]);
      Serial.printf("Light: %d, Fan: %d, Pump: %d.\n", light, fan, pump);
      break;
  }
}

void Control(char input) {
  switch (input) {
    case '0':
      toggleLight();
      break;
    case '1':
      toggleFan();
      break;
    case '2':
      togglePump();
      break;
    default:
      break;
  }
}

void toggleLight() {
  light = !light;
  digitalWrite(LightPin, light);
}

void toggleFan() {
  fan = !fan;
  digitalWrite(FanPin, fan);
}

void togglePump() {
  pump = !pump;
  digitalWrite(PumpPin, pump);
}