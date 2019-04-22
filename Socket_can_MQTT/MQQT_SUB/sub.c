#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include "sub.h"
#include "Cansendrecv.h"

#define ADDRESS     "tcp://localhost:1883" //"tcp://192.168.13.101:1883" 
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "Robofork/3/toS" // toR
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

void append(char* s, char c);
unsigned char data[8];
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

volatile MQTTClient_deliveryToken deliveredtoken;
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}



char* stringToBinary(char s[16]) {
    if(s == NULL)
		return 0; 
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    printf(" binary %s", binary);
    return binary;
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
	can_send(1,123,can_send_data);
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
	printf(" msg 1 is----------------------------------------------->  %c\n\n",(*can_msg_in_cpy)[1]);
		//while(*can_msg_in != '\0')  {
		for (k=0; k< strlen( can_msg_in); k++){ 
			printf(" Messasge  %c \n", (*can_msg_in)[k]);
			// getting the serial number 
			if ((*can_msg_in)[k]== '#'){
				if (first_sharp == -1){
					first_sharp = k;
					can_SerialNumber=(*can_msg_in)[k-1];
					printf(" first sharp has been found at --------- > %d \n\n", k);
				}
				else
				{
					second_sharp = k;
					int z=0;
					//getting the CAN ID
					for (int p =first_sharp+1; p<second_sharp;p++){
						can_ID[z]=(*can_msg_in)[p];
						z++;
					}
					printf(" second  sharp has found at --------- > %d\n\n", second_sharp);
				 }
				 
		
		}
		ct++;
		int c_count =0;
		//getting the Can data
		for (int k=second_sharp+1; k< strlen(can_msg_in_cpy)-1; k++){
			// printf("Temps%s", stringToBinary(&(*can_msg_in)[k]));
			 char  *temp1 = stringToBinary(&(*can_msg_in)[k]);
			 char  *temp2 = stringToBinary(&(*can_msg_in)[k+1]);
			// printf("Temps %s/t%s", temp1, temp2);
			 strcat(temp1,temp2);
			 
			 char c = strtol(temp1, 0, 2);
			 can_send_data[c_count] = c;
			c_count ++;
			//printf("Temps %s", &(*can_msg_in)[k]);
			//break;
			
		}
		
	}
	printf(" CAN SEND DATA -----******************************--->>>>>>------ > %s \n\n", &can_send_data);
	printf(" Serial Number is  --->>>>>>------ > %c \n\n", can_SerialNumber);
	int c=0;
	printf(" ID is -====>>>> %s\n",can_ID); 
	printf(" Number o f data  %d", k);
	
	

	
	
}

	
