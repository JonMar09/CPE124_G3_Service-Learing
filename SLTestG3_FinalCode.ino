#include <DHT.h>
#include <ESP8266WiFi.h>
#define DHTPIN D1
#define DHTTYPE DHT11

//User SSID and PASSWORD
const char* ssid = "*****";
const char* password = "*****";

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

//Function for setup in Wifi
void setup() {
  Serial.begin(921600);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
  Serial.println("Server started");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//Function for WiFi Server
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Display value of the Humidity and Temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  //Headers
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
