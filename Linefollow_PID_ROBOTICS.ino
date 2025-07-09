#include <Arduino.h>
#include <Servo.h>

// PID Constants
#define SETPOINT    0     // The goal is to keep the difference between the sensors at zero
#define KP          0.2   // The P value in PID
#define KI          0.1   // The I value in PID
#define KD          1.0   // The D value in PID

// Servo Pins
#define L_MOTOR     13    // Left motor pin
#define R_MOTOR     12    // Right motor pin

// Sensor Pins
#define SENSOR_LEFT A0    // Left sensor pin
#define SENSOR_RIGHT A1   // Right sensor pin

// Servo Objects
Servo servoLeft;
Servo servoRight;

// Speed Constants
#define MAX_SPEED_LEFT   2000   // The max speed for the left motor
#define SET_SPEED_LEFT   1800   // The goal speed for the left motor
#define MIN_SPEED_LEFT   1500   // The min speed for the left motor

#define MAX_SPEED_RIGHT  1500   // The max speed for the right motor
#define SET_SPEED_RIGHT  1200   // The goal speed for the right motor
#define MIN_SPEED_RIGHT  1000   // The min speed for the right motor

// Dead Zone Threshold
#define DEAD_ZONE   50    // Threshold value to consider small changes as zero

int lastError = 0;      // For storing PID error
int integral = 0;       // For storing integral value

void setup() {
  // Initialize Servos
  servoLeft.attach(L_MOTOR, 1000, 2000);  // Specify min and max pulse width in microseconds
  servoRight.attach(R_MOTOR, 1000, 2000); // Specify min and max pulse width in microseconds

  // Initialize Sensors
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  // Start Serial Communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read sensor values
  int leftSensorValue = analogRead(SENSOR_LEFT);
  int rightSensorValue = analogRead(SENSOR_RIGHT);

  // Print sensor values for debugging
  Serial.print("Left Sensor: ");
  Serial.print(leftSensorValue);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightSensorValue);

  // Compute the error (difference between sensor values)
  int error = leftSensorValue - rightSensorValue;

  // Apply the dead zone
  if (abs(error) < DEAD_ZONE) {
    error = 0;
  }

  // Compute the integral (accumulated error)
  integral += error;

  // Compute the motor adjustment
  int adjust = (error * KP) + (integral * KI) + ((error - lastError) * KD);

  // Print error, integral, and adjustment for debugging
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" | Integral: ");
  Serial.print(integral);
  Serial.print(" | Adjust: ");
  Serial.println(adjust);

  // Record the current error for the next iteration
  lastError = error;

  // Calculate speeds for left and right servos
  int leftSpeed, rightSpeed;

  // If error is positive, left sensor value is greater, turn right
  if (error > 0) {
    leftSpeed = constrain(SET_SPEED_LEFT + adjust, MIN_SPEED_LEFT, MAX_SPEED_LEFT);  // Decrease left servo speed to turn right
    rightSpeed = constrain(SET_SPEED_RIGHT + adjust, MIN_SPEED_RIGHT, MAX_SPEED_RIGHT); // Increase right servo speed to turn right
  }
  // If error is negative, right sensor value is greater, turn left
  else if (error < 0) {
    leftSpeed = constrain(SET_SPEED_LEFT + adjust, MIN_SPEED_LEFT, MAX_SPEED_LEFT); // Increase left servo speed to turn left
    rightSpeed = constrain(SET_SPEED_RIGHT + adjust, MIN_SPEED_RIGHT, MAX_SPEED_RIGHT); // Decrease right servo speed to turn left
  }
  // If error is zero, move straight
  else {
    leftSpeed = SET_SPEED_LEFT;   // Move left servo straight
    rightSpeed = SET_SPEED_RIGHT; // Move right servo straight
  }

  // Apply speeds to servos
  servoLeft.writeMicroseconds(leftSpeed);
  servoRight.writeMicroseconds(rightSpeed);

  // Print servo speeds for debugging
  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);

  // Small delay to allow servos to move
  delay(20);
}
