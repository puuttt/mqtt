#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char *ssid = "Barulah Kamar";
const char *password = "gantipassword";
const char *mqttServer = "192.168.2.100";
const char *mqttTopic = "LAMP";



void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Pesan yang di terima : ");
  Serial.println(topic);
  String message = "";
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.print("Message received: ");
  Serial.println(message);

  if (String(topic) == mqttTopic)
  {
    if (message == "on")
    {
      digitalWrite(LED_BUILTIN, LOW); // Nyalakan LED (LED_BUILTIN pada NodeMCU aktif saat LOW)
    }
    else if (message == "off")
    {
      digitalWrite(LED_BUILTIN, HIGH); // Matikan LED (LED_BUILTIN pada NodeMCU aktif saat LOW)
    }
  }
}

void setup_wifi()
{
  // delay(10);
  Serial.println();
  Serial.print("Mencoba terhubung ke: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    // delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCU_Client"))
    {
      Serial.println("connected");
      client.subscribe(mqttTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}