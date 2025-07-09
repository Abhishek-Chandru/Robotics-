#include <Servo.h>  // Include the Servo library

#define SENSOR_LEFT A0
#define SENSOR_RIGHT A1
#define THRESHOLD_BLACK 140  // Threshold for detecting black (adjust as needed)

Servo servoLeft;   // Create servo object for the left motor
Servo servoRight;  // Create servo object for the right motor

// Define the target coordinates
int targetX[] = {1, 1, 3, 3, 4, 4, 5, 5,6};
int targetY[] = {1, 2, 2, 3, 3, 2, 2, 4, 4};

// Define current position and direction
int currentX = 0;
int currentY = 1;
char currentDirection = 'd';  // Initial direction pointing right

void setup() {
  servoLeft.attach(13);  
  servoRight.attach(12); 
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  for (int i = 0; i < 9; i++) { // Ensure loop goes through all targets
    Serial.print("Current Position: X = ");
    Serial.print(currentX);
    Serial.print(", Y = ");
    Serial.println(currentY);

    // Step 1: Move to targetX
    moveToX(targetX[i]);

    // Step 2: Move to targetY
    moveToY(targetY[i]);

    // Determine the next move if there's a next target
    if (i < 8) { // Ensure we do not go out of bounds
      determineNextMove(targetX[i + 1], targetY[i + 1]);
    }
  }

  stopMotors();
  Serial.println("Reached final target. Stopping.");
  while (true);  // Infinite loop to stop further execution
}

void moveToX(int targetX) {
  Serial.print("Moving to X: ");
  Serial.println(targetX);

  if (currentX < targetX) {
    alignDirection('d');
    while (currentX < targetX) {
      forward();
      int sensorLeftValue = analogRead(SENSOR_LEFT);
      if (sensorLeftValue < THRESHOLD_BLACK) {
        currentX++;
        Serial.print("Current X: ");
        Serial.print(currentX);
        Serial.print(", Y: ");
        Serial.println(currentY);
        delay(500);
      }
    }
  } else if (currentX > targetX) {
    alignDirection('a');
    while (currentX > targetX) {
      forward();
      int sensorLeftValue = analogRead(SENSOR_LEFT);
      if (sensorLeftValue < THRESHOLD_BLACK) {
        currentX--;
        Serial.print("Current X: ");
        Serial.print(currentX);
        Serial.print(", Y: ");
        Serial.println(currentY);
        delay(500);
      }
    }
  }
  stopMotors();
  Serial.println("Reached target X.");
}

void moveToY(int targetY) {
  Serial.print("Moving to Y: ");
  Serial.println(targetY);

  if (currentY < targetY) {
    alignDirection('w');
    while (currentY < targetY) {
      forward();
      int sensorRightValue = analogRead(SENSOR_RIGHT);
      if (sensorRightValue < THRESHOLD_BLACK) {
        currentY++;
        Serial.print("Current X: ");
        Serial.print(currentX);
        Serial.print(", Y: ");
        Serial.println(currentY);
        delay(500);
      }
    }
  } else if (currentY > targetY) {
    alignDirection('s');
    while (currentY > targetY) {
      forward();
      int sensorRightValue = analogRead(SENSOR_RIGHT);
      if (sensorRightValue < THRESHOLD_BLACK) {
        currentY--;
        Serial.print("Current X: ");
        Serial.print(currentX);
        Serial.print(", Y: ");
        Serial.println(currentY);
        delay(500);
      }
    }
  }
  stopMotors();
  Serial.println("Reached target Y.");
}

void determineNextMove(int nextX, int nextY) {
  Serial.print("Determining next move to X: ");
  Serial.print(nextX);
  Serial.print(", Y: ");
  Serial.println(nextY);

  if (currentX == nextX) {
    // Check Y values if X values are the same
    if (nextY > currentY) {
      alignDirection('w');
    } else if (nextY < currentY) {
      alignDirection('s');
    }
  } else if (currentY == nextY) {
    // Check X values if Y values are the same
    if (nextX > currentX) {
      alignDirection('d');
    } else if (nextX < currentX) {
      alignDirection('a');
    }
  }

  Serial.println("Turn completed.");
}

void alignDirection(char desiredDirection) {
  if (currentDirection == desiredDirection) return;

  Serial.print("Aligning from ");
  Serial.print(currentDirection);
  Serial.print(" to ");
  Serial.println(desiredDirection);

  if (currentDirection == 'd') {
    if (desiredDirection == 'w') {
      turnLeft();
    } else if (desiredDirection == 's') {
      turnRight();
    } else if (desiredDirection == 'a') {
      turnLeft();
      turnLeft();
    }
  } else if (currentDirection == 'a') {
    if (desiredDirection == 'w') {
      turnRight();
    } else if (desiredDirection == 's') {
      turnLeft();
    } else if (desiredDirection == 'd') {
      turnRight();
      turnRight();
    }
  } else if (currentDirection == 'w') {
    if (desiredDirection == 'd') {
      turnRight();
    } else if (desiredDirection == 'a') {
      turnLeft();
    } else if (desiredDirection == 's') {
      turnRight();
      turnRight();
    }
  } else if (currentDirection == 's') {
    if (desiredDirection == 'd') {
      turnLeft();
    } else if (desiredDirection == 'a') {
      turnRight();
    } else if (desiredDirection == 'w') {
      turnLeft();
      turnLeft();
    }
  }
  currentDirection = desiredDirection;
}

void forward() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
}

void turnLeft() {
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);
  delay(900);  // Adjust delay as needed for a 90-degree turn
}

void turnRight() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);
  delay(900);  // Adjust delay as needed for a 90-degree turn
}

void stopMotors() {
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1520);
}

