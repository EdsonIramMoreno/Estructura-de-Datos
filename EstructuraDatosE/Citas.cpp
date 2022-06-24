#include "Citas.h"
#include <string.h>
#include <fstream>
#include <strsafe.h>
#include <iostream>


using namespace std;

Citas* Citas::SearchNodo(char* Clave, Citas* first) {
	Citas* aux = first;
	while (aux != NULL) {
		if (strcmp(Clave, aux->ClaveCita) == 0) {
			return aux;
		}
			aux = aux->Sig;
	}
	aux = 0;
	delete aux;
	return NULL;
}

void Citas::CopyNode(Citas* Cit, Citas* Nodo) {
	strcpy_s(Cit->ApellidoP, (Nodo)->ApellidoP);
	strcpy_s(Cit->ClaveCita, (Nodo)->ClaveCita);
	strcpy_s(Cit->ClaveUsuario, (Nodo)->ClaveUsuario);
	strcpy_s(Cit->clave, (Nodo)->clave);
	strcpy_s(Cit->FechaC, (Nodo)->FechaC);
	strcpy_s(Cit->HoraC, (Nodo)->HoraC);
	strcpy_s(Cit->NombreP, (Nodo)->NombreP);
	strcpy_s(Cit->Especialidad, (Nodo)->Especialidad);
	strcpy_s(Cit->Cancelar, (Nodo)->Cancelar);
	Cit->setEstadoCita((Nodo)->getEstadoCita());
}

void Citas::CleanNode(Citas* Cit) {
	strcpy_s(Cit->ApellidoP, "");
	strcpy_s(Cit->ClaveCita, "");
	strcpy_s(Cit->ClaveUsuario, "");
	strcpy_s(Cit->clave, "");
	strcpy_s(Cit->FechaC, "");
	strcpy_s(Cit->HoraC, "");
	strcpy_s(Cit->NombreP, "");
	strcpy_s(Cit->Especialidad, "");
	strcpy_s(Cit->Cancelar, "");
	Cit->setEstadoCita(0);
}

//1.-Pendiente, 2.-Atendida 3.-Cancelada;
void Citas::setEstadoCita(int Estado) {
	if (Estado >= 1 && Estado <= 3) {
		this->EstadoCita = Estado;
	}
}

int Citas::getEstadoCita() {
	return this->EstadoCita;
}

void Citas::SetCitasMonth(HWND hWnd, Citas* first, Citas Medico) {
	Citas* aux = first;
	int slash, dato, mes, j;
	char Dato[4] = { '\0' };
	mes = atoi(Medico.FechaC);
	while (aux != 0) {
		if (strcmp(aux->clave, Medico.clave) == 0) {
			slash = 0;
			j = 0;
			for (short i = 0; i < strlen(aux->FechaC); i++) {
				if (aux->FechaC[i] == '/')
					slash++;
				if (slash == 1) {
					if (isdigit(aux->FechaC[i])) {
						Dato[j] = aux->FechaC[i];
						j++;
					}
				}
				if (slash == 2)
					break;
			}
			dato = atoi(Dato);
			Dato[1] = '\0';
			if (dato == mes) {
				char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
				if (aux->getEstadoCita() == 1)
					strcpy_s(EstadoCita, "No atendida");
				else if (aux->getEstadoCita() == 2)
					strcpy_s(EstadoCita, "Atendida");
				else
					strcpy_s(EstadoCita, "Cancelada");
				StringCbPrintfA(Datos, 60, "%s / %s / %s    %s", aux->ClaveCita, aux->FechaC, aux->NombreP, EstadoCita);
				SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)Datos);
			}
		}
		aux = aux->Sig;
	}
	Dato[3] = '\0';
	aux = 0;
	delete aux;
}

