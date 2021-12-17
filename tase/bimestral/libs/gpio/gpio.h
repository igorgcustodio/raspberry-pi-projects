/*
 * gpio.h
 *
 *  Created on: 23 de out de 2019
 *      Author: igor
 */

#ifndef LIB_GPIO_H_
#define LIB_GPIO_H_

#define OUTPUT 1
#define INPUT 0

#define HIGH 1
#define LOW 0

void exportPin(int pino);
void direction(int pino, int direction);
void unport(int pino);
void setValue(int pino, int value);
int readPin(int pino);

#endif /* LIB_GPIO_H_ */
