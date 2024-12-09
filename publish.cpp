#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "GPIO.h"

using namespace std;

//Please replace the username, feed and token with your values
#define ADDRESS    "tcp://io.adafruit.com"
#define CLIENTID   "Beagle1"
#define TOPIC      "CayHunt12/feeds/project.button"
#define AUTHMETHOD "CayHunt12"
#define AUTHTOKEN  "aio_pQgY26FAQcLz0GR40JVzO3wIPRDz"
#define QOS        1
#define TIMEOUT    10000L

#define BUTTON_PIN 60

using namespace exploringBB;

MQTTClient client;
MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

void publishStatus(bool status) {
  char str_payload[100];
  sprintf(str_payload, "%d", status);
  pubmsg.payload = str_payload;
  pubmsg.payloadlen = strlen(str_payload);
  pubmsg.retained = false;

  int rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
  if (rc == MQTTCLIENT_SUCCESS) {
    cout << "Message sent: " << str_payload << endl;
  } else {  
    cout << "Failed to send message. Return code: " << rc << endl;
  }
}

bool ledState = false;

int buttonCallback(int value) {
      cout << "Button state: " << value << endl;

      ledState = !ledState;
      cout << "LED state set to: " << (ledState ? "ON" : "OFF") << endl;
      publishStatus(ledState);
      return 0;
}

int main(int argc, char* argv[]) {
   MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
   opts.keepAliveInterval = 20;
   opts.cleansession = 1;
   opts.username = AUTHMETHOD;
   opts.password = AUTHTOKEN;
   
   int rc;
   if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
      cout << "Failed to connect, return code " << rc << endl;
      return -1;
   }

   GPIO button(BUTTON_PIN);
   button.setDirection(INPUT);
   button.setEdgeType(BOTH);
   button.setDebounceTime(300);
   
   button.waitForEdge(buttonCallback);

   while (true) {
    //MQTTClient_yield();
    sleep(1);
   }

   MQTTClient_disconnect(client, 10000);
   MQTTClient_destroy(&client);
   return rc;
}