void Citas::SetCitasMonthRegistro(HWND hWnd, Citas* first, Citas Medico, char current[], const char* dir) {
	char FileCitasMes[100] = { '\0' };
	strcpy_s(FileCitasMes, current);
	strcat_s(FileCitasMes, dir);
	ofstream FillCitasMesTxt;
	SYSTEMTIME hoy;
	RtlZeroMemory(&hoy, sizeof(hoy));
	GetLocalTime(&hoy);
	FillCitasMesTxt.open(FileCitasMes, ios::out);
	if (FillCitasMesTxt.is_open()) {
		Citas* aux = first;
		int slash, dato, mes, j;
		char Dato[4] = { '\0' };
		mes = atoi(Medico.FechaC);
		FillCitasMesTxt << "El registro de los datos del día: " << hoy.wDay << "/" << hoy.wMonth << "/" << hoy.wYear << " es:" << endl;
		while (aux != 0) {
			if (strcmp(aux->clave, Medico.clave) == 0) {
				slash = 0;
				j = 0;
				for (short i = 0; i < strlen(aux->FechaC); i++) {
					if (aux->FechaC[i] == '/')
						slash++;
					if (slash == 1) {
						if (isdigit(aux->FechaC[i])) {
							Dato[j] = aux->FechaC[i];
							j++;
						}
					}
					if (slash == 2)
						break;
				}
				dato = atoi(Dato);
				Dato[1] = '\0';
				if (dato == mes) {
					char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
					FillCitasMesTxt << "ID Cita: " << aux->ClaveCita << endl;
					FillCitasMesTxt << "Clave medico: " << aux->clave << "               Consultorio: " << aux->ClaveCita[0] << endl;
					FillCitasMesTxt << "Especialidad: " << aux->Especialidad;
					FillCitasMesTxt << "Nombre del paciente: " << aux->NombreP << " " << aux->ApellidoP << endl;
					FillCitasMesTxt << "Fecha: " << aux->FechaC << "      " << "Hora cita: " << aux->HoraC << endl;
					FillCitasMesTxt << "Estado cita: ";
					if (aux->getEstadoCita() == 1)
						FillCitasMesTxt << "No atendida";
					else if (aux->getEstadoCita() == 2)
						FillCitasMesTxt << "Atendida";
					else {
						FillCitasMesTxt << "Cancelada" << endl;
						FillCitasMesTxt << "Motivo: " << aux->Cancelar;
					}
					FillCitasMesTxt << endl << "------------------------------------------------------" << endl;
				}
			}
			aux = aux->Sig;
		}
		Dato[3] = '\0';
		FillCitasMesTxt.close();
		aux = 0;
		delete aux;
	}
	char direccion[120] = { '\0' };
	StringCchPrintfA(direccion, 120, "La dirección del registro es: %s", FileCitasMes);
	MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
}

void Citas::SetCitasSemanaEsp(int dias[2][2], HWND hWnd, Citas* first, char* esp) {
	Citas* aux = first;
	char Dato[5] = { '\0' };
	int datoD, datoM;
	short j, slash;
	while (aux != 0) {
		if (strcmp(aux->Especialidad, esp) == 0) {
			j = 0;
			slash = 0;
			for (short i = 0; i < strlen(aux->FechaC); i++) {
				if (aux->FechaC[i] == '/')
					slash++;
				if (slash == 0) {
					if (isdigit(aux->FechaC[i])) {
						Dato[j] = aux->FechaC[i];
						j++;
					}
				}
				if (slash == 1)
					break;
			}
			datoD = atoi(Dato);
			Dato[1] = '\0';
			slash = 0;
			j = 0;
			for (short i = 0; i < strlen(aux->FechaC); i++) {
				if (aux->FechaC[i] == '/')
					slash++;
				if (slash == 1) {
					if (isdigit(aux->FechaC[i])) {
						Dato[j] = aux->FechaC[i];
						j++;
					}
				}
				if (slash == 2)
					break;
			}
			datoM = atoi(Dato);
			Dato[1] = '\0';
			if (dias[0][0] <= datoM && dias[0][1] >= datoM) {
				if (dias[0][0] == dias[0][1]) {
					if (dias[1][0] <= datoD && dias[1][1] >= datoD) {
						char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
						if (aux->getEstadoCita() == 1)
							strcpy_s(EstadoCita, "No atendida");
						else if (aux->getEstadoCita() == 2)
							strcpy_s(EstadoCita, "Atendida");
						else
							strcpy_s(EstadoCita, "Cancelada");
						StringCbPrintfA(Datos, 60, "%s / %s / %s    %s", aux->ClaveCita, aux->FechaC, aux->NombreP, EstadoCita);
						SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)Datos);
					}
				}
				else if (dias[0][0] <= dias[0][1]) {
					if (dias[1][0] <= datoD || dias[1][1] >= datoD) {
						char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
						if (aux->getEstadoCita() == 1)
							strcpy_s(EstadoCita, "No atendida");
						else if (aux->getEstadoCita() == 2)
							strcpy_s(EstadoCita, "Atendida");
						else
							strcpy_s(EstadoCita, "Cancelada");
						StringCbPrintfA(Datos, 60, "%s / %s / %s    %s", aux->ClaveCita, aux->FechaC, aux->NombreP, EstadoCita);
						SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)Datos);
					}
				}
			}
		}
		aux = aux->Sig;
	}
	aux = 0;
	delete aux;
}

