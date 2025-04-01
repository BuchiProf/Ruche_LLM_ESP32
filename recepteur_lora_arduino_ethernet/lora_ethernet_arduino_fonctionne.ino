
#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <LoRa.h>

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10, 129, 230, 3);
IPAddress myDns(10, 129, 253, 222);
// Paramètres Proxy
const char* proxyServer = "10.129.254.254"; // Adresse du proxy
int proxyPort = 3128; // Port du proxy

EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


void setup() {
  Ethernet.init(10);  // Most Arduino Ethernet hardware
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
      
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("LoRa Receiver");
  LoRa.setPins(9, 8, 2); // NSS, RST, DIO0
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
   
  }
   LoRa.setSyncWord(0x42);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  
 // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Paquet reçu !");

    /* read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }*/
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
            Serial.print("tableau de données : ");
            Serial.println(tabData[dataIndex]);
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
                Serial.print("Tension de batterie : ");
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
                Serial.print("Temperature exterieure : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="H="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(4, value);
                Serial.print("Humidite exterieure : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,2)=="P="){
                String value = tabData[i].substring(2);
                ThingSpeak.setField(5, value);
                Serial.print("Pression : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,3)=="Ti="){
                String value = tabData[i].substring(3);
                ThingSpeak.setField(6, value);
                Serial.print("Temperature interieure : ");
                Serial.println(value);
              }
              if(tabData[i].length() > 0 && tabData[i].substring(0,3)=="Hi="){
                String value = tabData[i].substring(3);
                ThingSpeak.setField(7, value);
                Serial.print("Humidite interieure : ");
                Serial.println(value);
              }
        }
    }

  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
 delay(2000);
  
  }
}
