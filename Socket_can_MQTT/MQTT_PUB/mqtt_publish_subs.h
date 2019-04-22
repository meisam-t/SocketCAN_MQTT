#ifndef MQTT_PUBLISH_SUBS_H //mqtt_publish_subs
#define MQTT_PUBLISH_SUBS_H
#include <MQTTClient.h>

int mqtt_pub(char * topic , char* data);
void *mqtt_sub();
void on_connect(char *client, char* userdata, int flags, int rc);
void on_message(char *client, char* userdata, char *msg);
void get_log_timestamp();
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlost(void *context, char *cause);

void *mqtt_sub_thread(void *t_id);
void *mqtt_pub_thread(void *t_id);




#endif




