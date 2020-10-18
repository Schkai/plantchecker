#!usr/bin/env python
import paho.mqtt.client as mqtt

# for local testing purposes
client = mqtt.Client()
client.connect("0.0.0.0", 1883, 60)

client.publish("test/temperature", "25")
client.disconnect()