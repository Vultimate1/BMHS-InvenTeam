#include <SPI.h>
#include <Ethernet.h>
#include <HX711.h>

// ETHERNET config.
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x28, 0x16, 0xA8, 0x6F, 0x52, 0x0D };
//byte mac[] = {0x6C, 0x2B, 0x59, 0xF0, 0x20, 0x41};
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,153); // example IPAddress ip(192, 168, 1, 100);
const int trigPin = 12;
const int echoPin = 8;
float calibration_factor = -7050;
HX711 scale;
// ASKSENSORS config.     
char server[] = "asksensors.com";                               // ASKSENSORS host name
const int port = 80;                                      // Port: HTTP=80
const char* apiKeyIn = "QKiqxH349WriTwQ09YW1jmBeA4HLryzF";      // API KEY IN (change it with your API KEY IN)
const
// dummy data
//int dumData = 100;    // set your data
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

double ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  double duration = pulseIn(echoPin, HIGH);
  double distancea = ((duration*0.034/2)*cos(30));
  double distance=distancea*24;
  return distance;
}
double load_measure()
{
 
}
void setup() {
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  scale.begin(3, 2);
  scale.set_scale(calibration_factor);
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  //Serial.println(zero_factor);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
    // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.print("connected asksensors.com");
      //Create a URL for the request
      String url = "http://asksensors.com/api.asksensors/write/";
      url += apiKeyIn;
      url += "?module2=";
      url += scale.get_units();
      Serial.print("********** requesting URL: ");
      Serial.println(url);
     //Make a HTTP request:
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n");
      //url = "?module2=";
      //client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      //         "Host: " + server + "\r\n" +
      //         "Connection: close\r\n\r\n");
    client.println();
    
    Serial.println("> Request sent to ASKSENSORS");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {
  
 // return distance;
  //int distance = ultrasonic();
  // if there are incoming bytes available
  // from the server, read them and print them:
   //Adjust to this calibration factor
  while (client.available()) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    //digitalWrite(trigPin, LOW);
    double duration = pulseIn(echoPin, HIGH);
    double distancea = ((duration*0.034/2)*cos(30));
    double distance=distancea*24;
    double load=scale.get_units();
    Serial.print("distance=");
    Serial.println(distance);
    Serial.print("load=");
    Serial.println(load);
    delay(1000);
    //client.print(scale.get_units());
    //Serial.println(scale.get_units());
    //client.print(distance);
    String url = "http://asksensors.com/api.asksensors/write/";
      url += apiKeyIn;
      url += "?module1=";
      url += distance;
    url = "http://asksensors.com/api.asksensors/write/";
      url += apiKeyIn;
      url += "?module2=";
      url += load;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + server + "\r\n");     
    delay(1000);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + server + "\r\n"); 
        
    //Serial.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + server + "\r\n");
    //delay(1000);
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