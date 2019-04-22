#include <MQTTClient.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <signal.h>
#include "mqtt_publish_subs.h"
#include "Cansendrecv.h"
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "pub.h"
#include "pubcanmqtt.h"


//コマンド引数処理
//#define MQTT_SERVER "192.168.13.101"

//MQTT QoS
int mqtt_pub_qos = 0;
int mqtt_sub_qos = 1;
//Robofork Serial Number
//SerialNumber = "3";
char *robo="Robofork/";
char *tor ="/toR";
char *tos = "/toS";
int splitter(char *strargv);
//処理対象の受信CAN - IDリスト
//can_id_list = [] #MQTTへPublishしないVersion
//can_id_list[] = { "401", "402", "403", "1F0", "27D", "270", "27F", "1A0", "4A0", "169", "110", "119", "180", "190" };
//間引かない受信CAN - IDリスト
//always_send_can_id_list[] = {"270", "27F", "4A0"};

//MQTT Data Set

// TODO
// change "3" to its variable name 

int counter=1;

char* myarray[100];

const char *topic_toR = "Robofork/" "3" "/toR";  
 //From Server to Robofork topic_toS = "Robofork/" + SerialNumber + "/toS" #From Robofork to Server

const char *topic_toS = "Robofork/" "3" "/toS"; //From Robofork to Server

//過剰送信を防ぐために、同じCAN - ID / Dataは間引く。その判定用の保存データ。
                                                                                                                      
char *saved_sent_can_data;

//CAN Data Set
char channel[] = "slcan0"; //Openblocks->"slcan0"

char bustype[] = "socketcan_native";

int main(int argc, char *argv[])

{
	char topics[200];
	if (argc > 2) {
		SerialNumber = argv[2];
		snprintf(topics,sizeof(topics),"%s%s%s",robo,SerialNumber,tor);
		printf("Args [%d] = %s ",argc,argv[4]);        
			MQTT_SERVER = argv[2];


	int coun= splitter(argv[3]);
	printf(" \ncounter  %d" ,coun);
	for (int k=0; k<coun-1; k++ ){
		printf("\nCAN ID %s \n", myarray[k]);
		can_id_list[k] = myarray[k];
		}
	
	int coun2=splitter(argv[4]);
	printf(" \ncounter  %d" ,coun2);
	for (int k=0; k<coun2-1; k++ ){
		printf("\nCAN ID %s \n", myarray[k]);
		always_send_can_id_list[k] = myarray[k];
		}
	}
	else {
		
	//SerialNumber = "3";
	snprintf(topics,sizeof(topics),"%s%s%s",robo,SerialNumber,tor);
	printf("topic is : %s", topics);
	MQTT_SERVER = "192.168.13.101";
		
	}

	free(myarray[100]);
	
	printf("\nMQTT server's eastablished on: %s \n",MQTT_SERVER);
	char *s = " this is a test to see if the data can be transfered";
	int can_socketid = can_init();
while(1) {
	can_recv(can_socketid);
}
    return 0;
}




int splitter(char *strargv)
{ 
	counter =1; 
    const char s[4] = ","; 
    char* tok; 
    
  
    // Use of strtok 
    // get first token 
    tok = strtok(strargv, s); 
  	myarray[0] = tok;
    // Checks for delimeter 
puts(" we are here "); 

    while (tok != 0) { 
        printf("%s ", tok); 
  
        // Use of strtok 
        // go through other tokens 
        tok = strtok(0, s); 
		myarray[counter] = tok;
		counter++;
    } 
for (int j=0; j < counter-1; j++) {
	printf(" \ncounter  %d reault is : %s ",j, myarray[j]);
	
}
  return counter;
} 










