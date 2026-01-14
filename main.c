#include <stdio.h>
#include <string.h>
#include <ctype.h>      
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
//#include "pacientes.h"
#include "usuarios.h"
//#include "medicos.h"
#include "citas.h"
#include "funciones_varias.h"


int nuevo, accion, seleccion, retroceso;

int main() {
	medico nuevoMedico;
	char usuario[20];
	char contrasena[20];
	int loginCorrecto = 0;
	int nuevo;
	tipoUsuario tipo;
	char *cedulaPaciente;
	int codigo;
	// Menu de acceso
	while (!loginCorrecto) {
		
		printf("\n----- MENU DE ACCESO -----\n");
		pedirCadena("Usuario: ", usuario, sizeof(usuario));
		pedirContrasena("Clave: ", contrasena, sizeof(contrasena));
		
		if (obtenerTipoUsuario(usuario, contrasena, &tipo)) {
			printf("\nInicio de sesion exitoso. Pulse enter para continuar.\n");
			getchar();
			system("cls");
			loginCorrecto = 1;
			// Mostrar pantalla según tipo
			switch(tipo) {
			case ADMINISTRADOR:
				// Pantalla administrador
				while(1) {
					printf("\n-----MENU PRINCIPAL-----\n");
					printf("1. Agregar/Modificar Paciente\n");
					printf("2. Agregar/Modificar Medico\n");
					printf("3. Asignar cita a un paciente\n");
					printf("4. Salir del programa\n");
					accion = validarOpcionMenu(4);
					cambioPantalla();
					switch(accion) {
					case 1:
						retroceso = 0;
						while(!retroceso) {
							printf("\n-----GESTION DE PACIENTES-----\n");
							printf("1: Agregar paciente\n");
							printf("2: Modificar paciente\n");
							printf("3: Eliminar paciente\n");
							printf("4: Mostrar lista de pacientes registrados\n");
							printf("5: Regresar al menu principal\n");
							seleccion = validarOpcionMenu(5);
							
							switch(seleccion) {
							case 1: 
							{
								Paciente p = crearpaciente(); 
								cambioPantalla();
							}
							break;
							
							case 2:
								modificarPaciente();
								cambioPantalla();
								break;
							case 3:
								eliminarPaciente();
								cambioPantalla();
								break;
							case 4:
								imprimirPacientes();
								cambioPantalla();
								break;
							case 5:
								cambioPantalla();
								retroceso = 1;
								break;
								
							default:
								printf("Opcion no valida\n");
							}
						}
						break;
						
					case 2:
						retroceso = 0;
						while(!retroceso) {
							printf("\n-----GESTION DE MEDICOS-----\n");
							printf("1: Agregar medico\n");
							printf("2: Modificar medico\n");
							printf("3: Eliminar medico\n");
							printf("4: Mostrar lista de medicos registrados\n");
							printf("5: Regresar al menu principal\n");
							seleccion = validarOpcionMenu(5);
							switch(seleccion) {
							case 1:
								nuevoMedico = m();
								if (guardarMedicoEnArchivo(&nuevoMedico)) {
									printf("Medico guardado correctamente.\n");
									cambioPantalla();
								}
								break;
							case 2:
								modificarMedico();
								cambioPantalla();
								break;
							case 3:
								eliminarMedico();
								cambioPantalla();
								break;
							case 4:
								mostrarMedicos();
								cambioPantalla();
								break;
							default:
								cambioPantalla();
								retroceso = 1;
								break;
							}
						}
						break;
					case 3:
						cedulaPaciente = buscarPacientePorCedula();
						if (cedulaPaciente != NULL) {
						agendarCita(cedulaPaciente);
						}
						break;
					case 4:
						printf("Saliendo del programa...\n");
						return 1;
						
					default:
						printf("Opcion invalida\n");
					}
				}
				break;
			case MEDICO:
				//Pantalla medico
				printf("Ingrese su codigo medico: ");
				codigo = buscarMedicoPorCodigo();
				if (codigo != -1) {
					printf("Medico con codigo %d encontrado\n", codigo);
				} else {
					return 1;
				}
				cambioPantalla();
				while(1) {
				printf("\n-----MENU PRINCIPAL-----\n");
				printf("1. Consultar lista de pacientes con citas pendientes\n");
				printf("2. Posponer cita\n");
				printf("3. Realizar diagnostico\n");
				printf("4. Salir del programa\n");
				accion = validarOpcionMenu(4);
				switch(accion) {
				case 1:
					mostrarCitas(codigo);
					cambioPantalla();
					break;
				case 2:
					modificarCita(codigo);
					cambioPantalla();
					break;	
				case 3:
					eliminarCita(codigo);
					cambioPantalla();
					break;
				default:
					printf("Saliendo del programa...\n");
					return 1;
				}
				break;
				}
			default:
				//Pantalla paciente
				printf("\n-----MENU PRINCIPAL-----\n");
				printf("1. Agendar una cita\n");
				printf("2. Reagendar cita\n");
				printf("3. Cancelar cita\n");
				printf("4. Consultar lista de medicos y horarios de disponibilidad\n");
				printf("5. Salir del programa\n");
				break;
			}
		} else { //Login incorrecto
			printf("\nUsuario o clave incorrectos.\n");
			printf("Desea registrar un nuevo usuario?\n");
			printf("| 1: Si | 2: No | 3: Salir del programa | ");
			nuevo = validarOpcionMenu(3);
			switch (nuevo) {
			case 1:
				registrarUsuarioPaciente();
				cambioPantalla();
				break;
			case 2:
				cambioPantalla();
				break;
				
			default:
				printf("Saliendo del programa...");
				return 1;
			}
		}
	}
}
