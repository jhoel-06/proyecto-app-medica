#ifndef USUARIOS_H
#define USUARIOS_H
//Estructura principal de un tipo de usuario
typedef enum {
	ADMINISTRADOR, MEDICO, PACIENTE,
} tipoUsuario;
typedef struct {
	char usuario[15];
	char contrasena[15];
	tipoUsuario tipo;
} Usuario;

int obtenerTipoUsuario(const char *usuario,const char *contrasena,tipoUsuario *tipo); //Funcion para validar el login
int registrarUsuario(); //Funcion para registrar un usuario

#endif