void Citas::SetCitasSemanaRegistroEsp(int dias[2][2], HWND hWnd, Citas* first, char* esp, char current[], const char* dir) {
	char FileCitasSem[100] = { '\0' };
	strcpy_s(FileCitasSem, current);
	strcat_s(FileCitasSem, dir);
	ofstream FillCitasSemTxt;
	SYSTEMTIME hoy;
	RtlZeroMemory(&hoy, sizeof(hoy));
	GetLocalTime(&hoy);
	FillCitasSemTxt.open(FileCitasSem, ios::out);
	Citas* aux = first;
	char Dato[5] = { '\0' };
	int datoD, datoM;
	short j, slash;
	if (FillCitasSemTxt.is_open()) {
		while (aux != 0) {
			if (strcmp(aux->Especialidad, esp) == 0) {
				j = 0;
				slash = 0;
				for (short i = 0; i < strlen(aux->FechaC); i++) {
					if (aux->FechaC[i] == '/')
						slash++;
					if (slash == 0) {
						if (isdigit(aux->FechaC[i])) {
							Dato[j] = aux->FechaC[i];
							j++;
						}
					}
					if (slash == 1)
						break;
				}
				datoD = atoi(Dato);
				Dato[1] = '\0';
				slash = 0;
				j = 0;
				for (short i = 0; i < strlen(aux->FechaC); i++) {
					if (aux->FechaC[i] == '/')
						slash++;
					if (slash == 1) {
						if (isdigit(aux->FechaC[i])) {
							Dato[j] = aux->FechaC[i];
							j++;
						}
					}
					if (slash == 2)
						break;
				}
				datoM = atoi(Dato);
				Dato[1] = '\0';
				if (dias[0][0] <= datoM && dias[0][1] >= datoM) {
					if (dias[0][0] == dias[0][1]) {
						if (dias[1][0] <= datoD && dias[1][1] >= datoD) {
							FillCitasSemTxt << "ID Cita: " << aux->ClaveCita << endl;
							FillCitasSemTxt << "Clave medico: " << aux->clave << "               Consultorio: " << aux->ClaveCita[0] << endl;
							FillCitasSemTxt << "Especialidad: " << aux->Especialidad;
							FillCitasSemTxt << "Nombre del paciente: " << aux->NombreP << " " << aux->ApellidoP << endl;
							FillCitasSemTxt << "Fecha: " << aux->FechaC << "      " << "Hora cita: " << aux->HoraC << endl;
							FillCitasSemTxt << "Estado cita: ";
							if (aux->getEstadoCita() == 1)
								FillCitasSemTxt << "No atendida";
							else if (aux->getEstadoCita() == 2)
								FillCitasSemTxt << "Atendida";
							else {
								FillCitasSemTxt << "Cancelada" << endl;
								FillCitasSemTxt << "Motivo: " << aux->Cancelar;
							}
							FillCitasSemTxt << endl << "------------------------------------------------------";
						}
					}
					else if (dias[0][0] <= dias[0][1]) {
						if (dias[1][0] <= datoD || dias[1][1] >= datoD) {
							FillCitasSemTxt << "ID Cita: " << aux->ClaveCita << endl;
							FillCitasSemTxt << "Clave medico: " << aux->clave << "               Consultorio: " << aux->ClaveCita[0] << endl;
							FillCitasSemTxt << "Especialidad: " << aux->Especialidad;
							FillCitasSemTxt << "Nombre del paciente: " << aux->NombreP << " " << aux->ApellidoP << endl;
							FillCitasSemTxt << "Fecha: " << aux->FechaC << "      " << "Hora cita: " << aux->HoraC << endl;
							FillCitasSemTxt << "Estado cita: ";
							if (aux->getEstadoCita() == 1)
								FillCitasSemTxt << "No atendida";
							else if (aux->getEstadoCita() == 2)
								FillCitasSemTxt << "Atendida";
							else {
								FillCitasSemTxt << "Cancelada" << endl;
								FillCitasSemTxt << "Motivo: " << aux->Cancelar;
							}
							FillCitasSemTxt << endl << "------------------------------------------------------";
						}
					}
				}
			}
			aux = aux->Sig;
		}
	}
	char direccion[120] = { '\0' };
	StringCchPrintfA(direccion, 120, "La dirección del registro es: %s", FileCitasSem);
	MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
	aux = 0;
	delete aux;
}

