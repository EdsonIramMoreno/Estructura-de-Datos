#ifndef _Citas
#define _Citas
#include <Windows.h>

class Citas {
	int EstadoCita;
public:
	char ApellidoP[30];//YA
	char NombreP[30];//YA
	char FechaC[11];//YA
	char clave[11];//YA
	char HoraC[6];//YA
	char ClaveCita[20];
	char ClaveUsuario[10];
	char Especialidad[20];
	char Cancelar[30] = {'\0'};
	Citas* Sig;
	Citas* Prev;

	void setEstadoCita(int Estado);

	int getEstadoCita();

	Citas* SearchNodo(char* Clave, Citas* first);

	void CopyNode(Citas* Cit, Citas* Nodo);

	void SetCitasMonth(HWND, Citas*, Citas);

	void SetCitasSemanaEsp(int dias[2][2], HWND, Citas*, char*);

	void SetCitasSemanaRegistroEsp(int dias[2][2], HWND, Citas*, char*, char[], const char*);

	void SetCitasSemanaRegistro(int dias[2][2], HWND, Citas*, char[], const char*);

	void SetCitasSemana(int dias[2][2], HWND, Citas*);

	void SetCitasMonthRegistro(HWND, Citas*, Citas, char[], const char*);

	bool ValCitMed(char*, Citas*);

	bool ValCitMed(Citas, Citas*);

	void CleanNode(Citas* Cit);
};

#endif