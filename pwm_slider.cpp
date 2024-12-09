#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "util.h"
#include "PWM.h"
using namespace std;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "Beagle1"
#define TOPIC       "CayHunt12/feeds/project.sliding"
#define AUTHMETHOD  "CayHunt12"
#define AUTHTOKEN   "aio_pQgY26FAQcLz0GR40JVzO3wIPRDz"
#define QOS         1
#define TIMEOUT     10000L
#define GPIO_PATH    "/sys/class/gpio/gpio48/"

// Use this function to control the LED
void writeGPIO(string filename, string value){
   fstream fs;
   string path(GPIO_PATH);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

exploringBB::PWM pwmLED("pwm-0:0");

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr = (char*) message->payload;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    
    if(strcmp(topicName, TOPIC) == 0) {
      float brightness = atof(payloadptr);
      if(brightness >= 0.0f && brightness <= 100.0f) {
        printf("Setting brightness to %.2f%%\n", brightness);
        pwmLED.setDutyCycle(brightness); // Adjust PWM duty cycle

        if (brightness == 0.0f) {
          writeGPIO("value", "0");
        } else {
          writeGPIO("value", "1");
        }
      } else {
        printf("Invalid brightness value: %.2f\n", brightness);
      }
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
  printf("\nConnection lost\n");
  printf("       cause: %s\n", cause);
}

int main(int argc, char* argv[]) {

    pwmLED.setPeriod(20000000);
    pwmLED.setDutyCycle(0.0f);
    pwmLED.run();

    writeGPIO("direction", "out");
    exploringBB::write("value", "filename", "0");

    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

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

