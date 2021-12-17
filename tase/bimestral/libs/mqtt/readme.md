# MQTT lib

## Constants

`CLIENTID` is the identifier of the client
`HOST` is the IP of broker MQTT

## Functions

### publish

```c
void publish(MQTTClient client, char *topic, char *payload)
```

Publishes a message to a specific topic.

### onMessage

```c
int onMessage(void *context, char *topicName, int topicLen, MQTTClient_message *message)
```

This function is triggered when a specif topic receives a message. It works using interruptions.
*Based on the default topic defined on setup*

### setup

```c
int setupMQTT(MQTTClient *client, char *address, char *subscribeTopic)
```

Setup the MQTT to connect to a specific host and a specific topic.

| Retorno | Significado |
| --- |  ---| 
| 1 | Operação realizada com sucesso |
| -1 | Ocorreu um erro na operação |