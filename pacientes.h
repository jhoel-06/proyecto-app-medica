#ifndef PACIENTES_H
#include <stdbool.h>
#define NOMBRE_LEN 31
#define CEDULA_LEN 11
#define TELEFONO_LEN 11
#define CORREO_LEN 51
//Estructura principal del paciente
typedef struct{
	char nombre[30];
	int edad;
	char cedula[11];
	char telefono[11];
	char correo[30];
}Paciente;

Paciente crearpaciente(void); //Funcion para añadir un paciente
void eliminarPaciente(); //Funcion para eliminar un paciente del archivo
void modificarPaciente(); //Funcion para modificar un paciente del archivo
void imprimirPacientes(); //Funcion para imprimir en pantalla la lista de pacientes registrados

#endif 
