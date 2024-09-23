#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RELAY_PIN_1 5 // Pin connected to the first relay module
#define RELAY_PIN_2 4 // Pin connected to the second relay module

const char* ssid = "jazib"; 
const char* password = "jazib1234"; 
const char* mqtt_server = "broker.hivemq.com"; // MQTT broker 

WiFiClient espClient; 
PubSubClient client(espClient); 

void setup() { 
  Serial.begin(115200); 
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); 
} 

void setup_wifi() { 
  delay(10); 
  Serial.println(); 
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
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

void callback(char* topic, byte* payload, unsigned int length) { 
  String string; 
  Serial.print("Message arrived ["); 
  Serial.print(topic); 
  Serial.print("] "); 
  for (int i = 0; i < length; i++) { 
    string+=((char)payload[i]); 
  } 
  Serial.println(string); 

  if (strcmp(topic, "/ThinkIOT/Servo-nodered/pump1") == 0) { // Check the topic for pump1
    int status = string.toInt(); 
    digitalWrite(RELAY_PIN_1, status);
  } else if (strcmp(topic, "/ThinkIOT/Servo-nodered/pump2") == 0) { // Check the topic for pump2
    int status = string.toInt(); 
    digitalWrite(RELAY_PIN_2, status);
  }
}

void reconnect() { 
  while (!client.connected()) { 
    Serial.print("Attempting MQTT connection..."); 
    if (client.connect("ESPClient")) { 
      Serial.println("connected"); 
      client.subscribe("/ThinkIOT/Servo-nodered/pump1"); 
      client.subscribe("/ThinkIOT/Servo-nodered/pump2"); 
    } else { 
      Serial.print("failed, rc="); 
      Serial.print(client.state()); 
      Serial.println(" try again in 5 seconds"); 
      delay(5000); 
    }
  }
} 

void loop() { 
  if (!client.connected()) { 
    reconnect(); 
  } 
  client.loop(); 
  delay(100);
}
