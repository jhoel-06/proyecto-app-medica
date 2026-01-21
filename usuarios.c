#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "usuarios.h"
#include "funciones_varias.h"
#include "pacientes.h"
#include "citas.h"

int validarFormatoUsuario(const char *usuario) {
	int len = strlen(usuario);
	
	if (len < 5 || len > 15)
		return 0;
	
	for (int i = 0; i < len; i++) {
		if (!isalpha(usuario[i]))   
			return 0;
	}
	return 1;
}

int validarFormatoContrasena(const char *contrasena) {
	int len = strlen(contrasena);
	
	if (len < 5 || len > 10)
		return 0;
	
	return 1; 
}

int validarS() {
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
		} else {
			return num;
		}
		
	}
}

tipoUsuario pedirTipoUsuario() {
	tipoUsuario tipo;
	int opcion, seguro;
	int contador = 0;
	
	printf("\nSeleccione el tipo de usuario:\n");
	printf("1. ADMINISTRADOR\n");
	printf("2. MEDICO\n");
	printf("3. PACIENTE\n");
	printf("Opcion: ");
	opcion = validarOpcionMenu(3);  
	
	switch(opcion) {
	case 1:
		printf("Ingrese el codigo de seguridad para registrar un nuevo administrador: ");
		do {
			contador += 1;
			if(contador == 4 && seguro != 1234) {
				printf("Limite de intentos permitido alcanzado.");
			} else {
				seguro = validarS();
				if(seguro != 1234 && contador < 3) {
				printf("Codigo no valido. Ingrese nuevamente: ");
				}
			}
		} while(contador < 4 && seguro != 1234);
		if(contador == 4 && seguro != 1234) {
			return -1;
		} else {
			tipo = ADMINISTRADOR;
		}
		break;
	case 2:
		printf("Ingrese su codigo medico: ");
		do {
			contador += 1;
			if(contador == 4 && seguro ==  -1) {
				printf("Limite de intentos permitido alcanzado.");
			} else {
				seguro = buscarMedicoPorCodigo();
				if(seguro == -1 && contador < 3) {
					printf("Codigo no valido. Ingrese nuevamente: ");
				}
			}
			
		} while(contador < 4 && seguro == -1);
		if(contador == 4 && seguro == -1) {
			return -1;
		} else {
			tipo = MEDICO;
		}
		
		break;		
	default:
		crearpaciente();
		tipo = PACIENTE;
		break;
	}
	return tipo;
}

int existeUsuario(const char *usuario) {
	FILE *f = fopen("usuarios.txt", "r");
	if (!f) return 0;
	
	Usuario temp;
	
	while (fscanf(f, "%s %s %d",temp.usuario,temp.contrasena,(int *)&temp.tipo) != EOF) {
		if (strcmp(usuario, temp.usuario) == 0) {
			fclose(f);
			return 1;
		}
	}
	
	fclose(f);
	return 0;
	}
	
	int validarUsuario(const char *usuario, const char *contrasena) {
		FILE *f = fopen("usuarios.txt", "r");
		if (!f) return 0;
		
		Usuario temp;
		
		while (fscanf(f, "%s %s %d",temp.usuario,temp.contrasena,(int *)&temp.tipo) != EOF) {
			if (strcmp(usuario, temp.usuario) == 0 &&
				strcmp(contrasena, temp.contrasena) == 0) {
				fclose(f);
				return 1;
			}
		}
		fclose(f);
		return 0;
	}
	
int obtenerTipoUsuario(const char *usuario,const char *contrasena,tipoUsuario *tipo) {	
	FILE *f = fopen("usuarios.txt", "r");
	if (!f) return 0;
	
	Usuario temp;
	
	while (fscanf(f, "%s %s %d",temp.usuario,temp.contrasena,(int *)&temp.tipo) != EOF) {
		
		if (strcmp(usuario, temp.usuario) == 0 &&strcmp(contrasena, temp.contrasena) == 0) {
			*tipo = temp.tipo;
			fclose(f);
			return 1; // login correcto
		}
	}
	
	fclose(f);
	return 0; // login incorrecto
}
	
	int registrarUsuario() {
		char usuario[15];
		char contrasena[15];
		tipoUsuario tipo;
		tipo = pedirTipoUsuario();
		if (tipo != -1) {
		pedirCadena("Nuevo usuario de acceso (5-15 caracteres alfabeticos): ",usuario, sizeof(usuario));
		
		pedirCadena("Nueva clave (5-10 caracteres): ",contrasena, sizeof(contrasena));
		
		if (!validarFormatoUsuario(usuario))
			return -2;
		
		if (!validarFormatoContrasena(contrasena))
			return -3;
		
		if (existeUsuario(usuario))
			return 0;
		
		FILE *f = fopen("usuarios.txt", "a");
		if (!f) return -1;
		
		fprintf(f, "%s %s %d\n", usuario, contrasena, tipo);
		fclose(f);
		printf("Usuario registrado exitosamente\n");
		return 1;
		} else {
			printf("\nCerrando el programa...");
			return 0;
		}
	}
