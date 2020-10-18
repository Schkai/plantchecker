#!/usr/bin/env python
import paho.mqtt.client as mqtt
import sqlite3
from time import gmtime, strftime

temperature_topic = "temperature/#"
humidity_topic = "humidity/#"
database_file = "data.db"

def on_connect(client, userdata, flags, rc):
    print("Connected with code "+str(rc))
    #TODO: Implement correct wildcards and topics for different rooms
    client.subscribe("temperature_topic")
    client.subscribe("humidity_topic")

def on_message(client, userdata, msg):
    current_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
    
    write_to_database(msg.topic, current_time, msg.payload)


def write_to_database(topic, time, payload):
    print(topic, time, payload)
    conn = sqlite3.connect(database_file)
    c = conn.cursor()
    c.execute("INSERT INTO test VALUES (?,?,?)", (topic, time, payload))
    conn.commit()



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.subscribe("test/temperature")
client.loop_forever()