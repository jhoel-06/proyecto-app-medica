#include "medicos.h"
#include "funciones_varias.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void validarNombre(char nombre[], int tam) {
	int valido;
	
	do {
		valido = 1;
		printf("Ingrese el nombre: ");
		fgets(nombre, tam, stdin);
		// Eliminar el salto de línea
		nombre[strcspn(nombre, "\n")] = '\0';
		
		// Verificar si está vacío
		if (strlen(nombre) == 0) {
			printf("Error: No puede dejar el nombre vacio.\n");
			valido = 0;
			continue;
		}
		// Verificar solo letras y espacios
		for (int i = 0; nombre[i] != '\0'; i++) {
			if (!isalpha(nombre[i]) && nombre[i] != ' ') {
				printf("Error: El nombre solo debe contener letras.\n");
				valido = 0;
				break;
			}
		}
		
	} while (!valido);
}

int codigoExiste(int codigo) {
	FILE *archivo = fopen("medicos.txt", "r");
	if (archivo == NULL) {
		// Si el archivo no existe aún, no hay códigos registrados
		return 0;
	}
	
	char linea[256];
	int codigoArchivo;
	
	while (fgets(linea, sizeof(linea), archivo)) {
		// Extrae solo el código desde la estructura del archivo
		if (sscanf(linea, "Nombre: %*[^|] | Codigo: %d", &codigoArchivo) == 1) {
			if (codigoArchivo == codigo) {
				fclose(archivo);
				return 1; // Código ya existe
			}
		}
	}
	
	fclose(archivo);
	return 0; // Código no encontrado
}


int validarCodigo() {
	char entrada[20];
	int valido;
	int codigo;
	
	do {
		valido = 1;
		printf("Ingrese un numero de 4 digitos: ");
		fgets(entrada, sizeof(entrada), stdin);

		entrada[strcspn(entrada, "\n")] = '\0';
		// Validar vacío
		if (strlen(entrada) == 0) {
			printf("Error: No puede dejar el campo vacio.\n");
			valido = 0;
			continue;
		}
		// Validar longitud
		if (strlen(entrada) != 4) {
			printf("Error: Debe ingresar exactamente 4 digitos.\n");
			valido = 0;
			continue;
		}
		// Validar solo números
		for (int i = 0; i < 4; i++) {
			if (!isdigit(entrada[i])) {
				printf("Error: Solo se permiten numeros.\n");
				valido = 0;
				break;
			}
		}
		if (!valido) continue;
		codigo = atoi(entrada);
		// Validar código duplicado
		if (codigoExiste(codigo)) {
			printf("Error: El codigo ya esta registrado.\n");
			valido = 0;
		}
		
	} while (!valido);
	
	return codigo;
}

medico m() {
	int seleccion;
	medico m;
	printf("Ingrese el nombre del medico: ");
	validarNombre(m.nombre, sizeof(m.nombre));
	
	printf("Asigne un codigo al medico: ");
	m.codigo = validarCodigo();
	
	printf("Selecione la especialidad del medico: \n");
	printf("|1:DENTISTA|2: DERMATOLOGO|3: PEDIATRA|4: GINECOLOGO|5: GENERAL|: ");
	seleccion = validarOpcionMenu(5);
	switch(seleccion) {
	case 1:
		m.especialidad = DENTISTA;
		strcpy(m.espe,"DENTISTA");
		break;
	case 2:
		m.especialidad = DERMATOLOGO;
		strcpy(m.espe,"DERMATOLOGO");
		break;
	case 3:
		m.especialidad = PEDIATRA;
		strcpy(m.espe,"PEDIATRA");
		break;
	case 4:
		m.especialidad = GINECOLOGO;
		strcpy(m.espe,"GINECOLOGO");
		break;
	default:
		m.especialidad = GENERAL;
		strcpy(m.espe,"GENERAL");
		break;
	}
	
	printf("Selecione el estado de disponibilidad actual: \n");
	printf("|1: DISPONIBLE|2: NO DISPONIBLE|: ");
	seleccion = validarOpcionMenu(2);
	switch(seleccion) {
	case 1:
		m.estado = DISPONIBLE;
		strcpy(m.est,"DISPONIBLE");
		break;
	default:
		m.estado = NO_DISPONIBLE;
		strcpy(m.est,"NO_DISPONIBLE");
		break;
	}
	return m;
}

int guardarMedicoEnArchivo(const medico *m) {
	FILE *archivo = fopen("medicos.txt", "a");
	
	if (archivo == NULL) {
		printf("Error al abrir el archivo.\n");
		return 0;
	}
	fprintf(archivo,"Nombre: %s | Codigo: %d | Especialidad: %s (%d) | Estado: %s (%d)\n",m->nombre,m->codigo,m->espe,m->especialidad,m->est,m->estado);
	fclose(archivo);
	return 1;
}

void mostrarMedicos() {
	FILE *archivo = fopen("medicos.txt", "r");
	char linea[300];
	
	char nombre[50], especialidad[50], estado[20];
	int codigo, espCod, estCod;
	
	if (archivo == NULL) {
		printf("No hay registros de medicos.\n");
		return;
	}
	
	printf("\n----- LISTA DE MEDICOS -----\n");
	printf("%-20s %-8s %-20s %-12s\n",
		   "NOMBRE", "CODIGO", "ESPECIALIDAD", "ESTADO");
	printf("-------------------------------------------------------------\n");
	
	while (fgets(linea, sizeof(linea), archivo)) {
		
		if (sscanf(linea,"Nombre: %49[^|]| Codigo: %d | Especialidad: %49[^ (] (%d) | Estado: %19[^ (] (%d)",nombre, &codigo, especialidad, &espCod, estado, &estCod) == 6) {
			
			printf("%-20s %-8d %-20s %-12s\n",nombre, codigo, especialidad, estado);
		}
	}
	
	fclose(archivo);
}