void Citas::SetCitasSemanaRegistro(int dias[2][2], HWND hWnd, Citas* first, char current[], const char* dir) {
	char FileCitasSem[100] = { '\0' };
	strcpy_s(FileCitasSem, current);
	strcat_s(FileCitasSem, dir);
	ofstream FillCitasSemTxt;
	SYSTEMTIME hoy;
	RtlZeroMemory(&hoy, sizeof(hoy));
	GetLocalTime(&hoy);
	FillCitasSemTxt.open(FileCitasSem, ios::out);
	Citas* aux = first;
	char Dato[5] = { '\0' };
	int datoD, datoM;
	short j, slash;
	if (FillCitasSemTxt.is_open()) {
		while (aux != 0) {
			j = 0;
			slash = 0;
			for (short i = 0; i < strlen(aux->FechaC); i++) {
				if (aux->FechaC[i] == '/')
					slash++;
				if (slash == 0) {
					if (isdigit(aux->FechaC[i])) {
						Dato[j] = aux->FechaC[i];
						j++;
					}
				}
				if (slash == 1)
					break;
			}
			datoD = atoi(Dato);
			Dato[1] = '\0';
			slash = 0;
			j = 0;
			for (short i = 0; i < strlen(aux->FechaC); i++) {
				if (aux->FechaC[i] == '/')
					slash++;
				if (slash == 1) {
					if (isdigit(aux->FechaC[i])) {
						Dato[j] = aux->FechaC[i];
						j++;
					}
				}
				if (slash == 2)
					break;
			}
			datoM = atoi(Dato);
			Dato[1] = '\0';
			if (dias[0][0] <= datoM && dias[0][1] >= datoM) {
				if (dias[0][0] == dias[0][1]) {
					if (dias[1][0] <= datoD && dias[1][1] >= datoD) {
						char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
						FillCitasSemTxt << "ID Cita: " << aux->ClaveCita << endl;
						FillCitasSemTxt << "Clave medico: " << aux->clave << "               Consultorio: " << aux->ClaveCita[0] << endl;
						FillCitasSemTxt << "Especialidad: " << aux->Especialidad;
						FillCitasSemTxt << "Nombre del paciente: " << aux->NombreP << " " << aux->ApellidoP << endl;
						FillCitasSemTxt << "Fecha: " << aux->FechaC << "      " << "Hora cita: " << aux->HoraC << endl;
						FillCitasSemTxt << "Estado cita: ";
						if (aux->getEstadoCita() == 1)
							FillCitasSemTxt << "No atendida";
						else if (aux->getEstadoCita() == 2)
							FillCitasSemTxt << "Atendida";
						else {
							FillCitasSemTxt << "Cancelada" << endl;
							FillCitasSemTxt << "Motivo: " << aux->Cancelar;
						}
						FillCitasSemTxt << endl << "------------------------------------------------------";
					}
				}
				else if (dias[0][0] <= dias[0][1]) {
					if (dias[1][0] <= datoD || dias[1][1] >= datoD) {
						char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
						FillCitasSemTxt << "ID Cita: " << aux->ClaveCita << endl;
						FillCitasSemTxt << "Clave medico: " << aux->clave << "               Consultorio: " << aux->ClaveCita[0] << endl;
						FillCitasSemTxt << "Especialidad: " << aux->Especialidad;
						FillCitasSemTxt << "Nombre del paciente: " << aux->NombreP << " " << aux->ApellidoP << endl;
						FillCitasSemTxt << "Fecha: " << aux->FechaC << "      " << "Hora cita: " << aux->HoraC << endl;
						FillCitasSemTxt << "Estado cita: ";
						if (aux->getEstadoCita() == 1)
							FillCitasSemTxt << "No atendida";
						else if (aux->getEstadoCita() == 2)
							FillCitasSemTxt << "Atendida";
						else {
							FillCitasSemTxt << "Cancelada" << endl;
							FillCitasSemTxt << "Motivo: " << aux->Cancelar;
						}
						FillCitasSemTxt << endl << "------------------------------------------------------";
					}
				}
			}
			aux = aux->Sig;
		}
	}
	char direccion[120] = { '\0' };
	StringCchPrintfA(direccion, 120, "La dirección del registro es: %s", FileCitasSem);
	MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
	aux = 0;
	delete aux;
}

