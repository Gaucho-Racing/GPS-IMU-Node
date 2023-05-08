#include <GPS.h>
#include <IMU.h>

unsigned long timer = 0;

void setup()
{
  Serial.begin(115200);
  GPSSetup();
  IMUsetup();
}

void loop()
{
  if(true){
    long time = millis();
    getGPSData();
    Serial.println(millis()-time);
    delay(100);
    timer = millis();
  }
  IMUloop();
}