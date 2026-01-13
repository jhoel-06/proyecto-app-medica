#include "pacientes.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		;
	}
}


bool soloDigitos(const char *cadena) {
	if (cadena == NULL) return false;
	for (size_t i = 0; cadena[i] != '\0'; i++) {
		if (!isdigit((unsigned char)cadena[i])) return false;
	}
	return true;
}


bool validarEmail(const char *email) {
	if (email == NULL) return false;
	size_t len = strlen(email);
	if (len < 5 || len >= CORREO_LEN) return false; 
	
	int countAt = 0;
	const char *atPos = NULL;
	for (size_t i = 0; i < len; i++) {
		if (email[i] == ' ') return false;
		if (email[i] == '@') {
			countAt++;
			atPos = &email[i];
		}
	}
	if (countAt != 1) return false;
	
	if (atPos == NULL) return false;
	const char *dot = strchr(atPos + 1, '.');
	if (dot == NULL) return false;
	if (dot == atPos + 1) return false; 
	if (dot == email + len - 1) return false; 
	return true;
}


static void trim(char *s) {
	if (!s) return;
	
	s[strcspn(s, "\n")] = '\0';

	char *start = s;
	while (*start == ' ') start++;
	if (start != s) memmove(s, start, strlen(start) + 1);

	char *end = s + strlen(s) - 1;
	while (end >= s && *end == ' ') {
		*end = '\0';
		end--;
	}
}


bool cedulaExiste(const char *cedula) {
	if (cedula == NULL || strlen(cedula) == 0) return false;
	FILE *f = fopen("pacientes.txt", "r");
	if (!f) return false; 
	
	char linea[512];
	bool existe = false;
	while (fgets(linea, sizeof(linea), f) != NULL) {
		trim(linea);
		if (linea[0] == '\0') continue;

		char *copia = strdup(linea);
		if (!copia) break;
		char *tok = strtok(copia, "|"); 
		tok = strtok(NULL, "|"); 
		tok = strtok(NULL, "|"); 
		if (tok) {
			trim(tok);
			if (strcmp(tok, cedula) == 0) {
				existe = true;
			}
		}
		free(copia);
		if (existe) break;
	}
	fclose(f);
	return existe;
}


bool correoExiste(const char *correo) {
	if (correo == NULL || strlen(correo) == 0) return false;
	FILE *f = fopen("pacientes.txt", "r");
	if (!f) return false;
	
	char linea[512];
	bool existe = false;
	while (fgets(linea, sizeof(linea), f) != NULL) {
		trim(linea);
		if (linea[0] == '\0') continue;
		char *copia = strdup(linea);
		if (!copia) break;
		char *tok = strtok(copia, "|"); 
		tok = strtok(NULL, "|"); 
		tok = strtok(NULL, "|"); 
		tok = strtok(NULL, "|"); 
		tok = strtok(NULL, "|"); 
		if (tok) {
			trim(tok);
			if (strcmp(tok, correo) == 0) {
				existe = true;
			}
		}
		free(copia);
		if (existe) break;
	}
	fclose(f);
	return existe;
}


bool guardarPacienteEnArchivo(const Paciente *p) {
	if (!p) return false;
	if (strlen(p->nombre) == 0 || strlen(p->cedula) == 0 ||
		strlen(p->telefono) == 0 || strlen(p->correo) == 0) {
		fprintf(stderr, "Error: Datos incompletos\n");
		return false;
	}
	
	FILE *f = fopen("pacientes.txt", "a");
	if (!f) {
		perror("fopen");
		return false;
	}
	
	char nombreSafe[NOMBRE_LEN];
	char correoSafe[CORREO_LEN];
	strncpy(nombreSafe, p->nombre, sizeof(nombreSafe)-1); nombreSafe[sizeof(nombreSafe)-1] = '\0';
	strncpy(correoSafe, p->correo, sizeof(correoSafe)-1); correoSafe[sizeof(correoSafe)-1] = '\0';
	for (size_t i = 0; i < strlen(nombreSafe); i++) if (nombreSafe[i] == '|') nombreSafe[i] = '-';
	for (size_t i = 0; i < strlen(correoSafe); i++) if (correoSafe[i] == '|') correoSafe[i] = '-';
	
	int r = fprintf(f, "%s|%d|%s|%s|%s\n",
					nombreSafe, p->edad, p->cedula, p->telefono, correoSafe);
	fclose(f);
	return (r > 0);
}


