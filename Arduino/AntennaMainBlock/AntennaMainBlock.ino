#include <Wire.h>
#include <QMC5883LCompass.h>
#include <MPU6050.h>
#include <MT6701.h>

// Sensor Objects
QMC5883LCompass compass;
MPU6050 mpu;
MT6701 mt6701; // Ensure the MT6701 library is installed

// Variables for MPU6050 Accelerometer Data
int16_t ax, ay, az;
float x, y, z;

// Calibration Offsets for MPU6050 (calculated during calibration)
int16_t accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;

// Calibration Offsets for QMC5883L Compass
int compassOffsetX = 0, compassOffsetY = 0, compassOffsetZ = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize QMC5883L Compass
  compass.init();
  compassCalibration();

  // Initialize MPU6050 Accelerometer
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1); // Halt execution if MPU6050 fails
  }
  // Configure DLPF for filtering and calibrate MPU6050
  mpu.setDLPFMode(6); // Set Digital Low Pass Filter to ~5 Hz
  calibrateMPU6050();

  // Initialize MT6701 Magnetic Angle Sensor
  Serial.println("MT6701 initialized (no explicit initialization required).");

  Serial.println("All sensors initialized and calibrated successfully.");
}

void loop() {
  /*
  // Read QMC5883L Compass Data
  int compassX, compassY, compassZ;
  compass.read(); // Read compass values into internal properties
  compassX = compass.getX() - compassOffsetX;
  compassY = compass.getY() - compassOffsetY;
  compassZ = compass.getZ() - compassOffsetZ;
  Serial.print("QMC5883L - X: "); Serial.print(compassX);
  Serial.print(" Y: "); Serial.print(compassY);
  Serial.print(" Z: "); Serial.println(compassZ);
  */

  // Read MPU6050 Accelerometer Data
  mpu.getAcceleration(&ax, &ay, &az);
  ax -= accelOffsetX;
  ay -= accelOffsetY;
  az -= accelOffsetZ;

  x = ax / 16384.0;
  y = ay / 16384.0;
  z = az / 16384.0;

  Serial.print("MPU6050 - Accel: ");
  Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" Z: "); Serial.println(z);

  /*
  // Read MT6701 Magnetic Angle Data
  float angle = mt6701.angleRead(); // Assuming getAngle() is defined in the MT6701 library
  Serial.print("MT6701 - Angle: ");
  Serial.println(angle);
  */
  delay(1000); // Delay for 1 second
}

// Function to calibrate QMC5883L Compass
void compassCalibration() {
  Serial.println("Calibrating QMC5883L...");
  int minX = 32767, maxX = -32768;
  int minY = 32767, maxY = -32768;
  int minZ = 32767, maxZ = -32768;

  // Rotate the sensor in all directions to record min/max values
  for (int i = 0; i < 500; i++) {
    compass.read();
    int x = compass.getX();
    int y = compass.getY();
    int z = compass.getZ();

    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;
    if (z < minZ) minZ = z;
    if (z > maxZ) maxZ = z;

    delay(10);
  }

  compassOffsetX = (maxX + minX) / 2;
  compassOffsetY = (maxY + minY) / 2;
  compassOffsetZ = (maxZ + minZ) / 2;

  Serial.print("Compass Calibration Offsets - X: "); Serial.print(compassOffsetX);
  Serial.print(" Y: "); Serial.print(compassOffsetY);
  Serial.print(" Z: "); Serial.println(compassOffsetZ);
}

// Function to calibrate MPU6050 Accelerometer
void calibrateMPU6050() {
  Serial.println("Calibrating MPU6050 Accelerometer...");
  int32_t sumX = 0, sumY = 0, sumZ = 0;
  int samples = 100;

  // Collect multiple samples to calculate the average offset
  for (int i = 0; i < samples; i++) {
    mpu.getAcceleration(&ax, &ay, &az);
    sumX += ax;
    sumY += ay;
    sumZ += az;
    delay(10);
  }

  // Calculate offsets
  accelOffsetX = sumX / samples;
  accelOffsetY = sumY / samples;
  accelOffsetZ = (sumZ / samples) - 16384; // Subtract g (1g = 16384 in MPU6050 units)

  Serial.print("Accel Calibration Offsets - X: "); Serial.print(accelOffsetX);
  Serial.print(" Y: "); Serial.print(accelOffsetY);
  Serial.print(" Z: "); Serial.println(accelOffsetZ);
}
