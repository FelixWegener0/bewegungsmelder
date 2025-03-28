#include "Arduino.h"
#include <SimpleDHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char *ssid = "";
const char *password = "";
const char *serverIp = "";

WiFiClientSecure client;
String sensorName = "";

uint8_t output_value = 0;
bool motion_detected = false;
byte inputPin = 5;

void sendPostRequest() {
  client.setInsecure();

  HTTPClient http;
  http.begin(client, serverIp);

  http.addHeader("Content-Type", "application/json");

  String postData = "{\"sensorName\":\"" + sensorName + "}";

  Serial.println("Sende POST-Request: " + postData);
  int httpResponseCode = http.POST(postData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.println(http.getString());

  http.end();
}

void setup() {
  pinMode(inputPin, INPUT);

  Serial.begin(115200);
  Serial.println("beginn waiting for 60s in startup");
  delay(60000);
}

void loop() {
  output_value = digitalRead(inputPin);

  if (output_value) {
    Serial.println("Object detected");
    motion_detected = true;
    sendPostRequest();
    delay(10000);
  } else {
    Serial.println("no Object detected");
    motion_detected = false;
  }

}
