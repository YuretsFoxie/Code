#include <Servo.h>

Servo myServo;  // Об'єкт для керування сервоприводом

const int photoResistorPin = A0;  // Пін, до якого підключено фоторезистор
const int servoPin = 9;           // Пін, до якого підключено сервопривід
const int lightThreshold = 500;   // Поріг освітленості (можна змінити)

bool lightWasOn = false;  // Змінна для відстеження стану світла

void setup() {
  myServo.attach(servoPin);  // Підключаємо сервопривід
  pinMode(photoResistorPin, INPUT);  // Налаштовуємо пін фоторезистора як вхід
}

void loop() {
  int lightLevel = analogRead(photoResistorPin);  // Зчитуємо рівень освітленості

  if (lightLevel > lightThreshold) {
    // Якщо світло ввімкнено
    if (!lightWasOn) {
      // Якщо світло щойно ввімкнулось, повертаємо сервопривід на 45 градусів вліво
      myServo.write(45);
      lightWasOn = true;  // Запам'ятовуємо, що світло було ввімкнено
    }
  } else {
    // Якщо світло вимкнено
    if (lightWasOn) {
      // Якщо світло щойно вимкнулось, залишаємо сервопривід на місці
      lightWasOn = false;  // Запам'ятовуємо, що світло було вимкнено
    }
  }

  // Якщо світло знову ввімкнено після вимкнення, повертаємо сервопривід на 45 градусів вправо
  if (lightLevel > lightThreshold && !lightWasOn) {
    myServo.write(135);
    lightWasOn = true;  // Запам'ятовуємо, що світло знову ввімкнено
  }

  delay(100);  // Невелика затримка для стабілізації
}
