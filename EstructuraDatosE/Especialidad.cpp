#include "Especialidad.h"
#include <iostream>
#include "resource.h"

	void Especialidad::MostrarDatosE(Especialidad *dir, HWND hWnd, bool mostrar, short pos) {
		Especialidad* aux = dir;
		char datos[35];
			StringCchPrintfA(datos, 34, " %s: %s",  aux->clave, aux->Nombre);
			if(mostrar)
				SendMessageA(GetDlgItem(hWnd, IDC_ToEspecial), LB_ADDSTRING, 0, (LPARAM)datos);
			else
				SendMessageA(GetDlgItem(hWnd, IDC_ToEspecial), LB_INSERTSTRING, pos, (LPARAM)datos);
	}

	void Especialidad::MostrarDatosEdit(Especialidad* dir, HWND hWnd) {
		Especialidad* aux = dir;
		SetWindowTextA(GetDlgItem(hWnd, IDC_IDEspecial), dir->clave);
		SetWindowTextA(GetDlgItem(hWnd, IDC_NomEspecial), dir->Nombre);
		SetWindowTextA(GetDlgItem(hWnd, IDC_DesEspecial), dir->DescripcionEsp);
	}

	bool Especialidad::ChecarBorrar(Especialidad* nodo, Tree_Medico* tree) {
		if (tree != NULL) {
			nodo->ChecarBorrar(nodo, tree->RL);
			if (strcmp(nodo->Nombre, tree->Especialidad) == 0) {
				return false;
			}
			nodo->ChecarBorrar(nodo, tree->RR);
		}
		else return true;
	}

	void Especialidad::CopyNode(Especialidad* pos, Especialidad* cambio) {
		strcpy_s(pos->clave, cambio->clave);
		strcpy_s(pos->Nombre, cambio->Nombre);
		strcpy_s(pos->DescripcionEsp, cambio->DescripcionEsp);
	}