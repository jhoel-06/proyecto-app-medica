#include "citas.h"
#include "funciones_varias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int validCodigo(const char *codigoStr) {
	if (strlen(codigoStr) == 0)
		return 0;
	
	for (int i = 0; codigoStr[i] != '\0'; i++) {
		if (!isdigit(codigoStr[i]))
			return 0;
	}
	return 1;
}
int buscarMedicoPorCodigo() {
	FILE *fm;
	medico m;
	char codigoStr[10];
	int codigoIngresado;
	
	do {
		fgets(codigoStr, sizeof(codigoStr), stdin);
		codigoStr[strcspn(codigoStr, "\n")] = '\0';
	} while (!validCodigo(codigoStr));
	
	codigoIngresado = atoi(codigoStr);
	
	fm = fopen("medicos.txt", "r");
	if (!fm) {
		printf("Error al abrir medicos.txt\n");
		return -1;
	}
	
	while (fscanf(fm,
				  "Nombre: %29[^|]| Codigo: %d | Especialidad: %29[^()] (%d) | Estado: %29[^()] (%d)\n",
				  m.nombre,
				  &m.codigo,
				  m.espe,
				  (int *)&m.especialidad,
				  m.est,
				  (int *)&m.estado) == 6) {
		
		if (m.codigo == codigoIngresado) {
			fclose(fm);
			return m.codigo;
		}
	}
	
	fclose(fm);
	printf("Codigo de medico no encontrado\n");
	return -1;
}

int validarCedula(const char *cedula) {
	if (strlen(cedula) != 10)
		return 0;
	
	for (int i = 0; i < 10; i++) {
		if (!isdigit(cedula[i]))
			return 0;
	}
	return 1;
}
char *buscarPacientePorCedula() {
	static char cedulaBuscada[11];
	char cedulaArchivo[11];
	FILE *fp;
	Paciente p;
	
	do {
		printf("Ingrese numero de cedula: ");
		fgets(cedulaBuscada, sizeof(cedulaBuscada), stdin);
		cedulaBuscada[strcspn(cedulaBuscada, "\n")] = '\0';
	} while (!validarCedula(cedulaBuscada));
	
	fp = fopen("pacientes.txt", "r");
	if (!fp) {
		printf("Error al abrir pacientes.txt\n");
		return NULL;
	}
	
	while (fscanf(fp,"%29[^|]|%d|%10[^|]|%10[^|]|%29[^\n]\n",p.nombre,&p.edad,cedulaArchivo,p.telefono,p.correo) == 5) {
		if (strcmp(cedulaBuscada, cedulaArchivo) == 0) {
			fclose(fp);
			return cedulaBuscada;  // encontrada
		}
	}
	
	fclose(fp);
	printf("Cedula no encontrada en el sistema\n");
	return NULL;
}

int validarFecha(const char *fecha) {
	int d, m, a;
	
	/* Detectar salto de línea */
	if (strchr(fecha, '\n') != NULL)
		return 0;
	/* Longitud exacta: dd/mm/yyyy -> 10 */
	if (strlen(fecha) != 10)
		return 0;
	/* Formato exacto */
	if (sscanf(fecha, "%2d/%2d/%4d", &d, &m, &a) != 3)
		return 0;
	if (fecha[2] != '/' || fecha[5] != '/')
		return 0;
	/* Rangos */
	if (d < 1 || d > 31)
		return 0;
	if (m < 1 || m > 12)
		return 0;
	if (a < 2024)
		return 0;
	return 1;
}

int validarHora(const char *hora) {
	int h, m;
	/* Detectar salto de línea */
	if (strchr(hora, '\n') != NULL)
		return 0;
	/* Longitud exacta: hh:mm -> 5 */
	if (strlen(hora) != 5)
		return 0;
	/* Formato exacto */
	if (sscanf(hora, "%2d:%2d", &h, &m) != 2)
		return 0;
	if (hora[2] != ':')
		return 0;
	/* Rangos */
	if (h < 0 || h > 23)
		return 0;
	if (m < 0 || m > 59)
		return 0;	
	return 1;
}

