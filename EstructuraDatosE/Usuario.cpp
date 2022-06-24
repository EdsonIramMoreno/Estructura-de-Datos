#include "Usuario.h"
#include <fstream>
using namespace std;


bool Usuario::ChecarLogin(HWND hWnd, bool RadioB, int a, int b, int c) {
	bool regresar;
	GetWindowTextA(GetDlgItem(hWnd,a), this->UsuarioC, 19);
	if (!RadioB)
		GetWindowTextA(GetDlgItem(hWnd, b), Contra, 19);
	else
		GetWindowTextA(GetDlgItem(hWnd, c), Contra, 19);
	regresar = LeerArchivo(hWnd, this);
	RadioB = false;
	return regresar;
}

char* Usuario::setNombre() {
	return this->Nombre;
}

char* Usuario::setClave() {
	return this->clave;
}

bool Usuario::LeerArchivo(HWND hWnd, Usuario* Datos) {
	ifstream archivo;
	bool checar = false;
	char UsuarioA[20] = { '\0' };
	char ContraA[20] = { '\0' };
	archivo.open("EstructuraDatos//Usuarios.txt", ios::in);
	if (archivo.fail()) {
		MessageBoxA(hWnd, "El archivo no se pudo abrir.", "Error", MB_OK | MB_ICONERROR);
	}
	else {
		while ((!archivo.eof()) && !checar) {
			archivo.getline(UsuarioA, 20);
			archivo.getline(ContraA, 20);
			archivo.getline(Datos->Nombre, 40);
			archivo.getline(Datos->clave, 10);
			if (strcmp(UsuarioA, Datos->UsuarioC) == 0 && strcmp(ContraA, Datos->Contra) == 0) {
				checar = true;
			}
		}
	}
	archivo.close();
	return checar;
}