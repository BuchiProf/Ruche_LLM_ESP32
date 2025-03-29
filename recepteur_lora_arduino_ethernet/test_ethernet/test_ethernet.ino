#include <SPI.h>
#include <Ethernet.h>

// Paramètres Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetClient client;

// Paramètres du serveur de test
const char* server = "www.example.com"; // Vous pouvez utiliser n'importe quel serveur web

// Paramètres Proxy
const char* proxyServer = "votre.proxy.server"; // Adresse du proxy
int proxyPort = 8080; // Port du proxy

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

  // Essayer de se connecter au proxy
  if (client.connect(proxyServer, proxyPort)) {
    Serial.println("Connexion réussie au proxy");
    // Envoyer une requête HTTP GET via le proxy
    client.println("GET http://www.example.com/ HTTP/1.1");
    client.println("Host: www.example.com");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Échec de la connexion au proxy");
  }
}

void loop() {
  // Lire les données du serveur et les afficher dans le moniteur série
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Si le serveur a fermé la connexion, arrêter le client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Déconnexion du serveur");
    client.stop();
    // Attendre avant de réessayer
    delay(5000);
  }
}