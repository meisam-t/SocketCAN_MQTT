
#ifndef SUB_H 
#define SUB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

void deliveredSub(void *context, MQTTClient_deliveryToken dt);
int msgarrvdSub(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlostSub(void *context, char *cause);
int subData();

#endif
