#include <DHT.h>
#include <ESP8266WiFi.h>
#define DHTPIN D1  
#define DHTTYPE DHT11

//Define the name and password of the WiFi network.
const char* ssid = "virus"; 
const char* password = "AntoMar304"; 

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

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

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Read the temperature and humidity from the sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String html = "<html><body>";
  html += "<h1>Temperature: " + String(t) + "C</h1>";
  html += "<h1>Humidity: " + String(h) + "%</h1>";
  html += "</body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);

  delay(1000);
  client.stop();
}
