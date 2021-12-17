#include <stdio.h>
#include "wiringPi.h"
#include <pthread.h>

void * proc(void *t) {	
	
	int *j = (int *)t;
	
	while (1) {
		usleep(250000);
		printf("\nValor %d: %d", *j, digitalRead(*j));
	
	}
}

int main(int argc, char **argv)
{
	pthread_t thread[2];
	
	int i[5] = {23, 24, 25, 29, 28};
	
	pthread_create(&thread[0], NULL, proc, (void *) &i[3]);
	pthread_create(&thread[1], NULL, proc, (void *) &i[4]);
		
	wiringPiSetup();
	
	pinMode(23, OUTPUT); // 35 c 
	pinMode(24, OUTPUT); // 37 b 
	pinMode(25, OUTPUT); // 39 a
	pinMode(29, INPUT); // 40
	pinMode(28, INPUT); // 38
	
	while (1) {
		
		usleep(250000);
		digitalWrite(23, LOW);
		digitalWrite(24, LOW);
		digitalWrite(25, LOW);
		usleep(1000000);
		digitalWrite(23, LOW);
		digitalWrite(24, LOW);
		digitalWrite(25, HIGH);
	}
	
	return 0;
}

