/*
Programme de calibration du HX711 avec un ESP32 (même programme que Arduino)
Le choix des broches numériques diffère

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 ESP32 
 D4 ->  HX711 CLK
 D12 -> DOUT
 3.3V -> VCC
 GND -> GND

 
*/

#include "HX711.h" 

#define LOADCELL_DOUT_PIN  12
#define LOADCELL_SCK_PIN  4

HX711 scale;

float calibration_factor = -450; //-450 pour cellule 5kg

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Lecture valeur à diviser : ");
  Serial.print(scale.get_units(10));
  Serial.println(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
//Serial.print("lecture moyenne moins tare : ");
//Serial.println(scale.get_value(5));
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
  delay(2000);
}
