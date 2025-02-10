/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Modified from the examples of the Arduino LoRa library
  More resources: https://RandomNerdTutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
*********/
#include <WiFi.h>
#include "secrets.h"
#include <ThingSpeak.h>
#include <SPI.h>
#include <LoRa.h>

// Remplacez par vos informations réseau
const char* ssid = SECRET_SSID;  
const char* password = SECRET_PASS; 
//thingspeak
const char* writeAPIKey = SECRET_WRITE_APIKEY;
const long channelID = SECRET_CH_ID;

WiFiClient espClient;



//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  //démarrage wifi
  setup_wifi();

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //868E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0x42) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0x42);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Paquet reçu !");

    // lire le paquet au format "BME280: T=22.5,H=68,P=989"
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData);
      
      // Tableau pour stocker les sous-chaînes
      String tabData[10]; // Ajustez la taille selon vos besoins
      int dataIndex = 0;

      // Variables pour stocker les positions
      int startIndex = 0;
      int endIndex = 0;

      // Boucle pour extraire les sous-chaînes
      while ((endIndex = LoRaData.indexOf(',', startIndex)) != -1) {
            tabData[dataIndex] = LoRaData.substring(startIndex, endIndex);
            //Serial.println(tabData[dataIndex]);
            dataIndex++;
            startIndex = endIndex + 1;
            }
  
        // Pour le dernier élément après la dernière virgule
        tabData[dataIndex] = LoRaData.substring(startIndex);
        //Serial.println(tabData[dataIndex]);


         for (int i = 0; i <= dataIndex; i++) {
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="B="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(1, value);
                Serial.print("Pourcentage de batterie : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="M="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(2, value);
                Serial.print("Masse : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="T="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(3, value);
                Serial.print("Temperature : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="H="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(4, value);
                Serial.print("Humidite : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="P="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(5, value);
                Serial.print("Pression : ");
                Serial.println(value);
              }
        }
     
                
    }
/*
    // print RSSI of packet
    Serial.print("avec un RSSI de ");
    Serial.print(LoRa.packetRssi());
    Serial.println(" dB");
    */

  //thingspeak

  int x = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (x == 200) {
    Serial.println("Mise à jour réussie.");
    Serial.println("##########FIN BOUCLE##########");
  } else {
    Serial.println("Erreur de mise à jour. Code HTTP: " + String(x));
    
  }
  
  delay(2000);
  



  }



}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP : ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(espClient);
}


