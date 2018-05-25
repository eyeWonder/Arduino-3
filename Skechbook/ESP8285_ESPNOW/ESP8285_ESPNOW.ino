#include "WifiEspNow.h"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

//static const int LED_PIN = 2;    // ESP-12F blue LED
//int ledState = HIGH;
static uint8_t PEER[] {0x5E, 0xCF, 0x7F, 0x90, 0xFA, 0xE8};

void printReceivedMessage(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg) {
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  for (int i = 0; i < count; ++i) {
    Serial.print(static_cast<char>(buf[i]));
  }
//  digitalWrite(LED_PIN, ledState);
//  ledState = 1 - ledState;
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println();

//  pinMode(LED_PIN, OUTPUT);
//  digitalWrite(LED_PIN, ledState);

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
//  WiFi.softAP("ESPNOW", nullptr, 3);
//  WiFi.softAPdisconnect(false);
  String Prefix = "ESPNOW:";
  String Mac = WiFi.softAPmacAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), 3);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }

  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }

}

void loop() {
  delay(1000);
}
