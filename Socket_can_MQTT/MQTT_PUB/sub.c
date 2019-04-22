#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include "sub.h"
#include "Cansendrecv.h"
#include <cjson/cJSON.h>
#define ADDRESS     "tcp://localhost:1883" //"tcp://192.168.13.101:1883" 
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "Robofork/3/toS" // toR
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
char *topics;
volatile MQTTClient_deliveryToken deliveredtoken;
void deliveredSub(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}
int msgarrvdSub(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	char* payloadptr;
	char* payloadData;
	char *copyStr[100];
	printf("Message arrived\n");
	printf("     topic Name: %s\n", topicName);
	printf("   message: " );
	payloadptr = message->payload;
	for(i=0; i<message->payloadlen; i++)
	{
		//printf("Data ...--- >%s",payloadptr++);
		
		putchar(*payloadptr++);
	}
	printf("\n");
	payloadptr = message->payload;
	int count=0; 
	while(*payloadptr != '\0') {
   		 printf("Data ...--- >%c", *payloadptr);
		 copyStr[count] = payloadptr;
    // move the ptr pointer to the next memory location
    payloadptr++;
  	count ++;
		
	}
	putchar('\n');
	printf(" \n Copy str value is :  . . . %s \n", *copyStr);
	//sprintf(copyStr,"%s",&payloadptr);
//	snprintf(payloadData,sizeof(payloadptr),"%s", payloadptr);

/// can send 
	can_send(1,1,*copyStr);
	printf(" Can sent Data Has been alread sent . . .\n");

//////// Can send data                    
	putchar('\n');
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}
void connlostSub(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}
int subData()
{
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;
	MQTTClient_create(&client, ADDRESS, CLIENTID,
					  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	MQTTClient_setCallbacks(client, NULL, connlostSub, msgarrvdSub, deliveredSub);
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
		   "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);
	do
	{
		ch = getchar();
	} while(ch!='Q' && ch != 'q');
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}
