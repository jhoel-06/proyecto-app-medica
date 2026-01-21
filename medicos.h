#ifndef MEDICOS_H
#define MEDICOS

//Estructura principal del medico
typedef enum {
	DENTISTA, DERMATOLOGO, PEDIATRA, GINECOLOGO, GENERAL
} especialidades;

typedef enum {
	DISPONIBLE, NO_DISPONIBLE
} estados;

typedef struct {
	char nombre[30];
	int codigo;
	especialidades especialidad;
	char espe[30];
	estados estado;
	char est[30];
} medico;

medico m(); //Crear estructura medico
int guardarMedicoEnArchivo(const medico *m); //Guardar los datos en un .txt
void mostrarMedicos(); //funcion para imprimir en pantalla la lista de medicos
void eliminarMedico(); //funcion para eliminar el registro de un medico
void modificarMedico(); //funcion para modificar el registro de un medico
#endif
