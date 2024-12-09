#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "GPIO.h"

using namespace std;
using namespace exploringBB;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "Beagle1"
#define TOPIC       "CayHunt12/feeds/project.led-toggle"
#define AUTHMETHOD  "CayHunt12"
#define AUTHTOKEN   "aio_pQgY26FAQcLz0GR40JVzO3wIPRDz"
#define QOS         1
#define TIMEOUT     10000L
#define LED_PIN     48

GPIO led(LED_PIN);

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char payload[message->payloadlen + 1];
    strncpy(payload, (char*)message->payload, message->payloadlen);
    payload[message->payloadlen] = '\0';

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   payload: %s\n", payload);
    
    if (strcmp(payload, "ON") == 0) {
      led.setValue(HIGH);
      cout << "Appliance is ON" << endl;
    } else if (strcmp(payload, "OFF") == 0) {
      led.setValue(LOW);
      cout << "Appliance is OFF" << endl;
    } else {
      printf("Unrecognized payload: %s\n", payload);
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}    

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    led.setDirection(OUTPUT);


    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

