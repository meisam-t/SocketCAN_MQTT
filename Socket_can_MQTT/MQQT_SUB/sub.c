#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include "sub.h"
#include "Cansendrecv.h"
#include "hextobin.h"

#define ADDRESS     "tcp://localhost:1883" //"tcp://192.168.13.101:1883" 
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "Robofork/3/toS" // toR
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

void append(char* s, char c);
unsigned char data[16];
char *topics;
char can_ID[3];
char *can_send_data;
char *csd;
char *can_SerialNumber;
int first_sharp = -1;
int second_sharp = -1;
int ct=0;
char d1[2];
char d2[2];
char d3[2];
char dumb[20]="";
int c_count =0; 
//char tmp[20][20] = {"",""};

volatile MQTTClient_deliveryToken deliveredtoken;
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void deliveredSub(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}
int msgarrvdSub(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	//payloadptr = NULL;
	int i=0;
	unsigned char* payloadData;
	//payloadData = malloc(100 * sizeof(*payloadData));
	unsigned char* payloadptr;
	//payloadptr = malloc(100 * sizeof(*payloadptr));
	unsigned char copyStr[100]={'\0'};
	printf("Message arrived\n");
	printf("     topic Name: %s\n", topicName);
	printf("   message: " );
	payloadptr = message->payload;
	for(i=0; i<message->payloadlen; i++)
	{
		copyStr[i] = *payloadptr++;
 		//putchar(*payloadptr++);
	}
	printf("\n");
	printf("copystr [0] %d, -=========> %c", i,&copyStr[0]);
	int count=0;

	
	putchar('\n');
	ret_can_id(copyStr);
	//printf(" \n str [6]:  . . . %c \n",copyStr[6]);

/// can send 
	//can_send(1,123,can_send_data);
	printf(" Can sent Data Has been alread sent . . .\n");

///
//////// Can send data  
	//free(payloadData);	
															
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


// getting can ID , Serial Number and Can data seperately
void ret_can_id(char (*can_msg_in)[]){
	//char (*can_msg_in)[] = *can_msg_in;
	can_send_data = malloc(16 * sizeof(char));
	char (*can_msg_in_cpy)[] = *can_msg_in;
	perror("Ret Can ID ");
	int k=0;
	int r;
	printf(" msg 1 is---------------------str len message %d -------------------------->  %c\n\n",strlen(can_msg_in),(*can_msg_in)[1]);
		//while(*can_msg_in != '\0')  {
		for (k=0; k< strlen(can_msg_in); k++){ 
			printf(" Messasge at %d  %c \n", k, (*can_msg_in)[k] );
			// getting the serial number 
			if ((*can_msg_in)[k]== '#'){
				perror("first if \n");
				if (first_sharp == -1){
					perror("second if\n");
					first_sharp = k;
					can_SerialNumber= &(*can_msg_in)[k-1];
					
					printf(" first sharp has been found at --------- > %d \n\n", k);
				}
				else
				{
					perror("else \n");
					second_sharp = k;
					int z=0;
					//getting the CAN ID
					for (int p =first_sharp+1; p<second_sharp;p++){
						can_ID[z]=(*can_msg_in)[p];
						z++;
					}
					printf(" second  sharp has found at --------- ---:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::> %d can id %s\n\n", second_sharp, can_ID);
				 }
				 
		
		}else {
		
		
		int sc= (int)second_sharp +1;
		//getting the Can data
		size_t np = sizeof(dumb);
		memset(dumb, 0, np*sizeof(dumb[0]));
		if (k> 5){
		for (k=6; k< strlen(can_msg_in_cpy); k++){
			//printf("Temps ----->>>>>-----%s", (*can_msg_in)[k]));
			//printf("second sharp %d ", sc);
			
			//if ( k > second_sharp) {
				
				dumb[c_count] = (*can_msg_in_cpy)[k];
				printf(" Dumb value[%d] = %s ========= >>>>> \n",c_count, &dumb[c_count]);
				c_count ++;
		//	}
			
		}
		}
	}
}
	hextobin(dumb,c_count);
	c_count = 0;
	printf(" CAN SEND DATA -----******************************--->>>>>>------ > %s \n\n", dumb);
	printf(" Serial Number is  %c--->>>>>>------ >  \n\n", *can_SerialNumber);
	int c=0;
	printf(" CAN ID is -====>>>> %s\n\n",can_ID); 
	printf(" Number o f data  %d \n", k);
	
	

	
	
}

	
