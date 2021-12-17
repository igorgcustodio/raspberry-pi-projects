#ifndef _H_MQTT
#define _H_MQTT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

#define CLIENTID "raspberry"
#define HOST "192.168.100.34"

/**
 * Publish a message to a topic
 * 
 * @param <MQTTClient>  client  the client variable
 * @param <char *>      topic   the topic to publish
 * @param <char *>      payload the message
 * 
*/
void publish(MQTTClient client, char *topic, char *payload);

/**
 * Triggered when receives a message
 * 
 * @param <void *>                  context     message's context
 * @param <char *>                  topicName   topic's context
 * @param <int>                     context     length of the topic
 * @param <MQTTClient_message *>    message     the message
 * 
 * @return int          1
*/
int onMessage(void *context, char *topicName, int topicLen, MQTTClient_message *message);

/**
 * Setup the MQTT client
 * 
 * @param <MQTTClient*>      client         the client
 * @param <char*>            address        the ip broker
 * @param <char*>            subscribeTopic the topic to subscribe
 * 
 * @return <int> 1 - setup successful, -1 - setup failed
 * 
*/
int setupMQTT(MQTTClient *client, char *address, char *subscribeTopic);

#endif