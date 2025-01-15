#include <Servo.h>

// Define pins
const int buttonA = 2; // Button A pin
const int buttonB = 3; // Button B pin
const int buttonC = 4; // Button C pin
const int escPin = 9;  // ESC signal pin

// Timing and state variables
unsigned long motorStartTime = 0; // Stores the time the motor started
unsigned long runDuration = 0;    // Stores the motor run duration
bool motorRunning = false;        // Motor state

Servo esc; // Create Servo object for ESC

void setup() {
  // Initialize Serial (optional for debugging)
  Serial.begin(9600);

  // Set button pins as inputs with pull-up resistors
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);

  // Attach ESC to signal pin
  esc.attach(escPin);
  esc.writeMicroseconds(1000); // Initialize ESC to idle
}

void loop() {
  // Read button states
  if (digitalRead(buttonA) == LOW) {
    runMotor(50, 25000); // 50% speed for 25 seconds
  } else if (digitalRead(buttonB) == LOW) {
    runMotor(25, 30000); // 25% speed for 30 seconds
  } else if (digitalRead(buttonC) == LOW) {
    stopMotor(); // Stop the motor (idle)
  }

  // Check if motor should return to idle
  if (motorRunning && millis() - motorStartTime >= runDuration) {
    stopMotor();
  }
}

// Function to run the motor at a given speed and duration
void runMotor(int speedPercent, unsigned long duration) {
  int pwmSignal = map(speedPercent, 0, 100, 1000, 2000); // Map speed to PWM range
  esc.writeMicroseconds(pwmSignal); // Set ESC speed
  motorStartTime = millis(); // Record start time
  runDuration = duration;    // Set run duration
  motorRunning = true;       // Set motor state
  Serial.print("Motor running at ");
  Serial.print(speedPercent);
  Serial.println("% speed");
}

// Function to stop the motor
void stopMotor() {
  esc.writeMicroseconds(1000); // Set ESC to idle (1000 µs)
  motorRunning = false;        // Set motor state
  Serial.println("Motor stopped (idle)");
}
