#ifndef CITAS_H
#define CITAS_H

int buscarMedicoPorCodigo(); //Buscar un medico en el archivo mediante su codigo
char *buscarPacientePorCedula(); //Buscar un paciente en el archivo mediante su cedula
void agendarCita(); //Funcion para agendar una cita
void mostrarCitas(int codigoMedico); //Funcion para mostrar el registro de citas
void modificarCita(int codigoMedico); //Funcion para modificar la fecha y hora de una cita(medico)
void eliminarCita(int codigoMedico); //Funcion para concluir una cita(medico)
void eliminarCitaPorCedula(char *cedulaPaciente); //Funcion para cancelar una cita(paciente)
void modificarCitaPorCedula(char *cedulaPaciente); //Funcion para modificar la fecha y hora de una cita(paciente)
#endif
