#ifndef PUBCANMQTT_H 
#define PUBCANMQTT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

static char *MQTT_SERVER ="192.168.13.101";
static char *SerialNumber = "3";
static char *always_send_can_id_list[20] ={"270", "27F", "4A0"};
static char *can_id_list[20] = { "401", "402", "403", "1F0", "27D", "270", "27F", "1A0", "4A0", "169", "110", "119", "180", "190" };


#endif
