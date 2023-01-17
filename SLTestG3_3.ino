#include <ESP8266WiFi.h>
#include <Servo.h>
#include "DHT.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define SECRET_SSID "virus"
#define SECRET_PASS "AntoMar304"

#define SECRET_CH_ID 2009202
#define SECRET_WRITE_APIKEY "M7A8RZXUZJZLDQMQ"
char ssid[] = "virus";   // your network SSID (name) 
char pass[] = "AntoMar304";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
Servo servo;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const int servoPin = D0;
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(921600);  // Initialize serial  
  WiFi.mode(WIFI_STA); 
  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  servo.attach(servoPin);
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  ThingSpeak.setField(1, t); // Temperature data placed in Field 1 Chart
  ThingSpeak.setField(2, h);// Humidity data placed in Field 2 Chart

 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 if(t > 30.50){ // If temperature reached 30.50C servo will turn
  for (int i = 0; i <= 180; i++) {  // goes from 0 degrees to 180 degrees
    servo.write(i);              // tell servo to go to position in variable 'i'
    delay(15);                     // waits 15ms for the servo to reach the position
  }
  for (int i = 180; i >= 0; i--) {  // goes from 180 degrees to 0 degrees
    servo.write(i);              // tell servo to go to position in variable 'i'
    delay(15);                     // waits 15ms for the servo to reach the position
  }

  if(x == 200){
    Serial.println("Channel update successful.");
    Serial.print(F("Humidity: "));Serial.print(h);Serial.print(F("%  Temperature: "));Serial.print(t);Serial.print(F("°C "));
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(5000);
 }
} // Wait 20 seconds to update the channel again