void eliminarMedico() {
	int accion;
	FILE *archivo = fopen("medicos.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	
	char linea[300];
	int codigoBuscado;
	int encontrado = 0;
	
	if (archivo == NULL || temp == NULL) {
		printf("Error al abrir los archivos.\n");
		return;
	}
	
	printf("Ingrese el codigo del medico a eliminar: ");
	codigoBuscado = validarCodigo();
	
	while (fgets(linea, sizeof(linea), archivo)) {
		int codigo;
		
		sscanf(linea, "%*[^0-9]%d", &codigo);
		
		if (codigo != codigoBuscado) {
			fputs(linea, temp);
		} else {
			encontrado = 1;
		}
	}
	if (encontrado) {
		printf("Medico encontrado.\n");
		printf("Desea eliminar el registro de este medico?\n");
		   printf("|1: si|2: NO|: ");
		   accion = validarOpcionMenu(2);
		   switch(accion) {
		   case 1:	
			   fclose(archivo);
			   fclose(temp);
			   remove("medicos.txt");
			   rename("temp.txt", "medicos.txt");
			   printf("Medico eliminado correctamente.");
			   break;
		   default:
			   printf("No se elimino el registro del medico.");
			   break;
		   }
	} else {
		printf("No se encontro el medico con codigo %d.\n", codigoBuscado);
	}	
}

void modificarMedico() {
	FILE *archivo = fopen("medicos.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	
	char linea[300];
	int codigoBuscado;
	int encontrado = 0;
	medico m;
	
	char entrada[50];
	
	if (archivo == NULL || temp == NULL) {
		printf("Error al abrir los archivos.\n");
		return;
	}
	
	printf("Ingrese el codigo del medico a modificar: ");
	codigoBuscado = validarCodigo();
	
	while (fgets(linea, sizeof(linea), archivo)) {
		int codigo;
		sscanf(linea, "%*[^0-9]%d", &codigo);
		
		if (codigo == codigoBuscado) {
			encontrado = 1;
			printf("\n--- MODIFICANDO MEDICO ---\n");
			printf("(Presione ENTER para mantener el valor actual)\n");
			/* ---------- NOMBRE ---------- */
			printf("Nuevo nombre: ");
			fgets(entrada, sizeof(entrada), stdin);
			
			if (strcmp(entrada, "\n") != 0) {
				entrada[strcspn(entrada, "\n")] = '\0';
				strcpy(m.nombre, entrada);
			} else {
				// conservar nombre anterior
				sscanf(linea, "Nombre: %[^|]", m.nombre);
			}
			m.codigo = codigoBuscado;
			/* ---------- ESPECIALIDAD ---------- */
			printf("Nueva especialidad:\n");
			printf("|1:DENTISTA|2:DERMATOLOGO|3:PEDIATRA|4:GINECOLOGO|5:GENERAL| (ENTER para omitir): ");
			fgets(entrada, sizeof(entrada), stdin);
			
			if (strcmp(entrada, "\n") != 0) {
				int sel = atoi(entrada);
				
				switch (sel) {
				case 1: m.especialidad = DENTISTA; strcpy(m.espe,"DENTISTA"); break;
				case 2: m.especialidad = DERMATOLOGO; strcpy(m.espe,"DERMATOLOGO"); break;
				case 3: m.especialidad = PEDIATRA; strcpy(m.espe,"PEDIATRA"); break;
				case 4: m.especialidad = GINECOLOGO; strcpy(m.espe,"GINECOLOGO"); break;
				default: m.especialidad = GENERAL; strcpy(m.espe,"GENERAL");
				}
			} else {
				sscanf(linea, "%*[^|]|%*[^|]| Especialidad: %[^ (]", m.espe);
				sscanf(linea, "%*[^ (](%d)", &m.especialidad);
			}
			/* ---------- ESTADO ---------- */
			printf("Nuevo estado:\n");
			printf("|1:DISPONIBLE|2:NO DISPONIBLE| (ENTER para omitir): ");
			fgets(entrada, sizeof(entrada), stdin);
			
			if (strcmp(entrada, "\n") != 0) {
				int sel = atoi(entrada);
				if (sel == 1) {
					m.estado = DISPONIBLE;
					strcpy(m.est,"DISPONIBLE");
				} else {
					m.estado = NO_DISPONIBLE;
					strcpy(m.est,"NO_DISPONIBLE");
				}
			} else {
				sscanf(linea, "%*[^|]|%*[^|]|%*[^|]| Estado: %[^ (]", m.est);
				sscanf(linea, "%*[^ (](%d)", &m.estado);
			}
			
			fprintf(temp,"Nombre: %s | Codigo: %d | Especialidad: %s (%d) | Estado: %s (%d)\n",m.nombre,m.codigo,m.espe,m.especialidad,m.est,m.estado);
		} else {
			fputs(linea, temp);
		}
	}
	
	fclose(archivo);
	fclose(temp);
	
	remove("medicos.txt");
	rename("temp.txt", "medicos.txt");
	
	if (encontrado)
		printf("Medico modificado correctamente.\n");
	else
		printf("No se encontro el medico con codigo %d.\n", codigoBuscado);
}