int citaExiste(int codigo, const char *fecha, const char *hora) {
	FILE *f = fopen("citas.txt", "r");
	if (!f) return 0;
	
	int cod;
	char fch[11], hr[6];
	
	while (fscanf(f, "CodigoMedico: %d | Fecha: %10s | Hora: %5s\n",
				  &cod, fch, hr) == 3) {
		if (cod == codigo && strcmp(fch, fecha) == 0 && strcmp(hr, hora) == 0) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

void agendarCita(const char *cedulaPaciente) {
	FILE *fm = fopen("medicos.txt", "r");
	if (!fm) {
		printf("Error al abrir medicos.txt\n");
		return;
	}
	
	medico lista[50];
	int n = 0;
	
	while (fscanf(fm,"Nombre: %29[^|]| Codigo: %d | Especialidad: %29[^()] (%d) | Estado: %29[^()] (%d)\n",lista[n].nombre,&lista[n].codigo,lista[n].espe,(int *)&lista[n].especialidad,lista[n].est,(int *)&lista[n].estado) == 6) {
n++;
	}
	fclose(fm);
	limpiarBuffer();
	int esp;
	printf("\nEspecialidad requerida:\n");
	printf("1. DENTISTA\n2. DERMATOLOGO\n3. PEDIATRA\n4. GINECOLOGO\n5. GENERAL\n");
	esp = validarOpcionMenu(5) - 1;
	printf("\nMedicos disponibles:\n");
	int indices[50], c = 0;
	
	for (int i = 0; i < n; i++) {
		if (lista[i].especialidad == esp) {
			printf("%d. %s | Estado: %s\n", c + 1, lista[i].nombre, lista[i].est);
			indices[c++] = i;
		}
	}
	
	if (c == 0) {
		printf("No hay medicos de esta especialidad\n");
		return;
	}
	
	int sel;
	do {
		printf("Seleccione medico (%d = Cancelar): ",c + 1);
		sel = validarOpcionMenu(c + 1);  // ahora permite 0..c
		
		if (sel == c + 1) {
			printf("Operacion cancelada por el usuario\n");
			return;   // salir de la funcion
		}
		sel--;  // ajustar a índice
		if (lista[indices[sel]].estado == NO_DISPONIBLE)
			printf("El medico seleccionado no esta disponible\n");
	} while (lista[indices[sel]].estado == NO_DISPONIBLE);
	
	if(sel != c + 1) {
	char fecha[11], hora[6];
	do {
		printf("Ingrese fecha (dd/mm/yyyy): ");
		scanf("%10s", fecha);
	} while (!validarFecha(fecha));
	
	do {
		printf("Ingrese hora (hh:mm): ");
		scanf("%5s", hora);
	} while (!validarHora(hora));
	
	int idx = indices[sel];
	
	if (citaExiste(lista[idx].codigo, fecha, hora)) {
		printf("ERROR: El medico ya tiene una cita en esa fecha y hora\n");
		return;
	}
	
	FILE *fc = fopen("citas.txt", "a");
	fprintf(fc, "CedulaPaciente: %s | CodigoMedico: %d | Fecha: %s | Hora: %s\n", cedulaPaciente, lista[idx].codigo, fecha, hora);
	fclose(fc);
	
	lista[idx].estado = NO_DISPONIBLE;
	strcpy(lista[idx].est, "NO_DISPONIBLE");
	
	fm = fopen("medicos.txt", "w");
	for (int i = 0; i < n; i++) {
		fprintf(fm,"Nombre: %s | Codigo: %d | Especialidad: %s (%d) | Estado: %s (%d)\n",lista[i].nombre,lista[i].codigo,lista[i].espe,lista[i].especialidad,lista[i].est,lista[i].estado);
	}
	fclose(fm);
	
	printf("Cita agendada exitosamente\n");
	}
	printf("Pulse enter para continuar: ");
	getchar();
	system("cls");
}

void mostrarCitas(int codigoMedico) {
	FILE *fc;
	int codigoArchivo;
	char cedula[11];
	char fecha[11];
	char hora[6];
	int encontrado = 0;
	
	fc = fopen("citas.txt", "r");
	if (!fc) {
		printf("No existen citas registradas\n");
		return;
	}
	
	printf("\nCitas del medico (Codigo %d):\n", codigoMedico);
	printf("---------------------------------------\n");
	
	while (fscanf(fc,"CedulaPaciente: %10[^|] | CodigoMedico: %d | Fecha: %10[^|] | Hora: %5[^\n]\n",cedula,&codigoArchivo,fecha,hora) == 4) {
		if (codigoArchivo == codigoMedico) {
			printf("Paciente: %s | Fecha: %s | Hora: %s\n",cedula, fecha, hora);
			encontrado = 1;
		}
	}
	
	fclose(fc);
	
	if (!encontrado) {
		printf("No hay citas registradas para este medico\n");
	}
}

void modificarCita(int codigoMedico) {
	FILE *fc, *temp;
	char cedula[11], fecha[11], hora[6];
	char nuevaFecha[11], nuevaHora[6];
	int codigoArchivo;
	int opcion, contador = 0, seleccion;
	
	fc = fopen("citas.txt", "r");
	if (!fc) {
		printf("No existen citas registradas\n");
		return;
	}
	
	/* Mostrar citas del medico */
	printf("\nCitas del medico %d:\n", codigoMedico);
	printf("-----------------------------------\n");
	
	while (fscanf(fc,"CedulaPaciente: %10[^|] | CodigoMedico: %d | Fecha: %10[^|] | Hora: %5[^\n]\n",cedula, &codigoArchivo, fecha, hora) == 4) {
		if (codigoArchivo == codigoMedico) {
			printf("%d. Cedula: %s | Fecha: %s | Hora: %s\n",++contador, cedula, fecha, hora);
		}
	}
	
	if (contador == 0) {
		printf("No hay citas para este medico\n");
		fclose(fc);
		return;
	}
	
	printf("Seleccione la cita a modificar (%d = Cancelar): ",contador + 1);
	seleccion = validarOpcionMenu(contador + 1);
	
	if (seleccion == contador + 1) {
		fclose(fc);
		printf("Operacion cancelada\n");
		return;
	}
	
	rewind(fc);
	temp = fopen("temp.txt", "w");
	
	int actual = 0;
	while (fscanf(fc,"CedulaPaciente: %10[^|] | CodigoMedico: %d | Fecha: %10[^|] | Hora: %5[^\n]\n",cedula, &codigoArchivo, fecha, hora) == 4) {
		if (codigoArchivo == codigoMedico) {
			actual++;
			if (actual == seleccion) {
				
				do {
					printf("Nueva fecha (dd/mm/yyyy): ");
					scanf("%10s", nuevaFecha);
				} while (!validarFecha(nuevaFecha));
				
				do {
					printf("Nueva hora (hh:mm): ");
					scanf("%5s", nuevaHora);
				} while (!validarHora(nuevaHora));
				
				strcpy(fecha, nuevaFecha);
				strcpy(hora, nuevaHora);
			}
		}
		
		fprintf(temp,"CedulaPaciente: %s | CodigoMedico: %d | Fecha: %s | Hora: %s\n",cedula, codigoArchivo, fecha, hora);
	}
	
	fclose(fc);
	fclose(temp);
	
	remove("citas.txt");
	rename("temp.txt", "citas.txt");
	
	printf("Fecha y hora modificadas correctamente\n");
}

void eliminarCita(int codigoMedico) {
	FILE *fc, *temp, *fm, *tempM;
	char cedula[11], fecha[11], hora[6];
	int codigoArchivo;
	int contador = 0, seleccion;
	
	fc = fopen("citas.txt", "r");
	if (!fc) {
		printf("No existen citas registradas\n");
		return;
	}
	
	printf("\nCitas del medico %d:\n", codigoMedico);
	printf("-----------------------------------\n");
	
	while (fscanf(fc,"CedulaPaciente: %10[^|] | CodigoMedico: %d | Fecha: %10[^|] | Hora: %5[^\n]\n",cedula, &codigoArchivo, fecha, hora) == 4) {
		
		if (codigoArchivo == codigoMedico) {
			printf("%d. Cedula: %s | Fecha: %s | Hora: %s\n",++contador, cedula, fecha, hora);
		}
	}
	
	if (contador == 0) {
		printf("No hay citas para este medico\n");
		fclose(fc);
		return;
	}
	
	printf("Seleccione la cita a concluir (%d = Cancelar): ", contador + 1);
	seleccion = validarOpcionMenu(contador + 1);
	
	if (seleccion == contador + 1) {
		fclose(fc);
		printf("Operacion cancelada\n");
		return;
	}
	
	rewind(fc);
	temp = fopen("temp.txt", "w");
	
	int actual = 0;
	while (fscanf(fc,
				  "CedulaPaciente: %10[^|] | CodigoMedico: %d | Fecha: %10[^|] | Hora: %5[^\n]\n",
				  cedula, &codigoArchivo, fecha, hora) == 4) {
		
		if (codigoArchivo == codigoMedico) {
			actual++;
			if (actual == seleccion)
				continue;  // omite la cita seleccionada
		}
		
		fprintf(temp,"CedulaPaciente: %s | CodigoMedico: %d | Fecha: %s | Hora: %s\n",cedula, codigoArchivo, fecha, hora);
	}
	
	fclose(fc);
	fclose(temp);
	
	remove("citas.txt");
	rename("temp.txt", "citas.txt");
	
	fm = fopen("medicos.txt", "r");
	if (!fm) {
		printf("Error al abrir medicos.txt\n");
		return;
	}
	
	tempM = fopen("tempMedicos.txt", "w");
	
	medico m;
	while (fscanf(fm,"Nombre: %29[^|] | Codigo: %d | Especialidad: %29[^()] (%d) | Estado: %29[^()] (%d)\n",m.nombre, &m.codigo, m.espe, (int*)&m.especialidad,m.est, (int*)&m.estado) == 6) {
		
		if (m.codigo == codigoMedico) {m.estado = DISPONIBLE;strcpy(m.est, "DISPONIBLE");
		}
		
		fprintf(tempM,"Nombre: %s | Codigo: %d | Especialidad: %s (%d) | Estado: %s (%d)\n",m.nombre, m.codigo, m.espe, m.especialidad, m.est, m.estado);
	}
	
	fclose(fm);
	fclose(tempM);
	
	remove("medicos.txt");
	rename("tempMedicos.txt", "medicos.txt");
	printf("Cita concluida\n");
}





