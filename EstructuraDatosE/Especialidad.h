#ifndef _Especialidad
#define _Especialidad
#include <windows.h>
#include <strsafe.h>
#include "DatMedico.h"

class Especialidad{
public:
	char clave[10];
	char Nombre[20];
	char DescripcionEsp[50];
	Especialidad* Sig;
	Especialidad* Prev;

	void MostrarDatosE(Especialidad *dir,HWND hWnd,bool mostrar, short pos);

	void MostrarDatosEdit(Especialidad* dir, HWND hWnd);

	bool ChecarBorrar(Especialidad* nodo, Tree_Medico* tree);

	void CopyNode(Especialidad*, Especialidad*);
};

#endif