#include <Servo.h>  // Include the Servo library
 
#define SENSOR_LEFT A0
#define SENSOR_RIGHT A1
#define THRESHOLD 205  // Adjust this value based on your sensor readings for the white line
 
Servo servoLeft;   // Create servo object for the left motor
Servo servoRight;  // Create servo object for the right motor
 
 //servoLeft.attach(L_MOTOR, 1000, 2000);  // Specify min and max pulse width in microseconds
 // servoRight.attach(R_MOTOR, 1000, 2000);
void setup() {
  // Attach servos to pins
  servoLeft.attach(13,1000, 2000);  
  servoRight.attach(12, 2000, 1000);
 
  // Initialize optical sensor pins
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
 
  // Initialize Serial communication for debugging
  Serial.begin(9600);
}
 
void loop() {
  // Read sensor values
  int sensorLeftValue = analogRead(SENSOR_LEFT);
  int sensorRightValue = analogRead(SENSOR_RIGHT);
 
  // Check if sensors detect the white line
  bool leftOnWhite = sensorLeftValue > THRESHOLD;
  bool rightOnWhite = sensorRightValue > THRESHOLD;
 
  // Print sensor values and boolean values for debugging
  Serial.print("Left Sensor: ");
  Serial.print(sensorLeftValue);
  Serial.print("  Left On White: ");
  Serial.print(leftOnWhite);  // Will print 1 (true) or 0 (false)
 
  Serial.print("  Right Sensor: ");
  Serial.print(sensorRightValue);
  Serial.print("  Right On White: ");
  Serial.println(rightOnWhite);  // Will print 1 (true) or 0 (false)
 
  // Decision making based on sensor readings
  if (leftOnWhite && rightOnWhite) {
    forward();  // Both sensors detect white, move forward
  } else if (!leftOnWhite && rightOnWhite) {
    turnLeft();  // Left sensor detects black, turn left
  } else if (leftOnWhite && !rightOnWhite) {
    turnRight();  // Right sensor detects black, turn right
  } else {
    stopMotors();  // Both sensors detect black, stop the motors
  }
 
  delay(150); // Small delay for stability
}
 
void forward() {
  servoLeft.writeMicroseconds(1600);  // Adjust values for forward motion
  servoRight.writeMicroseconds(1400); // Adjust values for forward motion
}
 
void turnLeft() {
  servoLeft.writeMicroseconds(1300);  // Stop the left wheel
  servoRight.writeMicroseconds(1200); // Right wheel forward
}
 
void turnRight() {
  servoLeft.writeMicroseconds(1700);  // Left wheel forward
  servoRight.writeMicroseconds(1800); // Stop the right wheel
}
 
void stopMotors() {
  servoLeft.writeMicroseconds(1500); // Stop the left servo
  servoRight.writeMicroseconds(1520); // Stop the right servo
}