void eliminarPaciente() {
	FILE *archivo = fopen("pacientes.txt", "r");
	if (archivo == NULL) {
		printf("No hay pacientes registrados.\n");
		return;
	}
	
	char cedulaEliminar[11];
	printf("Ingrese la cedula del paciente a eliminar (10 digitos): ");
	fgets(cedulaEliminar, sizeof(cedulaEliminar), stdin);
	cedulaEliminar[strcspn(cedulaEliminar, "\n")] = '\0';
	
	if (strlen(cedulaEliminar) != 10 || !soloDigitos(cedulaEliminar)) {
		printf("Cedula invalida.\n");
		fclose(archivo);
		return;
	}
	
	char linea[256];
	bool encontrado = false;
	
	// Datos del paciente a eliminar
	char nombre[31], cedula[11], telefono[11], correo[51];
	int edad;
	
	// 1?? Buscar el paciente primero
	while (fgets(linea, sizeof(linea), archivo)) {
		int n = sscanf(linea, "%30[^|]|%d|%10[^|]|%10[^|]|%50[^\n]",
					   nombre, &edad, cedula, telefono, correo);
		
		if (n == 5 && strcmp(cedula, cedulaEliminar) == 0) {
			encontrado = true;
			break;
		}
	}
	
	rewind(archivo); // Regresar al inicio para volver a leer
	
	if (!encontrado) {
		printf("No se encontro un paciente con la cedula %s.\n", cedulaEliminar);
		fclose(archivo);
		return;
	}
	
	// 2?? Mostrar datos para confirmación
	printf("\nPaciente encontrado:\n");
	printf("Nombre: %s\n", nombre);
	printf("Edad: %d\n", edad);
	printf("Cedula: %s\n", cedula);
	printf("Telefono: %s\n", telefono);
	printf("Correo: %s\n", correo);
	
	char confirmacion[5];
	printf("\nDesea eliminar este paciente? (s/n): ");
	limpiarBuffer();
	fgets(confirmacion, sizeof(confirmacion), stdin);
	
	if (tolower(confirmacion[0]) != 's') {
		printf("Operacion cancelada.\n");
		fclose(archivo);
		return;
	}
	
	// 3?? Crear archivo temporal y eliminar realmente
	FILE *temp = fopen("temp_pacientes.txt", "w");
	if (temp == NULL) {
		printf("No se pudo crear archivo temporal.\n");
		fclose(archivo);
		return;
	}
	
	bool eliminado = false;
	
	while (fgets(linea, sizeof(linea), archivo)) {
		char t_nombre[31], t_cedula[11], t_telefono[11], t_correo[51];
		int t_edad;
		
		int n = sscanf(linea, "%30[^|]|%d|%10[^|]|%10[^|]|%50[^\n]",
					   t_nombre, &t_edad, t_cedula, t_telefono, t_correo);
		
		if (n == 5 && strcmp(t_cedula, cedulaEliminar) == 0) {
			eliminado = true;
			continue; // NO escribirlo ? se elimina
		}
		
		fputs(linea, temp);
	}
	
	fclose(archivo);
	fclose(temp);
	
	if (eliminado) {
		remove("pacientes.txt");
		rename("temp_pacientes.txt", "pacientes.txt");
		printf("\nPaciente eliminado correctamente.\n");
	} else {
		printf("\nError inesperado: no se pudo eliminar.\n");
		remove("temp_pacientes.txt");
	}
}

