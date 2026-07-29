# Ultrasonic-Controlled-Servo-Motor-with-Arduino


An embedded C++ program for the **Arduino Uno R3** that integrates an **HC-SR04 Ultrasonic Sensor** with an **SG90 Micro Servo Motor**. The system continuously measures distance in real time and automatically triggers servo actuation when an object enters a defined proximity zone ($\le 10\text{ cm}$).

---

## 📌 Project Idea & Overview

The goal of this project is to create an automated touchless mechanism triggered by distance measurement. Potential real-world applications include automated trash bin lids, smart barrier gates, or proximity-activated dispensers.

### System Logic
1. **Continuous Sensing:** The HC-SR04 sensor transmits ultrasonic pulses and measures the return echo time to compute live distance in centimeters.
2. **Threshold Trigger:** When an object is detected at a distance of **$\le 10\text{ cm}$**, the servo motor rotates from its idle position ($0^\circ$) to its target active angle ($90^\circ$).
3. **Automatic Reset:** As soon as the object moves beyond $10\text{ cm}$, the servo automatically returns to its original position ($0^\circ$).

---

## 🛠️ Hardware & Components

| Component | Quantity | The need |
| :--- | :---: | :--- |
| **Arduino Uno R3** | 1 | Microcontroller board |
| **HC-SR04 Ultrasonic Sensor** | 1 | Ultrasonic distance sensor (at least >9) |
| **SG90 Micro Servo Motor** | 1 | 180° rotation micro servo |
| **Breadboard** | 1 | Extra Space |
| **Wires** | 5/4 | Male-to-Male/Male-to-Female hookup wires |
| **USB Cable (Type-A to Type-B)** | 1 | Microcontroller programming & power supply cable |

---

## 🔌 Circuit Connections & Pinout Table

Both the ultrasonic sensor and servo motor share the common 5V and GND rails on the breadboard powered directly by the Arduino Uno.

### Component Pinout

| Device | Component Pin | Connection / Rail | Arduino Pin |
| :--- | :--- | :--- | :--- |
| **HC-SR04 Sensor** | **VCC** | Breadboard **`+` Rail** | 5V |
| | **Trig** | Signal Wire | **Digital Pin 9** |
| | **Echo** | Signal Wire | **Digital Pin 10** |
| | **GND** | Breadboard **`-` Rail** | GND |
| **SG90 Servo** | **Red Wire** ($V_{CC}$) | Breadboard **`+` Rail** | 5V |
| | **Brown Wire** ($GND$) | Breadboard **`-` Rail** | GND |
| | **Orange Wire** (Signal) | Signal Wire | **Digital Pin 6** |

---

## 📐 Distance Calculation Formula

The distance $D$ (in cm) is calculated using the time duration $t$ (in microseconds) of the returned ultrasonic echo pulse:

$$D = \frac{t \times 0.0343}{2}$$

* **$0.0343\text{ cm/\mu s}$** represents the speed of sound in air.
* Division by **$2$** accounts for the two-way travel path of the ultrasonic pulse (to the object and back).

---

## 💻 The C++ Code

```cpp
#include <Servo.h>

// Pin Definitions
const int TRIG_PIN  = 9;   // Ultrasonic Trig pin
const int ECHO_PIN  = 10;  // Ultrasonic Echo pin
const int SERVO_PIN = 6;   // Servo Signal pin

// Angle Configurations
const int ORIGINAL_POS = 0;   // Default position (0 degrees)
const int TARGET_POS   = 90;  // Position when object is detected (90 degrees)

// Distance Threshold (in centimeters)
const float THRESHOLD_CM = 10.0;

Servo myServo;

void setup() {
  // Set pin modes for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Attach servo motor and set to initial position
  myServo.attach(SERVO_PIN);
  myServo.write(ORIGINAL_POS);
  
  // Start serial communication for live distance monitoring
  Serial.begin(9600);
}

void loop() {
  // 1. Send a 10-microsecond pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 2. Measure duration of the echo pulse (in microseconds)
  long duration = pulseIn(ECHO_PIN, HIGH);

  // 3. Calculate distance in centimeters
  float distanceCm = duration * 0.0343 / 2.0;

  // Print distance to Serial Monitor for testing
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // 4. Check if object is closer than or equal to 10 cm
  if (distanceCm > 0 && distanceCm <= THRESHOLD_CM) {
    myServo.write(TARGET_POS);    // Move servo to target angle (90°)
  } else {
    myServo.write(ORIGINAL_POS);  // Return servo to default position (0°)
  }

  delay(100); // Small delay for sensor stability
}
