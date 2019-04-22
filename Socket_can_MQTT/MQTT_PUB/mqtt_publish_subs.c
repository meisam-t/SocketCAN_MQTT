#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include "mqtt_publish_subs.h"
#include "Cansendrecv.h"
#include "MQTTClient.h"
#include <string.h>

#define True 1
//#define MAX_EVENTS 5
#define ADDRESS "tcp://localhost:1883"
#define CLIENTID "Client ID 1214"
#define TOPIC "MQTT Topic "
#define PAYLOAD "Payload message !"
#define QOS 1
#define TIMEOUT 10000L
#define MQTT_SERVER "192.168.13.101"

char *payload = "a payload";
int payloadlen = sizeof(payload);
int qos = 1;
int retained = 0;
int rc;
//MQTT decleations 
MQTTClient client;
MQTTClient_deliveryToken dt;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
volatile MQTTClient_deliveryToken deliveredtoken;


// Publish Mqtt messages 
int mqtt_pub(char *topic, char *data){
	//creating the client 
	rc = MQTTClient_create(&client, ADDRESS, NULL, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	pubmsg.payload = data;
	pubmsg.payloadlen = strlen(data);
	pubmsg.qos = qos;
	pubmsg.retained = retained;
	MQTTClient_publishMessage(client,TOPIC,&pubmsg,&dt);
	printf("Waiting for up to %d seconds for publication of %s\n"
		   "on topic %s for client with ClientID: %s\n",
		   (int)(TIMEOUT / 1000), PAYLOAD, TOPIC, CLIENTID);
	rc = MQTTClient_waitForCompletion(client, dt, TIMEOUT);
	printf("Message with delivery token %d delivered\n", dt);
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
};


//Subscriber 

void *mqtt_sub(){
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;
	MQTTClient_create(&client, ADDRESS, CLIENTID,
					  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	printf("Address: %s Subscribing to topic %s\nfor  %s using QoS%d\n\n"
		   "Press Q<Enter> to quit\n\n",
		   ADDRESS, TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);

	do
	{
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');

	MQTTClient_unsubscribe(client, TOPIC);
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	
};




void on_connect(char *client, char *userdata, int flags, int rc){

};
void on_message(char *client, char *userdata, char *msg){

};
void get_log_timestamp(){

};
void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	char *payloadptr;

	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");

	payloadptr = message->payload;
	for (i = 0; i < message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}



void *mqtt_sub_thread(void *t_id){
	
	mqtt_sub();
}
void *mqtt_pub_thread(void *t_id){
	
	mqtt_pub("Topic", "Data");
}


