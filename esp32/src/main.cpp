#include "Arduino.h"
#include <WiFi.h>
#include <config.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//define ESP32 Pins
#define ledPin 2
#define soilPin 36

//Instantiate Wireless Clients
WiFiClientSecure secureClient = WiFiClientSecure();
PubSubClient mqttClient(secureClient);

//Constants
const int MQTT_PORT = 8883;
const char PLANT_TOPIC[] = "plants/";

// Value for 0RH: 2450
// Value for 100RH 900
const int AirValue = 2450;
const int WaterValue = 900;
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;

void publishPlantState(String moistureStatus)
{
    // create 200 byte json payload
    StaticJsonDocument<200> jsonDocument;
    JsonObject eventDoc = jsonDocument.createNestedObject("event");
    eventDoc["moistureStatus"] = moistureStatus;
    char payload[200];
    serializeJson(eventDoc, payload);

    mqttClient.publish(PLANT_TOPIC, payload);
}

// callback function for when a message is dequeued from the MQTT server
void callback(char *topic, byte *payload, unsigned int length)
{
    // print message  for debugging
    Serial.print("Message arrived: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println("--");
}

String getCurrentMoistureStatus()
{
    Serial.println("Moisture Sensor Value:");
    //TODO: Refine reading of sensor data e.g. after current
    Serial.println(analogRead(soilPin)); // read the value from the sensor
    soilMoistureValue = analogRead(soilPin);
    String moistureStatus;

    if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
    {
        Serial.println("Very Wet Soil");
        moistureStatus = "Very Wet Soil";
    }
    else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
    {
        Serial.println("Wet Soil");
        moistureStatus = "Wet Soil";
    }
    else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
    {
        Serial.println("Dry Soil");
        moistureStatus = "Dry";
    }
    return moistureStatus;
}

void connectToWifi()
{
    Serial.print("Connecting to Wifi");
    WiFi.begin(SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Not connected..");
        delay(5000);
    }

    Serial.println("Wifi Connected.");
}

void connectToAWSIoTCore()
{
    //Setup MQTT Connection to AWS IoT Core
    mqttClient.setServer(AWS_END_POINT, MQTT_PORT);
    mqttClient.setCallback(callback);
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
        // Wait 5 seconds before retrying
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

void setup()
{
    //Set Baud rate to 115200
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    //init LED digital pin as an output
    pinMode(ledPin, OUTPUT);
    Serial.println("ESP32 RUNNING");

    //Start WiFi Connection
    connectToWifi();

    //Start MQTT Connection
    connectToAWSIoTCore();
}

void loop()
{
    String moistureStatus = getCurrentMoistureStatus();
    publishPlantState(moistureStatus);

    delay(10000);
}
