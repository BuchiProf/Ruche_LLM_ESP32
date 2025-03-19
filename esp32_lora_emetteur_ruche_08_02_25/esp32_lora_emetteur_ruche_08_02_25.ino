/*********
  Sources : https://RandomNerdTutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
            https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/#more-84742
  Adaptation pour :
  - mesure de température, humidité et pression atmo avec BME280,
  - envoi des données par LoRa via rfm95 en 886.6MHz,
  - mesure de masse avec HX711,
  - mesure tension batterie lipo
  - gestion de l'énergie avec un passage en deepsleep.
  Buchi S 02-2025
*********/
//Préparation deepsleep
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  900          /* Time ESP32 will go to sleep (in seconds) 1800s for 30 min*/

#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include "HX711.h" //https://github.com/bogde/HX711
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

// BME280 I2C
Adafruit_BME280 bme;
float temp;
float hum;
float pres;
// hx711
#define LOADCELL_DOUT_PIN  12
#define LOADCELL_SCK_PIN  4
HX711 scale;
float calibration_factor = -450; //-450 pour cellule 5kg
//#define calibration_factor 22983 //valeur obtenue expérimentalement pese personne lidl

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

//lecture du pont diviseur de tension (charge batterie 4.2V) sur port analogique 33
#define batterie 33
float tension;

/*******************
*****SETUP**********
********************/
void setup() {
 
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Affichage des données envoyées par LoRa");
  
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0x42) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0x42);
  Serial.println("LoRa Initializing OK!");

   // Initialize BME280 sensor 
  if (!bme.begin(0x76)) {
    Serial.println("Capteur BME280 non trouvé, vérifier cablage!");
    while (1);
  }
  /*configuration de la source de réveil :
  avec un timer pour se réveiller toutes les minutes (60 secondes)
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

// lecture batterie et renvoi de la valeur de la tension sur 12bits
// lecture batterie : map fonctionne avec des entiers
tension = analogRead(batterie) * (4.2/4095.0);
String charge = "B=" + String(tension);



// configuration et envoi de la masse
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  String masse = "M=" + String(scale.get_units(10)+252.0);
  


// config et envoi BME280
  temp = bme.readTemperature();
  hum = bme.readHumidity();
  pres = bme.readPressure()/100.0F;
  //concaténation des valeurs et envoi sour forme de texte
  String bme = "T=" + String(temp) + ",H=" + String(hum) + ",P=" + String(pres);

//on envoie tout dans un paquet de la forme "B=42,M=420,T=24,H=42,P=420"
String paquet = charge + "," + masse + "," + bme;
envoiLoRa(paquet);
    
/*on démarre le deepsleep*/
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

/***********************************
*************LOOP*******************
**********************************/
void loop() {
  // on ne rentre jamais dans la bouble loop
}


/*************************************************************
///////////////////LES FONCTIONS////////////////////////////
****************************************************************/



void envoiLoRa(String valeur){
  //une fonction qui envoi une chaine de caractère par LoRa
    LoRa.beginPacket();
    LoRa.print(valeur);
    LoRa.endPacket();
    Serial.println(valeur);
    delay(1000);
}
