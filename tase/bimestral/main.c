#include <stdio.h>
#include "libs/mqtt/mqtt.h"
#include "libs/db/db.h"
#include "libs/dht/dht.h"
#include "libs/gpio/gpio.h"

#define LED 25

void initLed()
{
    exportPin(LED);
    direction(LED, OUTPUT);
    setValue(LED, LOW);
}

void setLed()
{
    setValue(LED, HIGH);
}

void clearLED()
{
    setValue(LED, HIGH);
}

MQTTClient client;

void setup()
{
    setupMQTT(&client, HOST, "sub");
}

int main()
{
    //setup();

    //publish(client, "lyncon", "kashdkajhsd");

    char *sql = "SELECT * from COMPANY;";

    int c = exec(sql);

    printf("\nRetorno: %d\n", c);

    wiringPiSetup();

    initLed();

    while (1)
    {
        read_dht11_dat();
        setValue(LED, !readPin(LED));
        delay(1000);
    }

    return 0;
}