/*
#the approximate moisture levels for the sensor reading
# 0 to 300 dry soil
# 300 to 700 humid soil
# 700 to 950 in water
*/

#include "Arduino.h"

#ifndef ledPin
#define ledPin 2
#endif

#define sensorPin 36
int trigger = 300; // set the moisture level

void setup()
{
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
    Serial.println(analogRead(sensorPin) / 3.3); // read the value from the sensor
    float sensor_reading = (analogRead(sensorPin) / 3.3);
    if (sensor_reading >= trigger)
    {
        digitalWrite(ledPin, HIGH); // turn on the LED
    }
    else
    {
        digitalWrite(ledPin, LOW); // turn off LED
    }
    delay(200);
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
    Serial.println(analogRead(sensorPin));
    delay(100);
}