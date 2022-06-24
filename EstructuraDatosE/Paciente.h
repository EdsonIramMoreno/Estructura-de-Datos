#ifndef _Paciente
#define _Paciente
#include <windows.h>
#include "Citas.h"

class Paciente{
	void BuildArray(int, Paciente*, char*);

	void heapSort(Paciente arr[], int n);

	void FillRegistro(Paciente ArrPac[], int cant, char*);

	void swap(Paciente* actual, Paciente* Mov);

	void heapify(Paciente arr[], int n, int i);
public:
	char Apellidos[30];
	char Nombre[30];
	char fechaNa[20];
	short Genero;
	char MedPas[50];
	char Referencia[70];
	char clave[12];//Numero
	char Edad[11];
	Paciente* Sig;
	Paciente* Prev;

	char* CalcularEdad(char* FechaNa);

	void FillArray(Paciente* First, char*);

	Paciente* SerchPaciente(Paciente Buscar, Paciente* first);

	bool searchCita(Paciente Pac, Citas* Lista);

	void CopyNode(Paciente*, Paciente*);
};

#endif