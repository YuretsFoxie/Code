#include <TimerOne.h>

const unsigned long period = 500; // 1000000 for one second

const int inputPin = 3;
const int outputPin = 4;
const int ledPin = 13;

void setup()
{
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Timer1.initialize(period);
  Timer1.attachInterrupt(onTimer);
}

void loop() {}

void onTimer(void)
{
  bool isAlert = digitalRead(inputPin) == HIGH;

  if (isAlert)
    digitalWrite(outputPin, !digitalRead(outputPin));

  digitalWrite(ledPin, isAlert);
}
