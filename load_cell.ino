#include "HX711.h"
#include <SoftwareSerial.h>
#define DOUT  3
#define CLK  2
HX711 scale;
float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
SoftwareSerial blue(0,1);
void setup() {
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  blue.begin(9600);
}

void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  blue.print("1234");
  if (scale.get_units()>0){
   blue.print(3.564*scale.get_units(), 1);
  }// delay(10000);
 
}
