#ifndef CITAS_H
#define CITAS_H
#include "medicos.h"
#include "pacientes.h"
int buscarMedicoPorCodigo();
char *buscarPacientePorCedula();
int validarFecha(const char *fecha);
int validarHora(const char *hora);
int citaExiste(int codigo, const char *fecha, const char *hora);
void agendarCita();
void mostrarCitas(int codigoMedico);
void modificarCita(int codigoMedico);
void eliminarCita(int codigoMedico);
#endif
