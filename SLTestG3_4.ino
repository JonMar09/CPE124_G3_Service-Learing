//Description: fourth test of the code, using additional features for the project, like the
//buzzer and LED to notify user that the temperature reached 30.50C.

#include <ESP8266WiFi.h>
#include <Servo.h> // ;ibrary for servo
#include "DHT.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define SECRET_SSID "virus" //input SSID
#define SECRET_PASS "AntoMar304" // input SSID password

#define SECRET_CH_ID 2009202
#define SECRET_WRITE_APIKEY "M7A8RZXUZJZLDQMQ"

#define RED_PIN D5
#define GREEN_PIN D6
#define BLUE_PIN D7

char ssid[] = "virus";   // your network SSID (name) 
char pass[] = "AntoMar304";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

WiFiClient  client;
Servo servo;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

const int servoPin = D0; //assign servo pin to D0
const int buzzerpin = D2;// assign buzzer pin to D2

//Set DHT11
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(921600);  // Initialize serial  
  WiFi.mode(WIFI_STA); 

  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  servo.attach(servoPin);

  pinMode(buzzerpin, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("\nConnecting to ");
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

//Set RGB
  if (t > 31){ //set Red LED
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    // Turn the buzzer on
    digitalWrite(buzzerpin, HIGH); 
    Serial.println("Temperature exceeded 31C, Buzzer on");
    delay(3000); //5 seconds delay
    digitalWrite(buzzerpin, LOW); // Turn the buzzer off
    Serial.println("Buzzer off after 3 seconds");
  } else if (t <= 31) { //set Green LED
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  } else { //set Blue LED
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);    
  }

  ThingSpeak.setField(1, t); // Temperature data placed in Field 1 Chart
  ThingSpeak.setField(2, h);// Humidity data placed in Field 2 Chart

// Set Servo Motor Statements
 int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 if(t > 31){ // If temperature reached 30.50C servo will turn
  for (int i = 0; i <= 180; i++) {  // goes from 0 degrees to 180 degrees
    servo.write(i);              // tell servo to go to position in variable 'i'
    delay(15);                     // waits 15ms for the servo to reach the position
  }
  for (int i = 180; i >= 0; i--) {  // goes from 180 degrees to 0 degrees
    servo.write(i);              // tell servo to go to position in variable 'i'
    delay(15);                     // waits 15ms for the servo to reach the position
  }

// Output for channel update
  if(x == 200){
    Serial.println("Channel update successful.");
    Serial.print(F("Humidity: "));Serial.print(h);Serial.print(F("%  Temperature: "));Serial.print(t);Serial.print(F("°C "));
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(2000);// Wait 5 seconds to update the channel again
 }
} 
