#include <DHT.h>
#include <ESP8266WiFi.h>
#define DHTPIN D1
#define DHTTYPE DHT11

const char* ssid = "virus";
const char* password = "AntoMar304";

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

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

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

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