void modificarPaciente() {
	FILE *f = fopen("pacientes.txt", "r");
	if (!f) {
		printf("Error: No hay pacientes registrados\n");
		return;
	}
	
	char cedulaBuscar[11];
	printf("Ingrese la cedula del paciente a modificar (10 digitos): ");
	if (fgets(cedulaBuscar, sizeof(cedulaBuscar), stdin) == NULL) {
		printf("Entrada vacía\n");
		fclose(f);
		return;
	}
	trim(cedulaBuscar);
	if (strlen(cedulaBuscar) != 10 || !soloDigitos(cedulaBuscar)) {
		printf("Cedula invalida\n");
		fclose(f);
		return;
	}
	
	FILE *tmp = fopen("temp_pacientes.txt", "w");
	if (!tmp) {
		printf("No se pudo crear archivo temporal\n");
		fclose(f);
		return;
	}
	
	char linea[512];
	bool encontrado = false;
	
	while (fgets(linea, sizeof(linea), f) != NULL) {
		char copiaLinea[512];
		strncpy(copiaLinea, linea, sizeof(copiaLinea)-1); copiaLinea[sizeof(copiaLinea)-1] = '\0';
		trim(copiaLinea);
		if (copiaLinea[0] == '\0') continue;
		
		char *s = strdup(copiaLinea);
		if (!s) break;
		char *tok = strtok(s, "|");
		char nombre[30];
		char cedula[11];
		char telefono[11];
		char correo[30];
		int edad = 0;
		
		if (tok) strncpy(nombre, tok, sizeof(nombre)-1); else nombre[0] = '\0';
		tok = strtok(NULL, "|");
		if (tok) edad = atoi(tok);
		tok = strtok(NULL, "|");
		if (tok) strncpy(cedula, tok, sizeof(cedula)-1); else cedula[0] = '\0';
		tok = strtok(NULL, "|");
		if (tok) strncpy(telefono, tok, sizeof(telefono)-1); else telefono[0] = '\0';
		tok = strtok(NULL, "|");
		if (tok) strncpy(correo, tok, sizeof(correo)-1); else correo[0] = '\0';
		
		nombre[sizeof(nombre)-1] = '\0'; trim(nombre);
		cedula[sizeof(cedula)-1] = '\0'; trim(cedula);
		telefono[sizeof(telefono)-1] = '\0'; trim(telefono);
		correo[sizeof(correo)-1] = '\0'; trim(correo);
		
		if (strcmp(cedula, cedulaBuscar) == 0) {
			encontrado = true;
			printf("\nPaciente encontrado:\n");
			printf("Nombre: %s\nEdad: %d\nCedula: %s\nTelefono: %s\nCorreo: %s\n\n",
				   nombre, edad, cedula, telefono, correo);
			
			Paciente np;

			strncpy(np.nombre, nombre, sizeof(np.nombre)-1); np.nombre[sizeof(np.nombre)-1] = '\0';
			np.edad = edad;
			strncpy(np.cedula, cedula, sizeof(np.cedula)-1); np.cedula[sizeof(np.cedula)-1] = '\0';
			strncpy(np.telefono, telefono, sizeof(np.telefono)-1); np.telefono[sizeof(np.telefono)-1] = '\0';
			strncpy(np.correo, correo, sizeof(np.correo)-1); np.correo[sizeof(np.correo)-1] = '\0';
			
			char buffer[128];
			
			limpiarBuffer();
			
			printf("Ingrese nuevo nombre (enter para mantener): ");
			if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
				trim(buffer);
				if (strlen(buffer) > 0) {
					strncpy(np.nombre, buffer, sizeof(np.nombre)-1); np.nombre[sizeof(np.nombre)-1] = '\0';
				}
			}
			
			printf("Ingrese nueva edad (enter para mantener): ");
			if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
				trim(buffer);
				if (strlen(buffer) > 0) {
					int e = atoi(buffer);
					if (e >= 0 && e <= 150) np.edad = e; else printf("Edad invalida, se mantiene anterior\n");
				}
			}
			
			printf("Ingrese nuevo telefono (10 digitos, enter para mantener): ");
			if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
				trim(buffer);
				if (strlen(buffer) > 0) {
					if (strlen(buffer) == 10 && soloDigitos(buffer)) strncpy(np.telefono, buffer, sizeof(np.telefono)-1);
					else printf("Telefono invalido, se mantiene anterior\n");
				}
			}
			
			printf("Ingrese nuevo correo (enter para mantener): ");
			if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
				trim(buffer);
				if (strlen(buffer) > 0) {
					if (validarEmail(buffer)) {
						if (correoExiste(buffer) && strcmp(buffer, correo) != 0) {
							printf("El correo ya existe, no se actualiza\n");
						} else {
							strncpy(np.correo, buffer, sizeof(np.correo)-1);
							np.correo[sizeof(np.correo)-1] = '\0';
						}
					} else {
						printf("Formato de correo invalido, se mantiene anterior\n");
					}
				}
			}
			
			fprintf(tmp, "%s|%d|%s|%s|%s\n",
					np.nombre, np.edad, np.cedula, np.telefono, np.correo);
		} else {
			fprintf(tmp, "%s\n", copiaLinea);
		}
		
		free(s);
	}
	
	fclose(f);
	fclose(tmp);
	
	if (encontrado) {
		if (remove("pacientes.txt") != 0) perror("remove");
		if (rename("temp_pacientes.txt", "pacientes.txt") != 0) perror("rename");
		printf("Modificacion completada.\n");
	} else {
		remove("temp_pacientes.txt");
		printf("No se encontro paciente con cedula %s\n", cedulaBuscar);
	}
}


