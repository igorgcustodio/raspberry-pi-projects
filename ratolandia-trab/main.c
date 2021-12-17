#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>	
#include "wiringPi.h"
#include <pthread.h>
#include <GL/glut.h>
#include <locale.h>

#define RATOS 4
#define A 29
#define B 28
#define C 27

/* Variáveis globais - Progresso e tempo */
int horas, minutos, segundos, total;
int progressoBarraTempo = 10;

/* Variáveis globais - Ratos */
float progressoRatos = 1.0;
float porcentagemRatos[RATOS];
int movimentoRatos[RATOS];

/* Variáveis globais - Pinos */
const int pinoC[8]={LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};
const int pinoB[8]={LOW,LOW,HIGH,HIGH,LOW,LOW,HIGH,HIGH};
const int pinoA[8]={LOW,HIGH,LOW,HIGH,LOW,HIGH,LOW,HIGH};
const int muxPins[2] = {25, 24}; // atribuir demais pinos conforme necessidade

/* Variáveis globais */
int botaoCsvEstado = 1;
pthread_t thread[2];
int H_Thread;

/* Funções Glut */
void displayText( float x, float y, int r, int g, int b, char *string);
void displayTime();
void colors(int color, float *r, float *g, float *b);
void drawFields();
void drawButtonCSV();
void display(void);
void keyPressed(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);
void * ThreadProc(void* lpv);

/* Funções globais */
void * RatoProc(void *t);
void setup();
void exportCSV();

int main(int argc, char* argv[]){

	setlocale(LC_ALL, "");

	printf("\nQual será o tempo de atividade (digite como hh:mm:ss): ");
	scanf("%d:%d:%d", &horas, &minutos, &segundos);
	
	int i = 0;
	wiringPiSetup();
	
	digitalWrite(A, OUTPUT);
	digitalWrite(B, OUTPUT);
	digitalWrite(C, OUTPUT);
	
	digitalWrite(A, LOW);
	digitalWrite(B, LOW);
	digitalWrite(C, LOW);
	
	for (i = 0; i < (sizeof(muxPins) / sizeof(int)); i++) {
		digitalWrite(muxPins[i], INPUT);
	}
	
	for (i = 0; i < RATOS; i++) {
		porcentagemRatos[i] = 0.0;
		movimentoRatos[i] = 0;
	}

	total = segundos + (minutos * 60) + (horas * 3600);
	segundos = 0;
	minutos = 0;
	horas = 0;
	int progresso = (int) (880/total); // define em quanto a barra de progresso do tempo será incrementada
	
	progressoRatos = (float) (400/total); // define o valor de progresso para cada vez que o rato se mexer
	

	glutInit(&argc, argv);
	H_Thread = pthread_create(&thread[0], NULL,	ThreadProc,	NULL);
		
	#ifdef VERBOSE
	if (H_Thread != -1)
		printf("Thread GLUT Iniciada						[ 0K ] \n");
	else
		printf("Thread GLUT Iniciada						[ Não 0K ] \n");
	
	#endif
	
	H_Thread = pthread_create(&thread[1], NULL, RatoProc, NULL);
	
	#ifdef VERBOSE
	if (H_Thread != -1)
		printf("Thread Proc. dos Ratos Iniciada						[ 0K ] \n");
	else
		printf("Thread Proc. dos Ratos Iniciada						[ Não 0K ] \n");
	
	#endif
	
	while (total >= 0) {
		usleep(1000000);
		segundos += 1;
		if (segundos == 60) {
			segundos = 0;
			minutos += 1;
		}
		if (minutos == 60) {
			minutos = 0;
			horas += 1;
		}
		total -= 1;
		progressoBarraTempo += progresso;
	}
	
	usleep(1000000);
}

void displayText( float x, float y, int r, int g, int b, char *string) {
	int i = 0; 
	int j = strlen( string );
 
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	
	for(i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, string[i] );
	}
}

