#include <Servo.h>
#include <Arduino.h>

const int plusPin = 3;
const int minusPin = 6;
const int gndPin = 4;
const int pullupPin = 2;

const int minAngle = 500;
const int maxAngle = 2400;
const int step = 10;

Servo myServo;
int angle = 1500; // 0 - 3000

void onPlus();
void onMinus();



void setup()
{
  myServo.attach(9);
  myServo.writeMicroseconds(angle);
  
  pinMode(plusPin, INPUT_PULLUP);
  pinMode(minusPin, INPUT_PULLUP);
  pinMode(gndPin, INPUT_PULLUP);
  pinMode(pullupPin, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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



void loop() {}



// Timer2 interrupt service routine
ISR(TIMER2_COMPA_vect)
{
  bool isPlusPushed = digitalRead(plusPin) == LOW;  
  bool isMinusPushed = digitalRead(minusPin) == LOW;

  if (isPlusPushed && !isMinusPushed)
    onPlus();

  if (!isPlusPushed && isMinusPushed)
    onMinus();
}



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