Paciente crearpaciente(void) {
	Paciente p;
	memset(&p, 0, sizeof(p));
	char temp[256];
	
	printf("=== REGISTRO DE NUEVO PACIENTE ===\n");
	
	while (1) {
		printf("Ingrese nombre (max 30 caracteres, solo letras y espacios): ");
		if (fgets(temp, sizeof(temp), stdin) == NULL) { printf("Entrada invalida\n"); continue; }
		trim(temp);
		if (strlen(temp) == 0 || strlen(temp) >= NOMBRE_LEN) { printf("Nombre invalido\n"); continue; }
		bool ok = true;
		for (size_t i = 0; temp[i] != '\0'; i++) {
			if (!isalpha((unsigned char)temp[i]) && temp[i] != ' ') { ok = false; break; }
		}
		if (!ok) { printf("Nombre solo letras y espacios\n"); continue; }
		strncpy(p.nombre, temp, sizeof(p.nombre)-1); p.nombre[sizeof(p.nombre)-1] = '\0';
		break;
	}
	
	while (1) {
		printf("Ingrese edad (1-110): ");
		if (fgets(temp, sizeof(temp), stdin) == NULL) { printf("Entrada invalida\n"); continue; }
		trim(temp);
		if (!soloDigitos(temp)) { printf("Edad debe ser numerica\n"); continue; }
		int edad = atoi(temp);
		if (edad < 1 || edad > 110) { printf("Edad no valida\n"); continue; }
		p.edad = edad;
		break;
	}
	
	while (1) {
		printf("Ingrese cedula (10 digitos): ");
		if (fgets(temp, sizeof(temp), stdin) == NULL) { printf("Entrada invalida\n"); continue; }
		trim(temp);
		if (strlen(temp) != 10 || !soloDigitos(temp)) { printf("Cedula invalida\n"); continue; }
		if (cedulaExiste(temp)) {
			char resp[8];
			printf("La cedula %s ya existe. Desea probar otra? (s/n): ", temp);
			if (fgets(resp, sizeof(resp), stdin) == NULL) continue;
			trim(resp);
			if (resp[0] == 's' || resp[0] == 'S') continue; else { printf("Registro cancelado\n"); return p; }
		}
		strncpy(p.cedula, temp, sizeof(p.cedula)-1); p.cedula[sizeof(p.cedula)-1] = '\0';
		break;
	}
	
	while (1) {
		printf("Ingrese telefono (10 digitos): ");
		if (fgets(temp, sizeof(temp), stdin) == NULL) { printf("Entrada invalida\n"); continue; }
		trim(temp);
		if (strlen(temp) != 10 || !soloDigitos(temp)) { printf("Telefono invalido\n"); continue; }
		strncpy(p.telefono, temp, sizeof(p.telefono)-1); p.telefono[sizeof(p.telefono)-1] = '\0';
		break;
	}
	
	while (1) {
		printf("Ingrese correo electronico (max 50 caracteres): ");
		if (fgets(temp, sizeof(temp), stdin) == NULL) { printf("Entrada invalida\n"); continue; }
		trim(temp);
		if (!validarEmail(temp)) { printf("Correo invalido\n"); continue; }
		if (correoExiste(temp)) {
			char resp[8];
			printf("El correo %s ya existe. Desea usar otro? (s/n): ", temp);
			if (fgets(resp, sizeof(resp), stdin) == NULL) continue;
			trim(resp);
			if (resp[0] == 's' || resp[0] == 'S') continue; else { printf("Registro cancelado\n"); return p; }
		}
		strncpy(p.correo, temp, sizeof(p.correo)-1); p.correo[sizeof(p.correo)-1] = '\0';
		break;
	}
	
	if (!guardarPacienteEnArchivo(&p)) {
		printf("Error al guardar paciente\n");
		memset(&p, 0, sizeof(p));
	} else {
		printf("Paciente registrado correctamente.\n");
	}
	return p;
}

void imprimirPacientes() {
	char linea[200];
	FILE *f = fopen("pacientes.txt", "r");
	if (!f) {
		perror("fopen");
		return false;
	}
	printf("\n--- PACIENTES REGISTRADOS---\n");
	printf("NOMBRE|EDAD| CEDULA  | CELULAR | CORREO \n");
	while (fgets(linea, sizeof(linea), f) != NULL) {
		printf("%s", linea);
	}
	fclose(f);
}





