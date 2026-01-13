#include "funciones_varias.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>      
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

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
	printf("Pulse enter para continuar: ");
	getchar();
	system("cls");
}

int validarOpcionMenu(int x) {
	char entrada[10];
	int opcion;
	bool valido;
	
	do {
		valido = true;
		//printf("Seleccione una opcion: ");
		if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
			printf("Error al leer la entrada.\n");
			valido = false;
			continue;
		}
		
		entrada[strcspn(entrada, "\n")] = '\0';
		
		if (entrada[0] == '\0') {
			printf("Error: No se ingreso ninguna opcion.\n");
			valido = false;
			continue;
		}
		
		// Validar que sean números
		for (int i = 0; entrada[i] != '\0'; i++) {
			if (!isdigit((unsigned char)entrada[i])) {
				printf("Error: Solo se permiten numeros.\n");
				valido = false;
				break;
			}
		}
		
		if (!valido) continue;
		
		opcion = atoi(entrada);
		
		if (opcion < 1 || opcion > x) {
			printf("Error: Opcion invalida. Debe ser entre 1 y %d.\n",x);
			valido = false;
		}
		
	} while (!valido);
	
	return opcion;
}
