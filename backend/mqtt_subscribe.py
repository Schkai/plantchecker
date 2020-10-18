#!/usr/bin/env python
import paho.mqtt.client as mqtt
import sqlite3
from time import gmtime, strftime

temperature_topic = "temperature/#"
humidity_topic = "humidity/#"


def on_connect(client, userdata, flags, rc):
    print("Connected with code "+str(rc))
    #TODO: Implement correct wildcards and topics for different rooms
    client.subscribe("temperature_topic")
    client.subscribe("humidity_topic")

def on_message(client, userdata, msg):
    current_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
    result = (current_time + "\t" + str(msg.payload))
    print(msg.topic + ":\t" + result)


def write_to_database(topic, time, temperature, humidity):
    pass



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.subscribe("test/temperature")
client.loop_forever()