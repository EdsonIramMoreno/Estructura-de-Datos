#include "Paciente.h"
#include <strsafe.h>
#include <iostream>
#include "resource.h"
#include <fstream>

using namespace std;


void Paciente::heapify(Paciente arr[], int n, int i){
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
 
	if (left < n && strcmp(arr[left].Apellidos, arr[largest].Apellidos) > 0)
		largest = left;

	if (right < n && strcmp(arr[right].Apellidos, arr[largest].Apellidos) > 0)
		largest = right;

	if (largest != i){
		swap(&arr[i], &arr[largest]);
 
		heapify(arr, n, largest);
	}
}

void Paciente::heapSort(Paciente arr[], int n){
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
 
	for (int i = n - 1; i > 0; i--){
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}

void Paciente::FillArray(Paciente* First, char* current) {
	int count = 0;
	Paciente* aux = First;
	while (aux != 0) {
		count++;
		aux = aux->Sig;
	}
	aux->BuildArray(count, First, current);
	aux = 0;
	delete aux;
}

void Paciente::BuildArray(int cant, Paciente* nodo, char* current) {
	Paciente* ArrPac = new Paciente[cant];
	for (int i = 0; i < cant; i++) {
		ArrPac[i] = *nodo;
		nodo = nodo->Sig;
	}
	nodo->heapSort(ArrPac, cant);
	nodo->FillRegistro(ArrPac, cant, current);
	delete[] ArrPac;
}

void Paciente::FillRegistro(Paciente arr[], int count, char* current) {
	char archivo[120] = { '\0' };
	strcpy_s(archivo, current);
	strcat(archivo, "\\EstructuraDatos\\RegistroPacientes.txt");
	ofstream FillTxt;
	SYSTEMTIME hoy;
	RtlZeroMemory(&hoy, sizeof(hoy));
	GetLocalTime(&hoy);
	int i = 0;
	FillTxt.open(archivo, ios::out);
	if (FillTxt.is_open()) {
		FillTxt << "El registro de los datos del día: " << hoy.wDay << "/" << hoy.wMonth << "/" << hoy.wYear << " es:" << endl;
		while (i < count) {
			FillTxt << "Nombres: " << arr[i].Nombre;
			FillTxt << "\t\tApellidos: " << arr[i].Apellidos << endl;
			FillTxt << "Genero: ";;
			if (arr[i].Genero == 1)
				FillTxt << "Masculino";
			else
				FillTxt << "Femenino";
			FillTxt << "\t\t\tFecha de nacimiento: " << arr[i].fechaNa << endl;
			FillTxt <<"----------------------------------------------------------" << endl;
			i++;
		}
	}
	FillTxt.close();
}

void Paciente::swap(Paciente* actual, Paciente* Mov) {
	Paciente* aux = new Paciente;
	actual->CopyNode(aux, actual);
	actual->CopyNode(actual, Mov);
	actual->CopyNode(Mov, aux);
	delete aux;
}

Paciente* Paciente::SerchPaciente(Paciente Buscar, Paciente* first) {
	Paciente* aux = first;
	while (aux != 0) {
		if (strcmp(Buscar.Apellidos, aux->Apellidos) == 0) {
			if (strcmp(Buscar.Nombre, aux->Nombre) == 0) {
				return aux;
			}
		}
		aux = aux->Sig;
	}
	aux = 0;
	delete aux;
	return 0;
}

char* Paciente::CalcularEdad(char* FechaNa) {
	char Fecha[15] = {'\0'};
	int MesA, YearA, DiaA, MesN, YearN, DiaN, Year, Mes, Dia;
	short parentesis = 0, j = 0;
	SYSTEMTIME hoy;

	ZeroMemory(&hoy, sizeof(SYSTEMTIME));
	GetLocalTime(&hoy);
	size_t cbDest = 10 * sizeof(char);
	StringCbPrintfA(Fecha, cbDest, "%d", hoy.wMonth);
	MesA = atoi(Fecha);
	StringCbPrintfA(Fecha, cbDest, "%d", hoy.wYear);
	YearA = atoi(Fecha);
	StringCbPrintfA(Fecha, cbDest, "%d", hoy.wDay);
	DiaA = atoi(Fecha);
	ZeroMemory(&Fecha, strlen(Fecha));
	for (short i = 0; i < 15; i++) {
		if (FechaNa[i] == '/')
			parentesis++;
		if (parentesis == 0) {
			if (isdigit(FechaNa[i])) {
				Fecha[j] = FechaNa[i];
				j++;
			}
		}
		if (parentesis == 1)
			break;
	}
	DiaN = atoi(Fecha);
	ZeroMemory(&Fecha, sizeof(&Fecha));
	parentesis = 0;
	j = 0;
	for (short i = 0; i < 15; i++) {
		if (FechaNa[i] == '/')
			parentesis++;
		if (parentesis == 1) {
			if (isdigit(FechaNa[i])) {
				Fecha[j] = FechaNa[i];
				j++;
			}
		}
		if (parentesis == 2)
			break;
	}
	MesN = atoi(Fecha);
	ZeroMemory(&Fecha, sizeof(&Fecha));
	parentesis = 0;
	j = 0;
	for (short i = 0; i < strlen(FechaNa); i++) {
		if (FechaNa[i] == '/')
			parentesis++;
		if (parentesis == 2) {
			if (isdigit(FechaNa[i])) {
				Fecha[j] = FechaNa[i];
				j++;
			}
		}
		if (j == 4) {
			break;
		}
	}
	YearN = atoi(Fecha);
	ZeroMemory(&Fecha, sizeof(&Fecha));
	Year = YearA - YearN;
	if (Year <= 1) {
		if((MesA <= MesN) && (Year == 1)){
			StringCbPrintfA(Fecha, cbDest, "%d años", Year);
		}
		else {
			Mes = MesA - MesN;
			if(Mes < 0){
				Mes = 12 - MesN;
				Mes += MesA;
				StringCbPrintfA(Fecha, cbDest, "%d meses", Mes);
			}
			else if (Mes < 1) {
				Dia = DiaA - DiaN;
				if (Dia == 0 || Dia ==1) 
					StringCbPrintfA(Fecha, cbDest, "%d día", 1);
				else
				StringCbPrintfA(Fecha, cbDest, "%d días", Dia);
			}
			else
				StringCbPrintfA(Fecha, cbDest, "%d meses", Mes);
		}
	}
	else
		StringCbPrintfA(Fecha, cbDest, "%d años", Year);
	return Fecha;
}

bool Paciente::searchCita(Paciente Pac, Citas* Lista) {
	Citas* aux = Lista;
	while (aux != 0) {
		if (strcmp(Pac.Apellidos, aux->ApellidoP) == 0) {
			if (strcmp(Pac.Nombre, aux->NombreP) == 0)
				aux = 0;
				delete aux;
				return false;
		}
		aux = aux->Sig;
	}
	aux = 0;
	delete aux;
	return true;
}

void Paciente::CopyNode(Paciente* pos, Paciente* cambio) {
	strcpy_s(pos->Apellidos, cambio->Apellidos);
	strcpy_s(pos->Nombre, cambio->Nombre);
	strcpy_s(pos->fechaNa, cambio->fechaNa);
	strcpy_s(pos->MedPas, cambio->MedPas);
	strcpy_s(pos->Referencia, cambio->Referencia);
	strcpy_s(pos->clave, cambio->clave);
	strcpy_s(pos->Edad, cambio->Edad);
	pos->Genero = cambio->Genero;
}