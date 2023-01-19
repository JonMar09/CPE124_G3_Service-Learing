#include <DHT.h> //library for DHT11
#include <ESP8266WiFi.h> //Library for ESP8266
#define DHTPIN D1  //set dht pin to D1
#define DHTTYPE DHT11 // set type of dht

//Define the name and password of the WiFi network.
const char* ssid = "xxxxx"; 
const char* password = "xxxxx"; 

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);
//Initial set-up of the Arduino
void setup() {
  Serial.begin(921600);
  dht.begin(); //initialize the DHT sensor

  WiFi.begin(ssid, password); // This part will connect the device to the internet using the wifi module in the NODEMCU
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin(); // start the server
  Serial.println("Server started");
  // Print the IP address of the device
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//Checks the client server
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Read the temperature and humidity from the sensor
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  //HTML Output of the web server
  String html = "<html><body>";
  html += "<h1>Temperature: " + String(t) + "C</h1>"; //Displays "Temperature:" in web server
  html += "<h1>Humidity: " + String(h) + "%</h1>"; //Displays "Humidity:" in web server
  html += "</body></html>";
  
  client.println("HTTP/1.1 200 OK"); // Means server connected successfully
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);

  delay(1000); // delay output for 1ms
  client.stop();
}