void displayTime() {
	char seg[32];
	char min[32];
	char hor[32];
	
	if (segundos < 10) {
		sprintf(seg, "0%d", segundos);
	}
	else {
		sprintf(seg, "%d", segundos);
	}
	if (minutos < 10) {
		sprintf(min, "0%d", minutos);
	}
	else {
		sprintf(min, "%d", minutos);
	}
	if (horas < 10) {
		sprintf(hor, "0%d", horas);
	}
	else {
		sprintf(hor, "%d", horas);
	}
	

	char string[100];
    strcpy(string, "Tempo total: ");
    strcat(string,hor);
    strcat(string,":");
    strcat(string,min);
    strcat(string,":");
    strcat(string,seg);
	
	glBegin(GL_QUADS);
	glColor3f(0.03f, 0.5f, 1.0f);

	glVertex2f( 150, 155);
	glVertex2f( 400, 155);
	glVertex2f( 400, 210);
	glVertex2f( 150, 210);
	glEnd();
	
	displayText( 155, 190, 1.0f, 1.0f, 1.0f, string);
	sprintf(string, "%d ratos em monitoramento", RATOS);
	displayText( 155, 160, 1.0f, 1.0f, 1.0f, string);
}

void colors(int color, float *r, float *g, float *b) {
	
	switch (color) {
		
		case 1: //green
			*r = 0.0f;
			*g = 1.0f;
			*b = 0.0f;
		break;
		case 2: //red
			*r = 1.0f;
			*g = 0.0f;
			*b = 0.0f;
		break;
		case 3: //blue
			*r = 0.0f;
			*g = 0.0f;
			*b = 1.0f;
		break;
		case 4: //orange
			*r = 1.0f;
			*g = 0.5f;
			*b = 0.0f;
		break;
		case 5: //white
			*r = 1.0f;
			*g = 1.0f;
			*b = 1.0f;
		break;
		case 6: //black
			*r = 0.0f;
			*g = 0.0f;
			*b = 0.0f;
		break;
		default: // gray
			*r = 0.5f;
			*g = 0.5f;
			*b = 0.5f;
		break;
		
	}
	
}

void drawFields() {
	
	int i = 0;
	int j = 0;
	int k = 0;
	int cor = 5;
	
	for (i = 0; i < RATOS; i++) {
	
		glBegin(GL_QUADS);
		float r = 0.0f, g = 0.0f, b = 0.0f;
		cor = (movimentoRatos[i] == 1) ? 5 : 6;
		colors(cor, &r, &g, &b);
		glColor3f(r, g, b);

		glVertex2f( 10, (660 - j));
		glVertex2f( 30, (660 - j));
		glVertex2f( 30, (640 - j));
		glVertex2f( 10, (640 - j));
		glEnd();
		
		char numberMouse[10];
		sprintf(numberMouse, "Rato %d", (i + 1));
		
		displayText( 50, (642 - j), 0.0, 0.0, 0.0, numberMouse);
		
		j += 30; // distancia entre cada label dos ratos
		
		glBegin(GL_QUADS);
		r = 0.0f, g = 0.0f, b = 0.0f;
		colors((i + 1), &r, &g, &b);
		glColor3f(r, g, b);

		glVertex2f( (200 + k), 250);
		glVertex2f( (220 + k), 250);
		glVertex2f( (220 + k), porcentagemRatos[i] + 251);
		glVertex2f( (200 + k), porcentagemRatos[i] + 251);
		glEnd();
		
		k += 30; // distancia entre cada barra dos ratos
		
	}
	
}

void drawButtonCSV() {
	
	glBegin(GL_QUADS);
	float r = 0.0f, g = 0.0f, b = 0.0f;
	
	if (botaoCsvEstado == 1) {
		colors(5, &r, &g, &b);
		glColor3f(r, g, b);
		colors(6, &r, &g, &b);
	} 
	else {
		colors(6, &r, &g, &b);
		glColor3f(r, g, b);
		colors(5, &r, &g, &b);
	}

	glVertex2f( 150, 100);
	glVertex2f( 400, 100);
	glVertex2f( 400, 150);
	glVertex2f( 150, 150);
	glEnd();
	
	displayText( 215, 118, r, g, b, "Exportar CSV");
}

void display(void) {
	
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.03f, 0.5f, 1.0f);

	glVertex2f( 10 , 10);
	glVertex2f( (progressoBarraTempo), 10);
	glVertex2f( (progressoBarraTempo), 50);
	glVertex2f( 10, 50);
	glEnd();

	displayText( 10.0f, 60.0f, 0.0, 0.0, 0.0, "Progresso");
	displayTime();
	drawFields();
	drawButtonCSV();
	glFlush();
	
}

