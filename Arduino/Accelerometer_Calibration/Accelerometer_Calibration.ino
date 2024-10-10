#include <Wire.h>                   // Wire library - used for I2C communication

const int ADXL345 = 0x53;           // The ADXL345 sensor I2C address
const int xOffset = -5;             // Use Serial Plotter to adjust the values!
const int yOffset = -2;
const int zOffset = 0;

float x, y, z;



void setup()
{
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

  Serial.begin(9600);               // Initiate serial communication for printing the results on the Serial monitor
  Wire.begin();                     // Initiate the Wire library

  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D

  // Enable measurement
  Wire.write(8);                    // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
}



void loop()
{
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);                         // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);       // Read 6 registers total, each axis value is stored in 2 registers
  
  x = ( Wire.read()| Wire.read() << 8);     // X-axis value
  y = ( Wire.read()| Wire.read() << 8);     // Y-axis value
  z = ( Wire.read()| Wire.read() << 8);     // Z-axis value

  // For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
/*
  Serial.print("x = ");
  Serial.print(x / 256);
  Serial.print("y = ");
  Serial.print(y / 256);
  Serial.print("z = ");
  Serial.println(z / 256);
*/

  float a = sqrt(x * x + y * y + z * z) / 256;
  Serial.println(a);
}
