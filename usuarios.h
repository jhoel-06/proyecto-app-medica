#ifndef USUARIOS_H
#define USUARIOS_H
typedef enum {
	ADMINISTRADOR, MEDICO, PACIENTE,
} tipoUsuario;

typedef struct {
	char usuario[15];
	char contrasena[15];
	tipoUsuario tipo;
} Usuario;

tipoUsuario pedirTipoUsuario();
int obtenerTipoUsuario(const char *usuario,const char *contrasena,tipoUsuario *tipo);
int existeUsuario(const char *usuario);
int validarUsuario(const char *usuario, const char *contrasena);
int registrarUsuarioPaciente();

#endif
