/*
 * gpio.c
 *
 *  Created on: 23 de out de 2019
 *      Author: igor
 */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "gpio.h"

ssize_t Bytes;
char Buffer[3];
char direcao[35];
char valor[35];

int readPin(int pino) {

	int ARQUIVO;

	snprintf(valor, 35, "/sys/class/gpio/gpio%d/value", pino);
	ARQUIVO = open(valor, O_RDONLY);
	if (ARQUIVO == -1) {
		printf("Falha ao abrir Value");
		close(ARQUIVO);
		return -1;
	}

	read(ARQUIVO, Buffer, 3);

	close(ARQUIVO);

	return (atoi(Buffer));

}

void exportPin(int pino) {

	int ARQUIVO;

	ARQUIVO = open("/sys/class/gpio/export", O_WRONLY);
	if (ARQUIVO == -1) {
		printf("Programa falhou ao abrir o arquivo de exportação");
		close(ARQUIVO);
		return;
	}

	Bytes = snprintf(Buffer, 3, "%d", pino);

	write(ARQUIVO, Buffer, Bytes);
	close(ARQUIVO);

}

void direction(int pino, int direction) {
	int ARQUIVO;

	snprintf(direcao, 35, "/sys/class/gpio/gpio%d/direction", pino);
	ARQUIVO = open(direcao, O_WRONLY);

	if (ARQUIVO == -1) {
		printf("Programa falhou ao abrir o arquivo de direcao");
		close(ARQUIVO);
		return;
	}

	if (direction) {
		if (write(ARQUIVO, "out", 3) == -1) {
			printf("Programa falhou ao configurar direcao");
			close(ARQUIVO);
			return;
		}
	} else {
		if (write(ARQUIVO, "in", 2) == -1) {
			printf("Programa falhou ao configurar direcao");
			close(ARQUIVO);
			return;
		}
	}

	close(ARQUIVO);

}

void unport(int pino) {
	int ARQUIVO;

	ARQUIVO = open("/sys/class/gpio/unexport", O_WRONLY);
	if (ARQUIVO == -1) {
		printf("Programa falhou ao abrir o arquivo de exportação");
		close(ARQUIVO);
		return;
	}
	Bytes = snprintf(Buffer, 3, "%d", pino);

	write(ARQUIVO, Buffer, Bytes);
	close(ARQUIVO);

}

void setValue(int pino, int value) {
	int ARQUIVO;
	snprintf(valor, 35, "/sys/class/gpio/gpio%d/value", pino);
	ARQUIVO = open(valor, O_WRONLY);
	if (ARQUIVO == -1) {
		printf("Programa falhou ao abrir o arquivo de valor");
		close(ARQUIVO);
		return;
	}

	switch (value) {
	case 1:
		if (write(ARQUIVO, "1", 1) == -1) {
			printf("Programa falhou ao configurar valor");
			close(ARQUIVO);
			return;
		}
		break;
	case 0:
		if (write(ARQUIVO, "0", 1) == -1) {
			printf("Programa falhou ao configurar valor");
			close(ARQUIVO);
			return;
		}
		break;
	default:
		printf("Valor invalido");
		close(ARQUIVO);
		return;
		break;
	}

	close(ARQUIVO);
	return;
}
