// TCS230 or TCS3200 pins wiring to Arduino
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red, green, and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Begins serial communication
  Serial.begin(9600);
}

void loop() {
  // Reading the RED (R) frequency
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, 50, 150, 255, 0);  // Adjusted range for red color

  // Reading the GREEN (G) frequency
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, 20, 120, 255, 0);  // Adjusted range for green color

  // Reading the BLUE (B) frequency
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, 30, 120, 255, 0);  // Adjusted range for blue color

  // Printing the RGB values
  Serial.print("R = ");
  Serial.print(redColor);
  Serial.print(" G = ");
  Serial.print(greenColor);
  Serial.print(" B = ");
  Serial.print(blueColor);

  // Detecting color based on RGB values
  detectColor();

  Serial.println();
  delay(500);
}

void detectColor() {
  int threshold = 50; // Adjust this threshold based on your conditions

  // Calculate the ratios of RGB values
  float redRatio = redColor / (float)(redColor + greenColor + blueColor);
  float greenRatio = greenColor / (float)(redColor + greenColor + blueColor);
  float blueRatio = blueColor / (float)(redColor + greenColor + blueColor);

  // Check for the dominant color
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
