#include "Arduino.h"
#include <WiFi.h>
#include <config.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define ledPin 2
#define soilPin 36

//define environment variables
const char ssid[] = SSID;
const char wifi_password[] = WIFI_PASSWORD;

//Instantiate Wireless Clients
WiFiClientSecure secureClient = WiFiClientSecure();
PubSubClient mqttClient(secureClient);

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

void connectToWifi()
{
    Serial.print("Connecting to Wifi");
    Serial.print(SSID);
    WiFi.begin(SSID, wifi_password);
    while (WiFi.status != WL_CONNECTED)
    {
        Serial.print("Not connected..");
        delay(5000);
    }

    Serial.println("Wifi Connected.")
}

void connectToAWSIoTCore()
{
    //Setup MQTT Connection to AWS IoT Core
    mqttClient.setServer(AWS_END_POINT, 8883);
    secureClient.setCACert(AWS_PUBLIC_CERT);
    secureClient.setCertificate(AWS_DEVICE_CERT);
    secureClient.setPrivateKey(AWS_PRIVATE_KEY);
    Serial.println("Connecting to MQTT IoT Core...");

    mqttClient.connect(DEVICE_NAME);

    //Check if MQTT Client is connected
    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT IoT Core...");
        mqttClient.connect(DEVICE_NAME);
        delay(5000);
    }

    //If connected
    Serial.println("MQTT Connected.");
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
