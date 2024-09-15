void setup()
{
    Serial.begin(9600);
}

void loop()
{
    delay(2000);
    
    Serial.write(0xC1); // Передаём 3 байта, чтобы модуль вернул текущие параметры
    Serial.write(0xC1);
    Serial.write(0xC1);
}
