#define MQ135_PIN 26
#define TCS230_S2 4
#define TCS230_S3 2
#define TCS230_OUT 15
#define HC_SR04_TRIGGER 14
#define HC_SR04_ECHO 13
#define PH_SENSOR_PIN A0

// Calibration values for the pH sensor
float pH4Voltage = 1.94;  // Adjust this value based on your sensor's characteristics
float pH7Voltage = 2.5;   // Adjust this value based on your sensor's characteristics
float mVperpH = 59.16;    // Adjust this value based on your sensor's characteristics

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red, green, and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PH_SENSOR_PIN, INPUT);

  pinMode(TCS230_S2, OUTPUT);
  pinMode(TCS230_S3, OUTPUT);
  pinMode(TCS230_OUT, INPUT);
  digitalWrite(TCS230_S2, LOW);
  digitalWrite(TCS230_S3, LOW);

  pinMode(HC_SR04_TRIGGER, OUTPUT);
  pinMode(HC_SR04_ECHO, INPUT);
}

void loop() {
  float phValue = readPH();
  float mq135Value = readMQ135();
  readRGB();
  float distance = readHC_SR04();

  // Normalize pH value between 0 and 14
  phValue = constrain(phValue, 0.0, 14.0);

  // Classify color based on thresholds
  detectColor(50); // Adjust the threshold value based on your conditions

  // Display sensor values on Serial Monitor
  Serial.print("pH: ");
  Serial.print(phValue);
  Serial.print(" | MQ135: ");
  Serial.print(mq135Value);
  Serial.print(" | RGB: R=");
  Serial.print(redColor);
  Serial.print(" G=");
  Serial.print(greenColor);
  Serial.print(" B=");
  Serial.print(blueColor);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(5000); // Adjust the delay based on your requirements
}

float readPH() {
  // For testing purposes, return a static pH value
  return 7.0;
}

float readMQ135() {
  // Implement MQ135 sensor reading logic here
  // Placeholder return value, replace with actual logic
  return analogRead(MQ135_PIN);
}

void readRGB() {
  // Set S2 and S3 for BLUE color
  digitalWrite(TCS230_S2, LOW);
  digitalWrite(TCS230_S3, HIGH);
  blueColor = pulseIn(TCS230_OUT, HIGH);

  // Set S2 and S3 for RED color
  digitalWrite(TCS230_S2, HIGH);
  digitalWrite(TCS230_S3, HIGH);
  redColor = pulseIn(TCS230_OUT, HIGH);

  // Set S2 and S3 for GREEN color
  digitalWrite(TCS230_S2, LOW);
  digitalWrite(TCS230_S3, LOW);
  greenColor = pulseIn(TCS230_OUT, HIGH);
}

float readHC_SR04() {
  // Implement HC-SR04 sensor reading logic here
  digitalWrite(HC_SR04_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC_SR04_TRIGGER, LOW);

  return pulseIn(HC_SR04_ECHO, HIGH) * 0.034 / 2;
}

void detectColor(int threshold) {
  float redRatio = redColor / (float)(redColor + greenColor + blueColor);
  float greenRatio = greenColor / (float)(redColor + greenColor + blueColor);
  float blueRatio = blueColor / (float)(redColor + greenColor + blueColor);

  if (redRatio > greenRatio && redRatio > blueRatio && redColor > threshold) {
    Serial.println(" - Red detected!");
  } else if (greenRatio > redRatio && greenRatio > blueRatio && greenColor > threshold) {
    Serial.println(" - Green detected!");
  } else if (blueRatio > redRatio && blueRatio > greenRatio && blueColor > threshold) {
    Serial.println(" - Blue detected!");
  } else if (redRatio > greenRatio && blueRatio > greenRatio && redColor > threshold && blueColor > threshold) {
    Serial.println(" - Magenta detected!");
  } else if (redRatio > blueRatio && greenRatio > blueRatio && redColor > threshold && greenColor > threshold) {
    Serial.println(" - Yellow detected!");
  } else if (greenRatio > redRatio && blueRatio > redRatio && greenColor > threshold && blueColor > threshold) {
    Serial.println(" - Cyan detected!");
  } else if (redColor > threshold && greenColor > threshold && blueColor > threshold) {
    Serial.println(" - White detected!");
  } else if (redColor < threshold && greenColor < threshold && blueColor < threshold) {
    Serial.println(" - Black detected!");
  } else if (redColor > blueColor && redColor > greenColor && redColor > threshold) {
    Serial.println(" - Orange detected!");
  } else if (redRatio > greenRatio && blueRatio > greenRatio && redColor > threshold && blueColor < threshold) {
    Serial.println(" - Pink detected!");
  } else {
    Serial.println(" - Unknown color");
  }
}
