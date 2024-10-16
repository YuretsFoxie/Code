#include <Wire.h>                   // Wire library - used for I2C communication
#include <TimerOne.h>

const int xOffset = -3;             // Use Serial Plotter to adjust the values!
const int yOffset = 0;
const int zOffset = 1;

const int ADXL345 = 0x53;           // The ADXL345 sensor I2C address
const int movementStopLED = 2;
const int armedLED = 3;
const int explodePin = 4;
const int explodeToResistorPin = 5;
const unsigned long oneTenthOfSecond = 100000;
const unsigned long movementDelay = 50;   // There must be at least movementDelay / 10 seconds of the constant movement...
const unsigned long idleDelay = 600;      // And at least idleDelay / 10 seconds to arm the bomb
const float movementDelta = 0.03;         // Sensitivity threshold

float x, y, z;
unsigned long secondsCount = 0;
bool isMovementStopped = false;
bool isArmed = false;



void setup()
{
  pinMode(movementStopLED, OUTPUT);
  pinMode(armedLED, OUTPUT);
  pinMode(explodePin, OUTPUT);
  pinMode(explodeToResistorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(movementStopLED, LOW);
  digitalWrite(armedLED, LOW);
  digitalWrite(explodePin, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  Timer1.initialize(oneTenthOfSecond);
  Timer1.attachInterrupt(onTimer);

  // Accelerometer Calibration
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);                 // X-axis offset register
  Wire.write(xOffset);
  Wire.endTransmission();
  delay(10);
  
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);                 // Y-axis offset register
  Wire.write(yOffset);
  Wire.endTransmission();
  delay(10);
  
  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);                 // Z-axis offset register
  Wire.write(zOffset);
  Wire.endTransmission();
  delay(10);

  Wire.begin();                     // Initiate the Wire library

  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D

  // Enable measurement
  Wire.write(8);                    // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
}



void loop() {}



void onTimer(void)
{
  interrupts();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);                         // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);       // Read 6 registers total, each axis value is stored in 2 registers
  
  x = ( Wire.read() | Wire.read() << 8);     // X-axis value
  y = ( Wire.read() | Wire.read() << 8);     // Y-axis value
  z = ( Wire.read() | Wire.read() << 8);     // Z-axis value

  float a = sqrt(x * x + y * y + z * z) / 256;
  noInterrupts();
  
  //

  if (isMovementStopped == false)
  {
    if (secondsCount < movementDelay)
    {
      if (abs(a) >= 1 + movementDelta)
        secondsCount++;
      else
        secondsCount = 0;
    }
    else
    {
      secondsCount = 0;
      isMovementStopped = true;
      digitalWrite(movementStopLED, HIGH);
    }
  }
  else
  {
    if (secondsCount < idleDelay)
    {
      if (abs(a) < 1 + movementDelta)
        secondsCount++;
      else
        secondsCount = 0;
    }
    else
    {
      isArmed = true;
      digitalWrite(movementStopLED, LOW);
      digitalWrite(armedLED, HIGH);
    }
  }

  if (isArmed && abs(a) >= 1 + movementDelta)
    digitalWrite(explodePin, HIGH);

  if (abs(a) >= 1 + movementDelta)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}

// For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
