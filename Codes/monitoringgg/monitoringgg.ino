#include <WiFi.h>
#include <DHTesp.h>
#include <PubSubClient.h>

#define SensorPin 35        // pH meter Analog output to Arduino Analog Input 0
#define calibration_offset 0.7  // Calibration offset for pH sensor
#define calibration_value 21.34 // Calibration value for pH sensor (without offset)
#define MQ3PIN 34           // Define the pin for MQ-3 sensor
#define HC_SR04_TRIGGER 14  // HC-SR04 Trigger pin
#define HC_SR04_ECHO 12     // HC-SR04 Echo pin
#define S0 5
#define S1 4
#define S2 2
#define S3 13
#define sensorOut 15
#define DHTPIN 25           // DHT11 data pin
#define DHTTYPE DHTesp::DHT11   // DHT11 sensor type
#define PUMP_PIN 26         // Pump control pin

#define DISTANCE_TOPIC "sensors/distance"
#define RGB_TOPIC "sensors/rgb"
#define COLOR_TOPIC "sensors/color"
#define MQ3_TOPIC "sensors/mq3"
#define TEMPERATURE_TOPIC "sensors/temperature"
#define HUMIDITY_TOPIC "sensors/humidity"
#define PH_TOPIC "sensors/ph"

const char* ssid = "realme 9";
const char* password = "12345678";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_user = "user1";
const char* mqtt_password = "user1";

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

int red = 0;
int green = 0;
int blue = 0;

unsigned long int avgValue;  // Store the average value of the sensor feedback
int buf[10], temp;

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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("MQ-3, HC-SR04, TCS230, and DHT11 Sensor Test");
  setup_wifi();
  pinMode(MQ3PIN, INPUT);
  pinMode(HC_SR04_TRIGGER, OUTPUT);
  pinMode(HC_SR04_ECHO, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);  // Ensure the pump is off initially
  dht.setup(DHTPIN, DHTTYPE);  // Setup DHT sensor

  client.setServer(mqtt_server, mqtt_port);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  Serial.println("Color Detection started:");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int mq3Value = analogRead(MQ3PIN);
  Serial.print("MQ-3 Value: ");
  Serial.println(mq3Value);
  client.publish(MQ3_TOPIC, String(mq3Value).c_str(), true);

  color();
  String rgbString = String(red) + "," + String(green) + "," + String(blue);
  client.publish(RGB_TOPIC, rgbString.c_str(), true);

  Serial.print("RGB Values: R=");
  Serial.print(red);
  Serial.print(" G=");
  Serial.print(green);
  Serial.print(" B=");
  Serial.println(blue);

  String colorDetected = detectColor();
  Serial.print("Detected Color: ");
  Serial.println(colorDetected);
  client.publish(COLOR_TOPIC, colorDetected.c_str(), true);

  float distance = readHC_SR04();
  Serial.print("Distance: ");
  Serial.println(distance);
  client.publish(DISTANCE_TOPIC, String(distance).c_str(), true);

if (colorDetected == "Yellow" && distance < 13) {
    digitalWrite(PUMP_PIN, HIGH); // Turn the pump ON only if color is "Red" and distance is more than 4
} else {
    digitalWrite(PUMP_PIN, LOW); // Turn the pump OFF if distance is 4 or less, or color is not "Red"
}



  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    client.publish(TEMPERATURE_TOPIC, String(temperature).c_str(), true);
    client.publish(HUMIDITY_TOPIC, String(humidity).c_str(), true);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = -1 * (-5.70 * phValue + calibration_value + calibration_offset + 41.31);
  Serial.print("pH Value: ");
  Serial.println(phValue);
  
  client.publish(PH_TOPIC, String(phValue).c_str(), true);

  delay(1000);
}

float readHC_SR04() {
  digitalWrite(HC_SR04_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(HC_SR04_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC_SR04_TRIGGER, LOW);

  unsigned long duration = pulseIn(HC_SR04_ECHO, HIGH);
  float distance = duration * 0.034 / 2;

  if (distance == 0 || distance >= 400) {
    distance = -1; // Consider invalid reading as -1
  }
  return distance;
}

void color() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  red = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  digitalWrite(S3, HIGH);
  blue = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  digitalWrite(S2, HIGH);
  green = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
}

String detectColor() {
  if (red < blue && red < green && red < 25) {
    if (green - blue >= 10 && green - blue <= 25 && green - (2 * red) >= 8) {
      return "Red";
    } else if (green - red <= 10 && green - red >= -3 && blue >= green) {
      return "Yellow";
    } else if (blue - red >= 5 && blue - red <= 15 && green - (2 * red) <= 5 && green > 50) {
      return "Pink";
    } else if (green - blue >= -5 && green - blue <= 5 && green - (2 * red) <= 5 ) {
      return "Orange";
    }
  } else if (green < red && green < blue && green < 25) {
    return "Green";
  } else if ((red > green && blue < green) && blue < 25 && red > 40) {
    return "Blue";
  } else if (red - (2 * blue) >= -2 && red - (2 * blue) <= 5 && green - red < 10) {
    return "Purple";
  } else if (blue < red && blue < green && (blue && red and green) < 25) {
    if (red - green <= 5 && red - green >= 0 && ((green - blue) || (red - blue)) < 5 && blue < 50) {
      return "White";
    }
  }
  return "Unknown";
}