void keyPressed(unsigned char key, int x, int y) {

}

void MouseFunc(int button, int state, int x, int y){

	#ifdef VERBOSE
	printf("\n%d - %d", x, y);
	#endif
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			if (x >= 150 && x <= 400 && y >= 550 && y <= 600){
				exportCSV();
				botaoCsvEstado = 2;
			}
		}
		if (state == GLUT_UP) {
			if (x >= 150 && x <= 400 && y >= 550 && y <= 600){
				botaoCsvEstado = 1;
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
		
		}
	}

}

void * ThreadProc(void* lpv) {
	
	glutInitWindowSize(900, 700);					// Determina o tamanho da janela gráfica
	glutInitWindowPosition(0, 20);					// Determina qual será a posição inicial da Janela
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	// Define o modo de operação da Janela Gráfica
	glutCreateWindow("Ratolandia");					// Cria uma janela com as configurações anteriores e com um determinado título
	glOrtho(0, 900, 0, 700, -1, 1);					// Define o espaço de desenho em uma matrix ortográfica
	glutDisplayFunc(display);						// Define a função que atualizará a tela
	glutKeyboardFunc(keyPressed);					// Define a função que tratará teclas de teclado
	glutMouseFunc(MouseFunc);						// Define a função que tratatá o uso do mouse
	glutIdleFunc(display);							// Inicia a função display

	while (total >= 0)
		glutMainLoop();									// Inicia o laço de desenho em tela

	pthread_exit(NULL);
}

void * RatoProc(void *t) {
		
	while (total >= 0) {

		/*for (i = 0; i < (sizeof(muxPins) / sizeof(int)); i++) {
			usleep(250000);
			digitalWrite(A, pinoA[i]);
			digitalWrite(B, pinoB[i]);
			digitalWrite(C, pinoC[i]);
			
			if (digitalRead(muxPins[i])) {
				porcentagemRatos[0] += progressoRatos;
				movimentoRatos[0] = 1;
				printf("\nRATO 1");
			}
			else {
				movimentoRatos[0] = 0;
			}
			
		}*/
		
		usleep(250000);
		
		digitalWrite(A, LOW);
		digitalWrite(B, LOW);
		digitalWrite(C, LOW);
		if (digitalRead(muxPins[0])) {
			porcentagemRatos[0] += progressoRatos;
			movimentoRatos[0] = 1;
			printf("\nRATO 1");
		}
		else {
			movimentoRatos[0] = 0;
		}
		if (digitalRead(muxPins[1])) {
			porcentagemRatos[2] += progressoRatos;
			movimentoRatos[2] = 1;
			printf("\nRATO 3");
		}
		else {
			movimentoRatos[2] = 0;
			
		}
		
		usleep(250000);
		digitalWrite(A, HIGH);
		digitalWrite(B, LOW);
		digitalWrite(C, LOW);
		
		if (digitalRead(muxPins[0])) {
			porcentagemRatos[1] += progressoRatos;
			movimentoRatos[1] = 1;
			printf("\nRATO 2");
		}
		else {
			movimentoRatos[1] = 0;
		}
		if (digitalRead(muxPins[1])) {
			porcentagemRatos[3] += progressoRatos;
			movimentoRatos[3] = 1;
			printf("\nRATO 4");
		}
		else {
			movimentoRatos[3] = 0;
		}
		
	}
	
	pthread_exit(NULL);
}

void exportCSV() {
	
	FILE *file;
	
	char path[32];
	int i;

	sprintf(path, "arquivos/relatorio-%dh-%dmin-%dseg.csv", horas, minutos, segundos );
	
	file = fopen(path, "w+");
	
	if (file == NULL) {
		printf("\nO arquivo não pode ser exportado.\n");
		exit(0);
	}
	
	int h = (int) total/3600;
	int m = (int) ((total - h) / 60);
	int s = (total - h - m);
	
	fprintf(file, "ID_RATO, %% do tempo total em movimento, , Tempo total: %d:%d:%d, Tempo já monitorado: %d:%d:%d,\n", h, m, s, horas, minutos, segundos);
	
	for (i = 0; i < RATOS; i++) {
		
		fprintf(file, "%d, %.1f%%\n", (i + 1), porcentagemRatos[i]);
	}
	
	fclose(file);
	
}
