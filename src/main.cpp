#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "InfoConnexion.h"
#include "ESP8266WebServer.h"
#include "milligram_min_css.h"
#include "index_html.h"
#include "ESP8266HTTPClient.h"
#include "ArduinoOTA.h"

// Définition des entrées/sorties
#define PIN_LED_ROUGE 5
#define PIN_LED_VERTE 4

// Page html -> Déplacée dans index_html.h pour plus de lisibilité.
//const char index_html[] PROGMEM = R"=====()=====";

// Informations de connexion : cachées dans fichier InfoConnexion.h
// Vous pouvez décommenter ici ou créer comme moi un fichier InfoConnexion.h
//const char * SSID = "A définir";
//const char * PASSWORD = "A définir";

// Objet WebServer  
ESP8266WebServer serverWeb(80);

// Gestion des événements du WiFi
// Lorsque la connexion vient d'aboutir
void onConnected(const WiFiEventStationModeConnected& event);
// Lorsque l'adresse IP est attribuée
void onGotIP(const WiFiEventStationModeGotIP& event);

// Fonctions du serverWeb
void switchLedOn() {
  digitalWrite(PIN_LED_VERTE, HIGH);
  serverWeb.send(200, "text/plain", "1");
}

void switchLedOff() {
  digitalWrite(PIN_LED_VERTE, LOW);
  serverWeb.send(200, "text/plain", "0");
}

void handleRoot() {
  String temp(reinterpret_cast<const __FlashStringHelper *>(index_html));
  if (digitalRead(PIN_LED_VERTE) == HIGH) temp.replace("%LED%", "allumée"); else temp.replace("%LED%", "éteinte");
  serverWeb.send(200, "text/html", temp);
}

void setup() {
  IPAddress ip(192,168,0,201);
  IPAddress dns(192,168,0,1);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);

  // Mise en place d'une liaison série
  Serial.begin(9600L);
  delay(100);

  // Configuration des entrées/sorties
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);

  // Définition du nom de l'objet sur le réseau
  WiFi.hostname("ObjetLED");

  // Mode de connexion
  WiFi.mode(WIFI_STA);

  // Mode point d'accès
  WiFi.softAP("Objet LED");
 
  // Démarrage de la connexion
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(SSID, PASSWORD);

  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);

  // Mise en place du serveur WebServer
  serverWeb.on("/switchLedOn", switchLedOn);
  serverWeb.on("/switchLedOff", switchLedOff);
  serverWeb.on("/", handleRoot);
  serverWeb.on("/index.html", handleRoot);
  serverWeb.on("/rssi", [] {serverWeb.send(200, "text/plain", String(WiFi.RSSI())); });
  serverWeb.on("/milligram.min.css", [] {
    serverWeb.send(200, "text/css", MILLIGRAM_MIN_CSS);
  });

  serverWeb.begin();
}

void loop() {
  // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
  if (WiFi.isConnected()) {
    digitalWrite(PIN_LED_ROUGE, HIGH);
    ArduinoOTA.begin();
  }
  else {
    digitalWrite(PIN_LED_ROUGE, LOW);
  }
  serverWeb.handleClient();
  ArduinoOTA.handle();
}

void onConnected(const WiFiEventStationModeConnected& event) {
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
}

void onGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
  Serial.println("Passerelle IP : " + WiFi.gatewayIP().toString());
  Serial.println("DNS IP : " + WiFi.dnsIP().toString());
  Serial.print("Puissance de réception : ");
  Serial.println(WiFi.RSSI());
}
