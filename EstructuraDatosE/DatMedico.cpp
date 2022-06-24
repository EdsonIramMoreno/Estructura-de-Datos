#include "DatMedico.h"
#include <strsafe.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

	void Tree_Medico::CopyNode(Tree_Medico* nodo, Tree_Medico dato) {
		for(short i=0;i<7;i++)
		nodo->DiasAt[i] = dato.DiasAt[i];
		nodo->NumConsultorio = dato.NumConsultorio;
		strcpy_s(nodo->clave, dato.clave);
		strcpy_s(nodo->Especialidad, dato.Especialidad);
		strcpy_s(nodo->Nombre, dato.Nombre);
		strcpy_s(nodo->NumeroCel, dato.NumeroCel);
		strcpy_s(nodo->HorarioAt[0], dato.HorarioAt[0]);
		strcpy_s(nodo->HorarioAt[1], dato.HorarioAt[1]);
		strcpy_s(nodo->FotoMedico, dato.FotoMedico);
		char HoE[3] = { '\0' }, HoS[3] = { '\0' };
		for (short i = 0; i < 3; i++) {
			if (isdigit(dato.HorarioAt[0][i + 1]))
				HoE[i] = dato.HorarioAt[0][i + 1];
			if (isdigit(dato.HorarioAt[1][i + 1]))
				HoS[i] = dato.HorarioAt[1][i + 1];
		}
		nodo->RR = 0;
		nodo->RL = 0;
		nodo->PP = 0;
	}

	void Tree_Medico::CopyNodeS(Tree_Medico* nodo, Tree_Medico* dato) {
		for (short i = 0; i < 7; i++)
			nodo->DiasAt[i] = dato->DiasAt[i];
		nodo->NumConsultorio = dato->NumConsultorio;
		strcpy_s(nodo->clave, dato->clave);
		strcpy_s(nodo->Especialidad, dato->Especialidad);
		strcpy_s(nodo->Nombre, dato->Nombre);
		strcpy_s(nodo->NumeroCel, dato->NumeroCel);
		strcpy_s(nodo->HorarioAt[0], dato->HorarioAt[0]);
		strcpy_s(nodo->HorarioAt[1], dato->HorarioAt[1]);
		strcpy_s(nodo->FotoMedico, dato->FotoMedico);
		/*char HoE[3] = { '\0' }, HoS[3] = { '\0' };
		for (short i = 0; i < 3; i++) {
			if (isdigit(nodo.HorarioAt[0][i + 1]))
				HoE[i] = nodo.HorarioAt[0][i + 1];
			if (isdigit(nodo.HorarioAt[1][i + 1]))
				HoS[i] = nodo.HorarioAt[1][i + 1];
		}*/
	}

	void Tree_Medico::CopyNodeDel(Tree_Medico* nodo, Tree_Medico* datoN) {
		for (short i = 0; i < 7; i++) {
			nodo->DiasAt[i] = datoN->DiasAt[i];
		}
		nodo->NumConsultorio = datoN->NumConsultorio;
		strcpy_s(nodo->clave, datoN->clave);
		strcpy_s(nodo->Especialidad, datoN->Especialidad);
		strcpy_s(nodo->Nombre, datoN->Nombre);
		strcpy_s(nodo->NumeroCel, datoN->NumeroCel);
		strcpy_s(nodo->HorarioAt[0], datoN->HorarioAt[0]);
		strcpy_s(nodo->HorarioAt[1], datoN->HorarioAt[1]);
		strcpy_s(nodo->FotoMedico, datoN->FotoMedico);

	}

	void Tree_Medico::ModifyNode(Tree_Medico* nodo, Tree_Medico dato) {
		for (short i = 0; i < 7; i++)
			nodo->DiasAt[i] = dato.DiasAt[i];
		nodo->NumConsultorio = dato.NumConsultorio;
		strcpy_s(nodo->clave, dato.clave);
		strcpy_s(nodo->Especialidad, dato.Especialidad);
		strcpy_s(nodo->Nombre, dato.Nombre);
		strcpy_s(nodo->NumeroCel, dato.NumeroCel);
		strcpy_s(nodo->HorarioAt[0], dato.HorarioAt[0]);
		strcpy_s(nodo->HorarioAt[1], dato.HorarioAt[1]);
		strcpy_s(nodo->FotoMedico, dato.FotoMedico);
	}

	void Tree_Medico::FillDocMed(Tree_Medico* First, char current[]) {
		char FileMed[100] = { '\0' };
		strcpy_s(FileMed, current);
		strcat_s(FileMed, "\\EstructuraDatos\\RegistroMedicos.txt");
		ofstream FillTxt;
		SYSTEMTIME hoy;
		RtlZeroMemory(&hoy, sizeof(hoy));
		GetLocalTime(&hoy);
		Tree_Medico* aux = First;
		FillTxt.open(FileMed, ios::out);
		if (FillTxt.is_open()) {
			FillTxt << "El registro de los datos del día: " << hoy.wDay << "/" << hoy.wMonth << "/" << hoy.wYear << " es:" << endl;
			while (aux != NULL) {
				FillTxt << "Cedula profesional: " << aux->clave;
				FillTxt << "\t\t\tNombre: " << aux->Nombre << endl;
				FillTxt << "Especialidad: " << aux->Especialidad;
				FillTxt << "\t\t\tNumero de telefono: " << aux->NumeroCel << endl;
				FillTxt << endl;
				aux = aux->sig;
			}
		}
		FillTxt.close();
		aux = 0;
		delete aux;
	}

	bool Tree_Medico::ValidarHorario(char Horario[2][7]) {
		char HoE[3] = { '\0' }, HoS[3] = { '\0' };
		for (short i = 0; i < 3; i++) {
			if (isdigit(Horario[0][i + 1]))
				HoE[i] = Horario[0][i + 1];
			if (isdigit(Horario[1][i + 1]))
				HoS[i] = Horario[1][i + 1];
		}
		int HorE = atoi(HoE);
		int HorS = atoi(HoS);
		HorS -= 4;
		if (HorE <= HorS) {
			return true;
		}
		else return false;
	}

	bool Tree_Medico::ValidarDias(bool* Dias) {
		short di = 0;
		for (short i = 0; i < 7; i++) {
			if (Dias[i] == true) {
				di++;
				if (di == 3)
					return true;
			}
		}
		return false;
	}

	bool Tree_Medico::validarHorarioMedicos(Tree_Medico* arbol, Tree_Medico dato) {
		if (arbol != NULL) {
			if (strcmp(arbol->clave, dato.clave) != 0) {
				if (!validarHorarioMedicos(arbol->RL, dato))
					return false;
				bool checar = true;
				for (short i = 0; i < 7; i++) {
					if (arbol->DiasAt[i] == true && dato.DiasAt[i] == true) {
						checar = false;
						break;
					}
				}
				if (!checar) {
					if (arbol->NumConsultorio == dato.NumConsultorio) {
						char HoET[3] = { '\0' }, HoST[3] = { '\0' };
						char HoED[3] = { '\0' }, HoSD[3] = { '\0' };
						for (short i = 0; i < 3; i++) {
							if (isdigit(arbol->HorarioAt[0][i + 1]))
								HoET[i] = arbol->HorarioAt[0][i + 1];
							if (isdigit(arbol->HorarioAt[1][i + 1]))
								HoST[i] = arbol->HorarioAt[1][i + 1];
						}
						for (short i = 0; i < 3; i++) {
							if (isdigit(dato.HorarioAt[0][i + 1]))
								HoED[i] = dato.HorarioAt[0][i + 1];
							if (isdigit(dato.HorarioAt[1][i + 1]))
								HoSD[i] = dato.HorarioAt[1][i + 1];
						}
						int HorED, HorSD, HorET, HorST;
						HorED = atoi(HoED);
						HorSD = atoi(HoSD);
						HorET = atoi(HoET);
						HorST = atoi(HoST);
						if ((HorET < HorED) && (HorST > HorED)) {
							return false;
						}
						else if ((HorET < HorSD) && (HorST > HorSD)) {
							return false;
						}
						else if ((HorET >= HorED) && (HorST <= HorSD)) {
							return false;
						}
					}
				}
				if (!validarHorarioMedicos(arbol->RR, dato))
					return false;
			}
			else return true;
		}
		else return true;
	}

	bool Tree_Medico::checarHorarios(Tree_Medico* nodo, Tree_Medico dato) {
		bool checar = true;
		if (strcmp(nodo->clave, dato.clave) != 0) {
			for (short i = 0; i < 7; i++) {
				if (nodo->DiasAt[i] == true && dato.DiasAt[i] == true) {
					checar = false;
					break;
				}
			}
			if (!checar) {
				if (nodo->NumConsultorio == dato.NumConsultorio) {
					char HoET[3] = { '\0' }, HoST[3] = { '\0' };
					char HoED[3] = { '\0' }, HoSD[3] = { '\0' };
					for (short i = 0; i < 3; i++) {
						if (isdigit(nodo->HorarioAt[0][i + 1]))
							HoET[i] = nodo->HorarioAt[0][i + 1];
						if (isdigit(nodo->HorarioAt[1][i + 1]))
							HoST[i] = nodo->HorarioAt[1][i + 1];
					}
					for (short i = 0; i < 3; i++) {
						if (isdigit(dato.HorarioAt[0][i + 1]))
							HoED[i] = dato.HorarioAt[0][i + 1];
						if (isdigit(dato.HorarioAt[1][i + 1]))
							HoSD[i] = dato.HorarioAt[1][i + 1];
					}
					//Checar porque no entra a esta parte del codigo
					int HorED, HorSD, HorET, HorST;
					HorED = atoi(HoED);
					HorSD = atoi(HoSD);
					HorET = atoi(HoET);
					HorST = atoi(HoST);
					if ((HorET < HorED) && (HorST > HorED)) {
						return false;
					}
					else if ((HorET < HorSD) && (HorST > HorSD)) {
						return false;
					}
					else if ((HorET >= HorED) && (HorST <= HorSD)) {
						return false;
					}
				}
			}
		}
		else return true;
	}

	void Tree_Medico::SetMedicos(Tree_Medico* nodo, char* Esp, bool Sel, HWND hWnd, int Combo) {
		char Medico[80] = { '\0' };
		if (nodo != NULL) {
			SetMedicos(nodo->RL, Esp, Sel, hWnd, Combo);
			if (Sel) {
				StringCchPrintfA(Medico, 34, " %s: %s", nodo->clave, nodo->Nombre);
				SendMessageA(GetDlgItem(hWnd, Combo), CB_ADDSTRING, 0, (LPARAM)Medico);
			}
			else {
				if (strcmp(Esp, nodo->Especialidad) == 0) {
					StringCchPrintfA(Medico, 34, " %s: %s", nodo->clave, nodo->Nombre);
					SendMessageA(GetDlgItem(hWnd, Combo), CB_ADDSTRING, 0, (LPARAM)Medico);
				}
			}
			SetMedicos(nodo->RR, Esp, Sel, hWnd, Combo);
		}
	}