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
#include "sub.h"
#include "pubcanmqtt.h"

//コマンド引数処理
#define MQTT_SERVER "192.168.13.101"

//MQTT QoS
int mqtt_pub_qos = 0;
int mqtt_sub_qos = 1;
//Robofork Serial Number
const char *SerialNumber = "3";
const char *robo="Robofork/";
const char *tor ="/toR";
const char *tos = "/toS";

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

//処理対象の受信CAN - IDリスト
//can_id_list = [] #MQTTへPublishしないVersion
//can_id_list[] = { "401", "402", "403", "1F0", "27D", "270", "27F", "1A0", "4A0", "169", "110", "119", "180", "190" };
//間引かない受信CAN - IDリスト
//always_send_can_id_list[] = {"270", "27F", "4A0"};

//MQTT Data Set

// TODO
// change "3" to its variable name 





const char *topic_toR = "Robofork/" "3" "/toR";  
 //From Server to Robofork topic_toS = "Robofork/" + SerialNumber + "/toS" #From Robofork to Server

const char *topic_toS = "Robofork/" "3" "/toS"; //From Robofork to Server

//過剰送信を防ぐために、同じCAN - ID / Dataは間引く。その判定用の保存データ。
                                                                                                                      
char *saved_sent_can_data;

//CAN Data Set
char channel[] = "slcan0"; //Openblocks->"slcan0"

char bustype[] = "socketcan_native";

int main(int argc, char **argv)

{

   // initializing the data for subscriber // 
	// this part is the same as ./pub 
SerialNumber = argv[2];
	

	char topics[200];
	snprintf(topics,sizeof(topics),"%s%s%s",robo,SerialNumber,tor);
	if (argc > 2) {
		printf("Args [%d] = %s ",argc,argv[4]);        
			MQTT_SERVER = argv[2];
	}

// copy to array // list of can IDs  
	for (int j=0; j<atoi(argv[6]); j++ ){
		printf("\nCAN ID %s \n", argv[j+8]);
		can_id_list[j] = argv[j+8];
	}

/// 
	for (int j=0; j<atoi(argv[23]); j++ ){
		printf("\nAlways CAN ID %s \n", argv[j+25]);
		always_send_can_id_list[j] = argv[j+25];
	}

// read the values test 
	for (int i =0; i < 14; i++){
		printf(" canlist[%d] = %s", i, can_id_list[i]);
	}
	puts("\n");

	for (int i =0; i < 3; i++){
		printf(" canlist[%d] = %s", i, always_send_can_id_list[i]);
	}

///
	printf("\nMQTT server's eastablished on: %s \n",MQTT_SERVER);
	char *s = " this is a test to see if the data can be transfered";
	printf("%s", topics);	
	int can_socketid = can_init();



//
	int ret=subData();
    return 0;
}
