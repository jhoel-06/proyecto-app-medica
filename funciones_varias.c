#include "funciones_varias.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>      
#include <stdlib.h>
#include <conio.h>

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void pedirCadena(const char *mensaje, char *buffer, int tam) {
	printf("%s", mensaje);
	fgets(buffer, tam, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
}

void pedirContrasena(const char *mensaje, char *buffer, int tam) {
	char c;
	int i = 0;
	
	printf("%s", mensaje);
	
	while (1) {
		c = getch();   // No muestra el carácter
		// Enter
		if (c == 13) {
			buffer[i] = '\0';
			break;
		}
		// Backspace
		else if (c == 8 && i > 0) {
			i--;
			printf("\b \b");
		}
		// Caracter normal
		else if (i < tam - 1 && isprint(c)) {
			buffer[i++] = c;
			printf("*");
		}
	}
	printf("\n");

}

void cambioPantalla() {
	char temp[10];
	printf("\nPulse enter para continuar: ");
	fgets(temp, sizeof(temp), stdin);
	system("cls");
}

int validarOpcionMenu(int x) {
	char buffer[100];
	int num;
	char *valid;
	while(1) {
		fgets(buffer, sizeof(buffer), stdin);
		if(buffer[0] == '\n') {
			printf("Error salto de linea. Ingrese nuevamente: ");
			continue;
		}
		
		num = strtol(buffer, &valid, 10);
		
		if(*valid != '\n') {
			printf("Error: ");
			continue;
		} 
		if (num > 0 && num <= x) {
			return num;
		} else {
			printf("Error:  Opcion invalida. Debe ser entre 1 y %d.\n", x);
			continue;
		}
	}
}