void Citas::SetCitasSemana(int dias[2][2], HWND hWnd, Citas* first) {
	Citas* aux = first;
	char Dato[5] = { '\0' };
	int datoD, datoM;
	short j, slash;
	while (aux != 0) {
		j = 0;
		slash = 0;
		for (short i = 0; i < strlen(aux->FechaC); i++) {
			if (aux->FechaC[i] == '/')
				slash++;
			if (slash == 0) {
				if (isdigit(aux->FechaC[i])) {
					Dato[j] = aux->FechaC[i];
					j++;
				}
			}
			if (slash == 1)
				break;
		}
		datoD = atoi(Dato);
		Dato[1] = '\0';
		slash = 0;
		j = 0;
		for (short i = 0; i < strlen(aux->FechaC); i++) {
			if (aux->FechaC[i] == '/')
				slash++;
			if (slash == 1) {
				if (isdigit(aux->FechaC[i])) {
					Dato[j] = aux->FechaC[i];
					j++;
				}
			}
			if (slash == 2)
				break;
		}
		datoM = atoi(Dato);
		Dato[1] = '\0';
		if (dias[0][0] <= datoM && dias[0][1] >= datoM) {
			if (dias[0][0] == dias[0][1]) {
				if (dias[1][0] <= datoD && dias[1][1] >= datoD) {
					char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
					if (aux->getEstadoCita() == 1)
						strcpy_s(EstadoCita, "No atendida");
					else if (aux->getEstadoCita() == 2)
						strcpy_s(EstadoCita, "Atendida");
					else
						strcpy_s(EstadoCita, "Cancelada");
					StringCbPrintfA(Datos, 60, "%s / %s / %s    %s", aux->ClaveCita, aux->FechaC, aux->NombreP, EstadoCita);
					SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)Datos);
				}
			}
			else if (dias[0][0] <= dias[0][1]) {
				if (dias[1][0] <= datoD || dias[1][1] >= datoD) {
					char Datos[60] = { '\0' }, EstadoCita[12] = { '\0' };
					if (aux->getEstadoCita() == 1)
						strcpy_s(EstadoCita, "No atendida");
					else if (aux->getEstadoCita() == 2)
						strcpy_s(EstadoCita, "Atendida");
					else
						strcpy_s(EstadoCita, "Cancelada");
					StringCbPrintfA(Datos, 60, "%s / %s / %s    %s", aux->ClaveCita, aux->FechaC, aux->NombreP, EstadoCita);
					SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)Datos);
				}
			}
		}
		aux = aux->Sig;
	}
	aux = 0;
	delete aux;
}

bool Citas::ValCitMed(Citas Medico, Citas* Nodo) {
	Citas* aux = Nodo;
	while (aux != NULL) {
		if (strcmp(Medico.clave, Nodo->clave) == 0) {
			if (strcmp(Medico.FechaC, Nodo->FechaC) == 0) {
				if (strcmp(Medico.ClaveCita, Nodo->ClaveCita) != 0) {
					if (strcmp(Medico.HoraC, Nodo->HoraC) == 0) {
						aux = 0;
						delete aux;
						return false;
					}
				}
			}
		}
		aux = aux->Sig;
	}
	aux = 0;
	delete aux;
	return true;
}

bool Citas::ValCitMed(char* ClaveM, Citas* Nodo) {
	while (Nodo != 0) {
		if (strcmp(ClaveM, Nodo->clave) == 0)
			return false;
		Nodo = Nodo->Sig;
	}
	return true;
}
