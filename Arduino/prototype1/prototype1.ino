#include <TimerOne.h>

const unsigned long oneSecond = 1000000;
const unsigned long delayInSeconds = 60;

const int timerButtonPin = 2;
const int explodeButtonPin = 3;
const int timerLEDPin = 4;
const int explodeLEDPin = 5;

unsigned long secondsCount = 0;
bool isTimer = false;
bool isOver = false;



void setup(void)
{
  pinMode(timerButtonPin, INPUT);
  pinMode(explodeButtonPin, INPUT);
  pinMode(timerLEDPin, OUTPUT);
  pinMode(explodeLEDPin, OUTPUT);
  
  Timer1.initialize(oneSecond);
  Timer1.attachInterrupt(onTimer);

  attachInterrupt(digitalPinToInterrupt(timerButtonPin), onTimerButton, RISING);
  attachInterrupt(digitalPinToInterrupt(explodeButtonPin), onExplodeButton, CHANGE); // both press and release are handled
}

void loop(void) {}



void onTimer(void)
{
  if (!isTimer)
    return;

  secondsCount++;

  if (secondsCount >= delayInSeconds)
    isOver = true;

  digitalWrite(timerLEDPin, isOver ? HIGH : !digitalRead(timerLEDPin));
}



void onTimerButton(void)
{
  isTimer = true;
}



void onExplodeButton(void)
{
  if (isOver)
    digitalWrite(explodeLEDPin, HIGH);
}
