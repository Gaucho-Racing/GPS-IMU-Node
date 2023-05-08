/*
  Get the high precision geodetic solution for latitude and longitude
  By: Nathan Seidle
  Modified by: Steven Rowland and Paul Clark
  SparkFun Electronics
  Date: April 17th, 2020
  License: MIT. See license file for more information.

  This example shows how to inspect the accuracy of the high-precision
  positional solution. Please see below for information about the units.

  Feel like supporting open source hardware?
  Buy a board from SparkFun!
  ZED-F9P RTK2: https://www.sparkfun.com/products/15136

  Hardware Connections:
  Plug a Qwiic cable into the GNSS and a BlackBoard
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 115200 baud to see the output
*/

#include <Wire.h> //Needed for I2C to GNSS

#include <SparkFun_u-blox_GNSS_v3.h> //http://librarymanager/All#SparkFun_u-blox_GNSS_v3
SFE_UBLOX_GNSS myGNSS;

void GPSSetup()
{
  Wire.begin();

  if (myGNSS.begin(Wire) == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(5, VAL_LAYER_RAM); //Set output to 5 times a second. Change the RAM layer only - not BBR

  byte rate;
  myGNSS.setNavigationFrequency(20);
  if(myGNSS.getNavigationFrequency(&rate)) //Get the update rate of this module
  {
    Serial.print("Current update rate: ");
    Serial.println(rate);
  }
  else
  {
    Serial.println("getNavigationFrequency failed!");
  }
}

int32_t latitude = 0;
int8_t latitudeHp = 0;
int32_t longitude = 0;
int8_t longitudeHp = 0;
uint32_t accuracy = 0;
byte fixType = 0;

void getGPSData()
{
  //Query module. The module only responds when a new position is available
  if (myGNSS.getHPPOSLLH())
  {
    // getHighResLatitude: returns the latitude from HPPOSLLH as an int32_t in degrees * 10^-7
    // getHighResLatitudeHp: returns the high resolution component of latitude from HPPOSLLH as an int8_t in degrees * 10^-9
    // getHighResLongitude: returns the longitude from HPPOSLLH as an int32_t in degrees * 10^-7
    // getHighResLongitudeHp: returns the high resolution component of longitude from HPPOSLLH as an int8_t in degrees * 10^-9
    // getElipsoid: returns the height above ellipsoid as an int32_t in mm
    // getElipsoidHp: returns the high resolution component of the height above ellipsoid as an int8_t in mm * 10^-1
    // getMeanSeaLevel: returns the height above mean sea level as an int32_t in mm
    // getMeanSeaLevelHp: returns the high resolution component of the height above mean sea level as an int8_t in mm * 10^-1
    // getHorizontalAccuracy: returns the horizontal accuracy estimate from HPPOSLLH as an uint32_t in mm * 10^-1

    // If you want to use the high precision latitude and longitude with the full 9 decimal places
    // you will need to use a 64-bit double - which is not supported on all platforms

    // To allow this example to run on standard platforms, we cheat by converting lat and lon to integer and fractional degrees

    // The high resolution altitudes can be converted into standard 32-bit float

    // First, let's collect the position data
    // Serial.print(myGNSS.getHorizontalAccuracy());
    // Serial.print(" ");
    latitude = myGNSS.getHighResLatitude();
    latitudeHp = myGNSS.getHighResLatitudeHp();
    longitude = myGNSS.getHighResLongitude();
    longitudeHp = myGNSS.getHighResLongitudeHp();
    accuracy = myGNSS.getHorizontalAccuracy();
    fixType = myGNSS.getFixType();
  }
}

