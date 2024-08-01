#include <Servo.h>

const int interruptPlus = 3;
const int interruptMinus = 2;

const int minAngle = 500;
const int maxAngle = 2400;
const int step = 100;

Servo myServo;
int angle = 1500; // 0 - 3000

void setup()
{
  myServo.attach(9);
  myServo.writeMicroseconds(angle);
  
  pinMode(interruptPlus, INPUT);
  pinMode(interruptMinus, INPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPlus), onPlus, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptMinus), onMinus, FALLING);
}

void loop() {}

void onPlus()
{
  angle += step;

  if (angle > maxAngle)
    angle = maxAngle;

  myServo.writeMicroseconds(angle);
}

void onMinus()
{
  angle -= step;

  if (angle < minAngle)
    angle = minAngle;

  myServo.writeMicroseconds(angle);
}
