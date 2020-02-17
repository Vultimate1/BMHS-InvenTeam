#include <SPI.h>
#include <Ethernet.h>
#include "HX711.h"

#define DOUT A4
#define CLK A2
// ETHERNET config.
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
//byte mac[] = { 0x28, 0x16, 0xA8, 0x6F, 0x52, 0x0D };
byte mac[] = {0x6C, 0x2B, 0x59, 0xF0, 0x20, 0x41};
// Set the static IP address to use if the DHCP fails to assign
byte ip[]={192,168,1,166}; // example IPAddress ip(192, 168, 1, 100);
byte gateway[] = {192,168,1,1};
byte subnet[] = {255,255,255,0};
const int trigPin = 12;
const int echoPin = 8;
float calibration_factor = -8000;
HX711 scale;
// ASKSENSORS config.     
char server[] = "asksensors.com";                               // ASKSENSORS host name
const int port = 80;                                      // Port: HTTP=80
const char* apiKeyIn = "76RLSNCr2WHA48aU7VkKMpO1IFasyiqT";      // API KEY IN (change it with your API KEY IN)
// dummy data
//int dumData = 100;    // set your data
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

double ultrasonic()
{
  long duration;
  double distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (((duration*0.034)/2)/2.54);
  double height=48-distance*3;
  Serial.print("distance=");
  Serial.println(distance);
  return distance;
}

double weightx()
{
  double weight;
  scale.set_scale(calibration_factor);
  weight=scale.get_units(1)+0.08;
  double calibrated_weight=(weight-0.25)*375/408;
  Serial.print("weight=");
  Serial.println(calibrated_weight);
  if (calibrated_weight<0){
    return abs(calibrated_weight);
  }
  return calibrated_weight;
}

void setup() {
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  scale.begin(DOUT,CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
 // long zero_factor = scale.read_average(); //Get a baseline reading
 // Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 // Serial.println(zero_factor);
  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  }
   
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  Serial.println("Ethernet connected");
  delay(1000);
  
    Serial.println("> Request sent to ASKSENSORS");
   
}

void loop() {
  
  //scale.set_scale(calibration_factor);
  
  // if there are incoming bytes available
  // from the server, read them and print them:
   //Adjust to this calibration factor
  while (client.connect(server, port)) {
   // setup();
    int height = ultrasonic();
    double load = weightx();
    // if you get a connection, report back via serial:

    Serial.print("connected asksensors.com");
      //Create a URL for the request
      String url = "http://api.asksensors.com/write/";
      url += apiKeyIn;
      url += "?module1=";
      url += height;
      Serial.print("********** requesting URL: ");
      Serial.println(url);
     //Make a HTTP request:
      client.print(String("GET ") + url+ " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n");
   client.println();
      delay(1000); 
      String url2 = "http://api.asksensors.com/write/";
      url2 += apiKeyIn;
      url2 += "?module2=";
      url2 += load;
      Serial.print("********** requesting URL: ");
      Serial.println(url2);
      client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n");
    client.println();
    delay(1000);
    
  }
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing forevermore:
    while (true);
  }
}
