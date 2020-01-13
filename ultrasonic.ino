#include <SoftwareSerial.h>
const int trigPin = 7;
const int echoPin = 6;
//SoftwareSerial blue(2,3);
double height()
{
  digitalWrite(trigPin, HIGH);
  double firstval= pulseIn(6, HIGH);
  return firstval;
}
void setup() {
//  blue.begin(9600);
//  Blynk.begin(Serial, auth);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop() {
  int duration=0;
  double distance=0;
  double heightx=0;
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
 
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  duration = pulseIn(6, HIGH);
  double distancea = (((duration*0.034/2)-2.25)*cos(30));
  distance=distancea*24/7.3;
  if (24-distance<0)
  {
    heightx=0;
  } else {
    heightx=24-distance;
  }
 
  Serial.println(heightx);
  delay(10);
//  Blynk.run();
}
