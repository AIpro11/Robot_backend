#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor frontLeft(1);
AF_DCMotor rearLeft(2);
AF_DCMotor frontRight(3);
AF_DCMotor rearRight(4);
Servo scanner;
#define SERVO_PIN 10
const int LEFT_ANGLE = 45;
const int CENTER_ANGLE = 90;
const int RIGHT_ANGLE = 135;
#define TRIG_PIN A0
#define ECHO_PIN A1
const int MOTOR_SPEED = 160;
const int FOLLOW_DISTANCE = 30;
const int MAX_TRACK_DISTANCE = 100;
const int SCAN_DELAY = 120;
const int TURN_TIME = 200;
const int FORWARD_TIME = 150;
struct ScanResult {
  int angle;
  int distance;
};
int lostCount = 0;
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  scanner.attach(SERVO_PIN);
  scanner.write(CENTER_ANGLE);
  setMotorSpeed(MOTOR_SPEED);
  stopRobot();
  delay(1000);
  Serial.println("==============================");
  Serial.println("HAND FOLLOWING ROBOT");
  Serial.println("4 MOTOR CONFIGURATION");
  Serial.println("==============================");
}
void loop() {
  ScanResult target = scanForTarget();
  Serial.print("Target angle: ");
  Serial.print(target.angle);
  Serial.print(" | Target distance: ");
  Serial.println(target.distance);
  if (target.distance == 999 || target.distance > MAX_TRACK_DISTANCE) {
    if (lostCount < 2) lostCount++;
    if (lostCount >= 2) {
      stopRobot();
      Serial.println("TARGET LOST");
    }
    return;
  }
  lostCount = 0;
  if (target.distance <= FOLLOW_DISTANCE) {
    stopRobot();
    Serial.println("FOLLOW DISTANCE REACHED");
    return;
  }
  if (target.angle == LEFT_ANGLE) {
    turnLeft();
    delay(TURN_TIME);
  } else if (target.angle == RIGHT_ANGLE) {
    turnRight();
    delay(TURN_TIME);
  } else {
    moveForward();
    delay(FORWARD_TIME);
  }
  stopRobot();
  delay(100);
}
int readOnce() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 8000);
  if (duration == 0) {
    return 999;
  }
  int distance = duration * 0.0343 / 2;
  if (distance < 2 || distance > 400) {
    return 999;
  }
  return distance;
}
int getDistance() {
  int r[3];
  for (int i = 0; i < 3; i++) {
    r[i] = readOnce();
    delay(15);
  }
  if (r[0] > r[1]) { int t = r[0]; r[0] = r[1]; r[1] = t; }
  if (r[1] > r[2]) { int t = r[1]; r[1] = r[2]; r[2] = t; }
  if (r[0] > r[1]) { int t = r[0]; r[0] = r[1]; r[1] = t; }
  return r[1];
}
ScanResult scanForTarget() {
  int angles[] = { LEFT_ANGLE, CENTER_ANGLE, RIGHT_ANGLE };
  ScanResult best;
  best.angle = CENTER_ANGLE;
  best.distance = 999;
  for (int i = 0; i < 3; i++) {
    scanner.write(angles[i]);
    delay(SCAN_DELAY);
    int distance = getDistance();
    Serial.print("Scanning ");
    Serial.print(angles[i]);
    Serial.print(" degrees: ");
    Serial.println(distance);
    if (distance >= 2 && distance <= MAX_TRACK_DISTANCE && distance < best.distance) {
      best.angle = angles[i];
      best.distance = distance;
    }
  }
  scanner.write(CENTER_ANGLE);
  delay(100);
  return best;
}
void setMotorSpeed(int speed) {
  frontLeft.setSpeed(speed);
  rearLeft.setSpeed(speed);
  frontRight.setSpeed(speed);
  rearRight.setSpeed(speed);
}
void moveForward() {
  frontLeft.run(FORWARD);
  rearLeft.run(FORWARD);
  frontRight.run(FORWARD);
  rearRight.run(FORWARD);
}
void turnLeft() {
  frontLeft.run(BACKWARD);
  rearLeft.run(BACKWARD);
  frontRight.run(FORWARD);
  rearRight.run(FORWARD);
}
void turnRight() {
  frontLeft.run(FORWARD);
  rearLeft.run(FORWARD);
  frontRight.run(BACKWARD);
  rearRight.run(BACKWARD);
}
void stopRobot() {
  frontLeft.run(RELEASE);
  rearLeft.run(RELEASE);
  frontRight.run(RELEASE);
  rearRight.run(RELEASE);
}
/*
========================================
ASSUMED CONFIGURATION
========================================
Main Controller : Arduino Uno
Motor Shield    : L293D V1-compatible shield
Motors          : Four motors (M1,M2,M3,M4)
HC-SR04 TRIG    : A0
HC-SR04 ECHO    : A1
SG90 Signal     : D10
========================================
*/
