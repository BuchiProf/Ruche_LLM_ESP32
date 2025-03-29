#include <SPI.h>
#include <Ethernet.h>
#include <RH_RF95.h>

// Paramètres Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetClient client;

// Paramètres ThingSpeak
const char* server = "api.thingspeak.com";
String writeAPIKey = "VOTRE_API_KEY";
const int updateThingSpeakInterval = 20 * 1000; // Intervalle de mise à jour en millisecondes

// Paramètres Proxy
const char* proxyServer = "votre.proxy.server"; // Adresse du proxy
int proxyPort = 8080; // Port du proxy

// Paramètres LoRa
#define RFM95_CS 9
#define RFM95_RST 8
#define RFM95_INT 2
#define RF95_FREQ 868.0 // Fréquence LoRa pour l'Europe

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendre que le port série soit prêt
  }

  // Initialisation Ethernet
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Échec de la configuration Ethernet avec DHCP");
    // Essayez une configuration manuelle
    Ethernet.begin(mac, ip);
  }
  delay(1000); // Attendre que l'Ethernet Shield soit prêt
  Serial.println("Ethernet initialisé");

  // Initialisation LoRa
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("Initialisation LoRa échouée");
    while (1);
  }
  Serial.println("LoRa initialisé");
  rf95.setFrequency(RF95_FREQ);

  // Définir le syncword
  rf95.spiWrite(RH_RF95_REG_39_SYNC_WORD, 0x42);
}

void loop() {
  // Vérifier les connexions Ethernet
  if (client.connect(proxyServer, proxyPort)) {
    Serial.println("Connecté au proxy");

    // Lire les données de température et d'humidité du module LoRa
    float temperature = lireTemperature();
    float humidite = lireHumidite();

    // Créer la chaîne de données à envoyer
    String postData = "api_key=" + writeAPIKey + "&field1=" + String(temperature) + "&field2=" + String(humidite);

    // Envoyer les données à ThingSpeak via le proxy
    client.print("POST http://api.thingspeak.com/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: " + String(postData.length()) + "\n\n");
    client.print(postData);

    // Attendre la réponse du serveur
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println("Déconnecté de ThingSpeak");
  } else {
    Serial.println("Échec de connexion au proxy");
  }

  // Vérifier les messages LoRa
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      Serial.print("Message LoRa reçu : ");
      Serial.println((char*)buf);

      // Traiter les données reçues (par exemple, extraire température et humidité)
      float temperature = extraireTemperature(buf, len);
      float humidite = extraireHumidite(buf, len);

      // Envoyer les données à ThingSpeak
      envoyerAThingSpeak(temperature, humidite);
    }
  }

  // Attendre avant la prochaine mise à jour
  delay(updateThingSpeakInterval);
}

float lireTemperature() {
  // Remplacez cette fonction par le code pour lire la température du module LoRa
  return 25.0; // Exemple de valeur
}

float lireHumidite() {
  // Remplacez cette fonction par le code pour lire l'humidité du module LoRa
  return 60.0; // Exemple de valeur
}

float extraireTemperature(uint8_t* buf, uint8_t len) {
  // Convertir le buffer en chaîne de caractères d'une chaîne de caractères au format "T=26,H=85"
  String data = String((char*)buf);

  // Trouver la position de "T="
  int startIndex = data.indexOf("T=") + 2;
  int endIndex = data.indexOf(",", startIndex);

  // Extraire la sous-chaîne contenant la température
  String tempString = data.substring(startIndex, endIndex);

  // Convertir la sous-chaîne en float
  float temperature = tempString.toFloat();

  return temperature;
}

float extraireHumidite(uint8_t* buf, uint8_t len) {
  // Convertir le buffer en chaîne de caractères d'une chaîne de caractères au format "T=26,H=85"
  String data = String((char*)buf);

  // Trouver la position de "H="
  int startIndex = data.indexOf("H=") + 2;
  int endIndex = data.indexOf(",", startIndex);
  if (endIndex == -1) {
    endIndex = data.length();
  }

  // Extraire la sous-chaîne contenant l'humidité
  String humidString = data.substring(startIndex, endIndex);

  // Convertir la sous-chaîne en float
  float humidite = humidString.toFloat();

  return humidite;
}

void envoyerAThingSpeak(float temperature, float humidite) {
  if (client.connect(proxyServer, proxyPort)) {
    String postData = "api_key=" + writeAPIKey + "&field1=" + String(temperature) + "&field2=" + String(humidite);

    client.print("POST http://api.thingspeak.com/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: " + String(postData.length()) + "\n\n");
    client.print(postData);

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println("Déconnecté de ThingSpeak");
  } else {
    Serial.println("Échec de connexion au proxy");
  }
}