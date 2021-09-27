#include "DHT.h"
#include <WiFi.h>
#define DHT11PIN 27
#define pin 15
const char* ssid = "raghava"; 
const char* password = "12345678";

const char* host = "api.thingspeak.com";
String api_key = "USJZMPEGRWCJI8CX";


WiFiServer server(80);

DHT dht(DHT11PIN, DHT22);

void setup()
{
  
  Serial.begin(115200);
  pinMode(pin, OUTPUT);
   //define the baudrate of 9600
  WiFi.mode(WIFI_STA);
  delay(100);
Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.println(".");
}
Serial.println("");
Serial.println("Successfully connected to WiFi.");
Serial.println("IP address of ESP32 is : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server is ready to go");
Serial.println("Beginning to read and upload the values of sensor to thingspeak");
/* Start the DHT11 Sensor */
  dht.begin();
}

void loop()
{
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  send_data_thingspeak(temp,humi);
}

void send_data_thingspeak(float temp, float humi){
   
  Serial.println(".");
  Serial.println("Preparing to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(temp);
    data_to_send += "&field2=";
    data_to_send += String(humi);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  
  
  }
client.stop();
}
