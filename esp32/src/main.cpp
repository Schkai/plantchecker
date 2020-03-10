#include "Arduino.h"
#include <WiFi.h>
#include <config.h>

#define ledPin 2
#define soilPin 36

//define environment variables
const char ssid[] = SSID;
const char wifi_password[] = WIFI_PASSWORD;

// Value for 0RH: 2450
// Value for 100RH 900
const int AirValue = 2450;
const int WaterValue = 900;
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;

void setup()
{
    //Set Baud rate to 115200
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    //init LED digital pin as an output
    pinMode(ledPin, OUTPUT);
    Serial.println("ESP32 RUNNING");
}

void loop()
{

    Serial.println("Moisture Sensor Value:");
    //TODO: Get correct reading of sensor data e.g. after current
    Serial.println(analogRead(soilPin)); // read the value from the sensor
    soilMoistureValue = analogRead(soilPin);

    if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
    {
        Serial.println("Very Wet Soil");
    }
    else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
    {
        Serial.println("Wet Soil");
    }
    else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
    {
        Serial.println("Dry Soil");
    }

    delay(500);
}

void switch_leds()
{
    //turn the LED on (HIGH is the voltage level)
    Serial.print("LED ON");
    digitalWrite(ledPin, HIGH);
    //wait for a second
    delay(1000);

    Serial.print("LED OFF");

    // turn the LED off by making the voltage low
    digitalWrite(ledPin, LOW);

    //wait for a second
    delay(1000);
}

void read_analog()
{
    Serial.println(analogRead(soilPin));
    delay(100);
}