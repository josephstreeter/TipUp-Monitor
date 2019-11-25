#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <thread>
#include <chrono>

#ifndef STASSID
#define STASSID "Crusader21-2.4"
#define STAPSK  "Oak_Shore_Dr"
#endif

WiFiClient espClient;
PubSubClient client(espClient); // Initializes the espClient.

// ##### BEGIN VARIABLES #####

const char* ssid     = STASSID;
const char* password = STAPSK;
const uint16_t port = 17;
const int button = 0;
String clientId = "Tip-up-01";
int s;
const char* mqtt_server = "test.mosquitto.org";     //MQTT broker address

// ##### END VARIABLES #####



// ##### BEGIN FUNCTIONS #####

int getStatus()  //This function will check the status of the switch
{
  auto buttonState = digitalRead( button );
  if ( buttonState == LOW ) 
  {
    return 1;
    delay( 500 );   // This prevents the button state from being read again for half a second.
  }
  else
  {
    return 0;
  }
}

int sendMessage(char* status) //This function will send msg to MQTT broker
{
  Serial.println();
  Serial.println(status);
  client.publish("tipups/tipup01", status);
 
  return 0;
}

void reconnect() 
{
  client.setServer(mqtt_server, 1883);
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    //String clientId = "ESP8266Client-";
    //clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("tipups/tipup01", clientId.c_str());
      // ... and resubscribe
      client.subscribe("tipups/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void connectWifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void configPins()
{
   pinMode(button, INPUT_PULLUP);
}

void configSerial()
{
  Serial.begin(115200);
}
// ##### END FUNCTIONS #####

void setup() 
{
  configSerial();
  configPins();
  connectWifi();
}

// ##### BEGIN PROGRAM #####
void loop() 
{
  if (!client.connected()) //Connect to MQTT Broker
  {
    reconnect();
  }
  client.loop();

  //Check flag status
  int x = getStatus();
  char * msg;
  if (x == 0 && s != 0) //Is the status 0 and was the last run 0?
  {
    s = 0;
    msg = "Tip-up 1 No Flag";
    sendMessage(msg);
  }
  else if (x == 1 && s != 1) //Is the status 1 and was the last run 1?
  {
    s = 1;
    msg = " Flag";
    sendMessage(msg);
  }
  
  delay(5);
}
