#ifndef DHT_H
#define DHT_H

#include <stdio.h>
#include <inttypes.h>
#include "wiringPi.h"
#include "../gpio/gpio.h"

#define MAXTIMINGS	85
#define DHTPIN  7



void read_dht11_dat();

#endif
