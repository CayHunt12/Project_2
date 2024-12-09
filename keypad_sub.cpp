#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "LED.h"

using namespace std;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "Beagle1"
#define TOPIC       "CayHunt12/feeds/project.keypad"
#define AUTHMETHOD  "CayHunt12"
#define AUTHTOKEN   "aio_pQgY26FAQcLz0GR40JVzO3wIPRDz"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    cout << "Message with Token value " << dt << "delivery confirmed" << endl;
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message* message) { 
    char* payloadptr = (char*)message->payload;
    float blinkCount = atoi(payloadptr);
    cout << "Recieved Number: " << blinkCount << endl;

    LED usr3(3);
    usr3.blink(blinkCount, 500);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}    


void connlost(void *context, char *cause) {
    cout << "Connection Lost\nCause: " << cause << endl;
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        cout << "Failed to connect, return code " << rc << endl;
        return -1;
    }
    
    cout << "Subscribed to topic: " << TOPIC << endl;
    MQTTClient_subscribe(client, TOPIC, QOS);
    
    cout << "Press 'Q' or 'q' and <Enter> to quit" << endl;
    
    char ch;
    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

