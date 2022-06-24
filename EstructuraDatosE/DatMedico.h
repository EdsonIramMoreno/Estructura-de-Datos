#ifndef _DatMedico
#define _DatMedico
#include <windows.h>

class Tree_Medico{
public:
	char Nombre[50]; //Check
	char clave[11]; //Check 15
	bool DiasAt[7];
	char FotoMedico[100];
	char HorarioAt[2][7];//Check
	char NumeroCel[12];//Check
	char Especialidad[20];//Check
	short NumConsultorio;//Check
	Tree_Medico* RR;
	Tree_Medico* RL;
	Tree_Medico* PP;
	Tree_Medico* sig = 0;
	Tree_Medico* prev = 0;

	void CopyNode(Tree_Medico* nodo, Tree_Medico dato);

	void CopyNodeDel(Tree_Medico* nodo, Tree_Medico* datoN);

	void ModifyNode(Tree_Medico* nodo, Tree_Medico dato);

	void FillDocMed(Tree_Medico* First, char current[]);
	
	bool checarHorarios(Tree_Medico*, Tree_Medico);

	bool validarHorarioMedicos(Tree_Medico*, Tree_Medico);

	bool ValidarDias(bool*);

	bool ValidarHorario(char Horario[2][7]);

	void SetMedicos(Tree_Medico*, char*, bool, HWND, int);

	void CopyNodeS(Tree_Medico* nodo, Tree_Medico* dato);
};

#endif