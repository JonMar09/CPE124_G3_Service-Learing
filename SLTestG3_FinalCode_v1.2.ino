/*
Code Update: Server displays the temp in webserver and servo button working.
*/
//Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Servo.h>

// Network Credentials
const char* ssid = "virus"; //Name of Netwoork
const char* password = "AntoMar304"; // Network Password

//DHT Sensor pins
#define DHTPIN D1 // Dht pin assigned in D1
#define DHTTYPE DHT11 // Type of temp sensor    
DHT dht(DHTPIN, DHTTYPE);

// RGB pin Setup
#define RED_PIN D5
#define GREEN_PIN D6
#define BLUE_PIN D7

float t = 0.0;
float h = 0.0;

AsyncWebServer server(80);
Servo servo;

const int servoPin = D0; //assign servo pin to D0
const int buzzerpin = D2;// assign buzzer pin to D2

unsigned long previousMillis = 0;
const long interval = 10000;  

//HTML Body
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .button {
  border: none;
  color: white;
  padding: 16px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  transition-duration: 0.4s;
  cursor: pointer;
}

.button1 { <!--On Valve Label-->
  background-color: white;
  color: black;
  border: 2px solid #4CAF50;
}

.button1:hover { <!--On Valve label-->
  background-color: #4CAF50;
  color: white;
}

.button2 { <!--Reset-->
  background-color: white;
  color: black;
  border: 2px solid #008CBA;
}

.button2:hover { <!--Reset -->
  background-color: #008CBA;
  color: white;
}
  </style>
</head> <!--End of head tag -->



<body> <!-- Webpage design -->
  <h2>TempApp</h2> <!--Title of webserver-->
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
  <button class="button button1">Start</button>
  <button class="button button2">Reset </button>

</body> <!--End of body Tag -->

<script> <!--for java program-->
setInterval(function ( ) { <!--For Temperature -->
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) { <!--For Humidity -->
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script> 
</html>)rawliteral";
//End of HTML Body

String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup(){
  Serial.begin(921600);
  dht.begin();
  WiFi.begin(ssid, password);

  servo.attach(servoPin);

  pinMode(buzzerpin, OUTPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT); 

  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nServer Active: ");
  Serial.println(WiFi.localIP()); // Prints the IP Address of ESP8266 

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
    }
    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.println(h);
    }


    if (t > 31){ //set Red LED
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    // Turn the buzzer on
    digitalWrite(buzzerpin, HIGH); 
    Serial.println("Temperature exceeded 31C, Buzzer on");
    delay(800); //0.8 seconds delay
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
  if(t > 31){ // If temperature reached 31C servo will turn//
  for (int i = 0; i <= 180; i++) {  // goes from 0 degrees to 180 degrees//
    servo.write(i);              // tell servo to go to position in variable 'i'//
    delay(15);                     // waits 15ms for the servo to reach the position//
  }
  for (int i = 180; i >= 0; i--) {  // goes from 180 degrees to 0 degrees//
    servo.write(i);              // tell servo to go to position in variable 'i'//
    delay(15);                     // waits 15ms for the servo to reach the position//
  }

  }
 }
}