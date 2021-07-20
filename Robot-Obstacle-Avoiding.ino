#include <Servo.h>          //Servo motor library.
#include <NewPing.h>        //Ultrasonic sensor function library.

//our HEX WB291111 control pins
const int enB = 6;
const int LeftMotorForward = 5;
const int LeftMotorBackward = 4;

const int enA = 7;
const int RightMotorForward = 3;
const int RightMotorBackward = 2;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;

#define maxSpeed 100
int currentSpeed = 100;

boolean turnedLeft = false;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
Servo servo_motor; //our servo name


void setup() {

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  servo_motor.attach(11); //our servo pin

  servo_motor.write(90);
  delay(3000);
  distance = readPing();
  delay(200);
  distance = readPing();
  delay(200);
  distance = readPing();
  delay(200);
  distance = readPing();
  delay(200);
}

void loop() {
  boolean isCloseToObstacle = false;
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  //When we recognize obstacle between 7 cm to 15 cm, we'll slow down
  if(distance <= 150 && distance > 65 && distance%10 == 0)
  {
    currentSpeed -= 10;
    isCloseToObstacle = true;
  }
  else if (distance <= 65) {
    moveStop();
    delay(500);
    moveBackward();
    delay(600);
    moveStop();
    delay(500);
    distanceRight = lookRight();
    delay(500);
    distanceLeft = lookLeft();
    delay(500);

    if (distance >= distanceLeft || distance >= distanceRight) { //the distance of one of the sides is longer than 6 cm
      if (distanceLeft < distanceRight)
      {
        turnRight();
        moveStop();
      }
      else
      {
        turnLeft();
        moveStop();
      }
    }
    else { //The distance on both sides is shorter than 6 cm, so we'll reverse
      moveBackward();
    }
  }
  else {
    if(isCloseToObstacle == true)
    {
        currentSpeed = 100;
    }
    moveForward();
  }
  
  distance = readPing();
}

int lookRight() {
  servo_motor.write(10);
  delay(1000);
  int distance = readPing();
  delay(500);
  servo_motor.write(90);
  return distance;
}

int lookLeft() {
  servo_motor.write(170);
  delay(1000);
  int distance = readPing();
  delay(500);
  servo_motor.write(90);
  return distance;
  delay(500);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) { //the sensor dont recognize any obstacle that is in distance less than max distance 
    cm = 250;
  }
  return cm;
}

void moveStop() {

  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward() {

  if (!goesForward) {

    goesForward = true;

    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);

    analogWrite(enA, currentSpeed);

    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);

    analogWrite(enB, currentSpeed);

  }
}

void moveBackward() {

  goesForward = false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  analogWrite(enA, 100);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);

  analogWrite(enB, 100);

}

void turnRight() {

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  analogWrite(enA, 200);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);

  analogWrite(enB, 300);

  delay(500);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  analogWrite(enA, 200);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  analogWrite(enB, 300);

}

void turnLeft() {

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  analogWrite(enA, 300);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  analogWrite(enB, 200);

  delay(500);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  analogWrite(enA, 300);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  analogWrite(enB, 200);
}
