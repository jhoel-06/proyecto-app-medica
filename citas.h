#ifndef CITAS_H
#define CITAS_H
#include "medicos.h"
#include "pacientes.h"
int buscarMedicoPorCodigo();
char *buscarPacientePorCedula();
void agendarCita();
void mostrarCitas(int codigoMedico);
void modificarCita(int codigoMedico);
void eliminarCita(int codigoMedico);
void eliminarCitaPorCedula(char *cedulaPaciente);
void modificarCitaPorCedula(char *cedulaPaciente);
#endif
