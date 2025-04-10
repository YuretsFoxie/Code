#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 1);
enum Indicators { Left, Right };

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false); // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.setIntensity(0, 8); // Set the brightness to a medium values
  lc.clearDisplay(0);   // and clear the display

  show(Left, 0);
  show(Right, 0);
}

void loop() {
  if (Serial.available() >= sizeof(int) * 2) { // Check if enough bytes are available
    int receivedValue1, receivedValue2;

    // Read the two integers
    Serial.readBytes((char*)&receivedValue1, sizeof(receivedValue1));
    Serial.readBytes((char*)&receivedValue2, sizeof(receivedValue2));
    
    lc.clearDisplay(0);
    show(Left, receivedValue1);
    show(Right, receivedValue2);
  }
}

void show(Indicators indicator, int value)
{
  int i = (indicator == Left) ? 4 : 0;

  if (value < 0)
  {
    lc.setChar(0, 3 + i, '-', false);
  }

  int absValue = abs(value);

  int hundreds = absValue / 100;
  int decades = (absValue - hundreds * 100) / 10;
  int units = absValue - hundreds * 100 - decades * 10;

  lc.setDigit(0, 2 + i, hundreds, false);
  lc.setDigit(0, 1 + i, decades, false);
  lc.setDigit(0, 0 + i, units, false);
}



/*
#include "LedControl.h"

// Pin 12 is connected to the DATA IN-pin of the first MAX7221
// Pin 11 is connected to the CLK-pin of the first MAX7221
// Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221

LedControl lc = LedControl(12, 11, 10, 1);
enum Indicators { Left, Right };

void setup() {
  lc.shutdown(0,false); // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.setIntensity(0,8); // Set the brightness to a medium values
  lc.clearDisplay(0);   // and clear the display
  setupTimer2();

  show(Left, 0);
  show(Right, 0);
}

void loop() {}



ISR(TIMER2_COMPA_vect) {
  int x = 10 - analogRead(A1) / 50;
  int y = 10 - analogRead(A0) / 50;

  lc.clearDisplay(0);
  show(Left, x);
  show(Right, -y);
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

void show(Indicators indicator, int value)
{
  int i = (indicator == Left) ? 4 : 0;

  if (value < 0)
  {
    lc.setChar(0, 3 + i, '-', false);
  }

  int absValue = abs(value);

  int hundreds = absValue / 100;
  int decades = (absValue - hundreds * 100) / 10;
  int units = absValue - hundreds * 100 - decades * 10;

  lc.setDigit(0, 2 + i, hundreds, false);
  lc.setDigit(0, 1 + i, decades, false);
  lc.setDigit(0, 0 + i, units, false);
}
*/
