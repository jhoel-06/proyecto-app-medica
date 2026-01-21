#ifndef FUNCIONES_H
#define FUNCIONES
void limpiarBuffer(); //Limpiar residuos del buffer
void pedirCadena(const char *mensaje, char *buffer, int tam); //Funcion para validar una cadena de texto
void pedirContrasena(const char *mensaje, char *buffer, int tam); //Funcion para validar una contraseña
int validarOpcionMenu(int x); //validar que un numero este un rango determinado
void cambioPantalla(); //limpiar la pantalla


#endif
