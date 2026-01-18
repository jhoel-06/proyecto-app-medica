#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usuarios.h"
#include "funciones_varias.h"



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

tipoUsuario pedirTipoUsuario() {
	tipoUsuario tipo;
	int opcion;
		printf("\nSeleccione el tipo de usuario:\n");
		printf("1. ADMINISTRADOR\n");
		printf("2. MEDICO\n");
		printf("3. PACIENTE\n");
		printf("Opcion: ");
		opcion = validarOpcionMenu(3);  
	
	switch(opcion) {
	case 1:
		tipo = ADMINISTRADOR;
		break;
	case 2:
		tipo = MEDICO;
		break;		
	default:
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
	
	int registrarUsuarioPaciente() {
		char usuario[15];
		char contrasena[15];
		tipoUsuario tipo;
		
		pedirCadena("Nuevo usuario (5-15 caracteres alfabeticos): ",usuario, sizeof(usuario));
		
		pedirCadena("Nueva clave (5-10 caracteres): ",contrasena, sizeof(contrasena));
		
		tipo = pedirTipoUsuario();
		
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
	}
