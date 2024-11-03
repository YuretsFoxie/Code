#include <AccelStepper.h>
#include <Arduino.h>

const int xPin = 7;
const int yPin = 6;

int x = 0;  // x range: 0...1023; x zero: 508...510
int y = 0;  // y range: 0...1023; y zero: 521...523

bool isReadingX = true;
bool isUp = false;
bool isDown = false;
bool isLeft = false;
bool isRight = false;

AccelStepper xStepper(4, 12, 11, 10, 9);
AccelStepper yStepper(4, 8, 7, 6, 5);



void setup()
{
  xStepper.setMaxSpeed(500);
  yStepper.setMaxSpeed(500);
  setupTimer2();
}



void setupTimer2()
{
  // Disable all interrupts
  noInterrupts();
  
  // Clear Timer2 control registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // Set Timer2 to CTC mode (Clear Timer on Compare Match)
  TCCR2A |= (1 << WGM21);

  // Set prescaler to 1024
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  // Set compare match value for 0.1 second interval
  OCR2A = 156;

  // Enable Timer2 compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  // Enable global interrupts
  interrupts();
}



void loop()
{
  xStepper.runSpeed();
  yStepper.runSpeed();
}



ISR(TIMER2_COMPA_vect)
{
  if (isReadingX)
  {
    x = analogRead(xPin);
    isDown = (x < 500);
    isUp = (x > 520);
  }
  else
  {
    y = analogRead(yPin);
    isLeft = (y < 510);
    isRight = (y > 530);
  }
  
  if (isUp)
    xStepper.setSpeed(100);
  else if (isDown)
    xStepper.setSpeed(-100);
  else
    xStepper.setSpeed(0);

  if (isLeft)
    yStepper.setSpeed(100);
  else if (isRight)
    yStepper.setSpeed(-100);
  else
    yStepper.setSpeed(0);

  isReadingX = !isReadingX;
}
