#ifndef _Usaurio
#define _Usuario
#include <windows.h>

class Usuario{
	char Nombre[50];
	char UsuarioC[20];
	char Contra[20];
	char clave[10];
	bool LeerArchivo(HWND, Usuario*);
public:
	bool ChecarLogin(HWND, bool, int, int, int);
	char* setNombre();
	char* setClave();
};

#endif