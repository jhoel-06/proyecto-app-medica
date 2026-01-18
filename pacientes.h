#ifndef PACIENTES_H
#include <stdbool.h>
#define NOMBRE_LEN 31
#define CEDULA_LEN 11
#define TELEFONO_LEN 11
#define CORREO_LEN 51


typedef struct{
	char nombre[30];
	int edad;
	char cedula[11];
	char telefono[11];
	char correo[30];
}Paciente;
Paciente crearpaciente(void);
bool guardarPacienteEnArchivo(const Paciente *p);
void eliminarPaciente();
void modificarPaciente(void);
void imprimirPacientes();

#endif 
