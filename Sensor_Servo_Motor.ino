#include <Servo.h>

// Pin Definitions
const int TRIG_PIN  = 9;   //  Trig pin
const int ECHO_PIN  = 10;  //  Echo pin
const int SERVO_PIN = 6;   // Servo Signal pin

// Angle Configurations
const int ORIGINAL_POS = 0;   // Default position (0 degrees)
const int TARGET_POS   = 90;  // Position when object is detected (90 degrees)


const float THRESHOLD_CM = 10.0;

Servo myServo;

void setup() {
  // Set pin  ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  
  myServo.attach(SERVO_PIN);
  myServo.write(ORIGINAL_POS);
  
  
  Serial.begin(9600);
}

void loop() {
  // 1.  trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 2. Measure duration 
  long duration = pulseIn(ECHO_PIN, HIGH);

  // 3. Calculate distance in centimeters
  float distanceCm = duration * 0.0343 / 2.0;

  
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // 4. Check if object is closer than or equal to 10 cm
  if (distanceCm > 0 && distanceCm <= THRESHOLD_CM) {
    myServo.write(TARGET_POS);    // Move servo
  } else {
    myServo.write(ORIGINAL_POS);  // Return servo to default position (0°)
  }

  delay(100); // Small delay for stability
}