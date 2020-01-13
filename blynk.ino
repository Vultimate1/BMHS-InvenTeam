#define BLYNK_PRINT Serial

//#include <BlynkSimpleEsp8266.h>

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(0, 1 ); // RX, TX
#include <BlynkSimpleStream.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "TSVv2HEe1PiTiks3zw23Sz1GepxBfN06";
const int pot=A0;
BlynkTimer timer;
SoftwareSerial blue(0,1);
void setup()
{
  // Debug console
  DebugSerial.begin(9600);
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  blue.begin(9600);
  Blynk.begin(Serial, auth);
  pinMode(pot, OUTPUT);
  timer.setInterval(1000, V0);
}
void timerEvent()
{
  float v0=analogRead(pot);
  Blynk.virtualWrite(V0,v0);
}
void loop()
{
  Blynk.run();
  timer.run();
}