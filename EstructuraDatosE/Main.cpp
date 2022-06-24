#include <windows.h>
#include <Windows.h>
#include <Winuser.h>
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Linked_List.h"
#include "Especialidad.h"
#include "Paciente.h"
#include "DatMedico.h"
#include "Usuario.h"
#include "Citas.h"
#include "Tree.h"
#include <ctype.h>
#include <stdio.h>

using namespace std;

BOOL CALLBACK LoginProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MedicoProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EspecialidadProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PacienteProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CitasProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MostrarCitasProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool ChecarNombre(char* Nombre, int cant, int Cesp);
bool ChecarTelefonos(char* telefono, Tree_Medico* tree, Tree_Medico Actual);
bool ChecarTel(char* telefono, Tree_Medico* tree, Tree_Medico Actual);
bool ChecarCed(char* cedula, Tree_Medico* tree);
bool ChecarCedulas(char* cedula, Tree_Medico* tree);
bool ChecarDes(char* Des, short cant);
void GetSelectedDate(HWND hMonthCal, char FechaN[20]);
short GetSelectedDateCita(HWND hMonthCal, Tree_Medico* Doc,Citas* FCita);
void CvtUniAnsi(wchar_t* cad, char foto[]);
void ArchivosSelectorSave(HWND hWnd, char foto[]);
void AgregarMedicoBMP(char foto[100], HWND, int, char*);
bool ChecarSelectedDate(Citas CitaFH);
void GetSelectedDate(int hMonth, HWND hWnd, char FechaN[20]);
void GetSelectedDateW(int hMonth, HWND hWnd, int dias[2][2]);
void AgregarMenu(HWND hWnd);
bool validarDatosM(Tree_Medico* _Med, Tree_Medico* first, HWND hWnd);
void CleanMedWindowM(HWND hWnd);
bool validarDatosP(Paciente* _Pac, HWND hWnd);
void CleanPacWindowP(HWND hWnd);
bool validarDatosC(Citas Cita, Citas* first, HWND hWnd);
void CleanCitWindowC(HWND hWnd);
bool validarDatosE(Especialidad Esp, HWND hWnd, int a);
void CleanEspWindowE(HWND hWnd);
//Templates validar
template<class T>
	bool ChecarNumeros(char* Num, short cant, short posO);
template<class T>
	bool ChecarNombre(char* Nom, short cant, short posO);

//VariablesGlobales
char Esp[20] = { '\0' }, auxFoto[100] = { '\0' }, auxFoto2[100] = { '\0' }, Current[100] = { '\0' };
bool _RadioBoutton = false;
HWND _HLogin = 0, _HMedico = 0, _HEspecialidad = 0,
_HPaciente = 0, _HCitas = 0, _HMostrarCitas = 0;
short pos = -1, horasMed = 0;
Especialidad _EspTemp;
Linked_List<Especialidad>* _ListEsp;
Paciente _PacTemp;
Linked_List<Paciente>* _ListPac;
Citas _CitaTemp;
Linked_List<Citas>* _ListCitas;
Tree_Medico _MedTemp;
Tree<Tree_Medico>* _TreeMed;
Tree_Medico* auxMed;
Paciente* auxPac;
Citas* auxCitas;
Usuario usuario;
#define Menu_RegistroMedico 10
#define Menu_RegistroPaciente 20
#define Menu_VentanaMedico 30
#define Menu_VentanaPaciente 40
#define Menu_VentanaCitas 50
#define Menu_VentanaMostrarCitas 60
#define Menu_VEsp 70

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	MSG msg;
	BOOL ret;

	GetCurrentDirectoryA(99, Current);
	_TreeMed = new Tree<Tree_Medico>("\\EstructuraDatos\\Medicos.txt", Current);
	_ListCitas = new Linked_List<Citas>("\\EstructuraDatos\\Citas.txt", Current);
	_ListPac = new Linked_List<Paciente>("\\EstructuraDatos\\Paciente.txt", Current);
 	_ListEsp = new Linked_List<Especialidad>("\\EstructuraDatos\\Especialidad.txt", Current);

	_HMedico= CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_WMedicos), 0, (DLGPROC)MedicoProc, 0);
	ShowWindow(_HMedico, nCmdShow);
	
	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1)
			return -1;

		if (!IsDialogMessage(_HMedico, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	delete _ListEsp;
	delete _ListPac;
	delete _TreeMed;
	delete _ListCitas;
	delete auxMed;
	delete auxPac;
	delete auxCitas;
	/*_ListEsp->~Linked_List();
	_ListPac->~Linked_List();
	_TreeMed->~Tree();*/
	return 0;
}

BOOL CALLBACK LoginProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char Contra[20] = { '\0' };
	switch (uMsg){
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case IDC_Entrar: {
			bool a = usuario.ChecarLogin(hWnd, _RadioBoutton, IDC_UsuarioL,IDC_ContraL, IDC_ContraML);
			if (a) {
				DestroyWindow(hWnd);
				EnableWindow(_HMedico, true);
				SetWindowTextA(GetDlgItem(_HMedico, IDC_NombreUs), usuario.setNombre());
			}
			break;
		}
		case IDC_Mostrar:
			if (!_RadioBoutton) {
				GetWindowTextA(GetDlgItem(hWnd, IDC_ContraL), Contra, 19);
				ShowWindow(GetDlgItem(hWnd, IDC_ContraML), 1);
				SetWindowTextA(GetDlgItem(hWnd, IDC_ContraML), Contra);
				ShowWindow(GetDlgItem(hWnd, IDC_ContraL), 0);
				_RadioBoutton = true;
			}
			else {
				GetWindowTextA(GetDlgItem(hWnd, IDC_ContraML), Contra, 19);
				ShowWindow(GetDlgItem(hWnd, IDC_ContraL), 1);
				SetWindowTextA(GetDlgItem(hWnd, IDC_ContraL), Contra);
				ShowWindow(GetDlgItem(hWnd, IDC_ContraML), 0);
				_RadioBoutton = false;
			}
			break;
		}
		break;
	case WM_INITDIALOG:{
		ShowWindow(GetDlgItem(hWnd, IDC_ContraML), 0);
		strcpy_s(auxFoto, "\\EstructuraDatos\\FotoInicio\\Inicio.bmp");
		AgregarMedicoBMP(auxFoto, hWnd, IDC_FotoInicio, Current);
		ZeroMemory(&auxFoto, sizeof(auxFoto));
	}
	break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK MedicoProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			//Menu:
		case Menu_VentanaMostrarCitas: {
			_HMostrarCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WMostrarCitas), hWnd, (DLGPROC)MostrarCitasProc);
			ShowWindow(_HMostrarCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VEsp: {
			_HEspecialidad = CreateDialog(0, MAKEINTRESOURCE(IDD_WEspecial), hWnd, (DLGPROC)EspecialidadProc);
			ShowWindow(_HEspecialidad, SW_SHOW);
			pos = -1;
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaPaciente: {
			_HPaciente = CreateDialog(0, MAKEINTRESOURCE(IDD_WPaciente), hWnd, (DLGPROC)PacienteProc);
			ShowWindow(_HPaciente, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaCitas: {
			_HCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WCitas), hWnd, (DLGPROC)CitasProc);
			ShowWindow(_HCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_RegistroMedico: {
			_TreeMed->quickSort(_TreeMed->GetRoot(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroMedicos.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de médicos.", MB_OK);
		}
			break;
		case Menu_RegistroPaciente: {
			_PacTemp.FillArray(_ListPac->Getfirst(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroPacientes.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
		}
			break;
			//Manejo de botones
		case IDC_GuardarMedico: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_NombreMedico), _MedTemp.Nombre,49);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NumeroMedico), _MedTemp.NumeroCel, 11);
			GetWindowTextA(GetDlgItem(hWnd, IDC_CedulaMedico), _MedTemp.clave, 14);
			if (validarDatosM(&_MedTemp, _TreeMed->GetRootS(), hWnd)) {
				strcpy_s(auxFoto2, Current);
				strcat_s(auxFoto2, "\\EstructuraDatos\\Foto_Medicos\\");
				strcat_s(auxFoto2, _MedTemp.clave);
				strcat_s(auxFoto2, ".bmp");
				CopyFileA(auxFoto, auxFoto2, 1);
				StringCbPrintfA(_MedTemp.FotoMedico, 100, "EstructuraDatos\\Foto_Medicos\\%s.bmp",_MedTemp.clave);
				_TreeMed->AddTreeNode(_TreeMed->GetRoot(), _MedTemp);
			MessageBoxA(hWnd, "Medico agregado", "Medico agregado correctamente", MB_OK);
			CleanMedWindowM(hWnd);
			/////////Limpia la variable temporal////////////////
			ZeroMemory(&_MedTemp, sizeof(_MedTemp));
			ZeroMemory(&auxFoto, sizeof(auxFoto));
			ZeroMemory(&auxFoto2, sizeof(auxFoto2));
			char aux[5] = { '\0' };
			AgregarMedicoBMP(aux, hWnd, IDC_MedicoFoto, Current);
			}
			else
			MessageBoxA(hWnd, "No se pudo agregar el medico", "Error al capturar", MB_ICONERROR);
		}
			break;
		case IDC_BuscarMed: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_MedicoBu), _MedTemp.clave, 15);
			if (strcmp(_MedTemp.clave, "") != 0) {
				auxMed = _TreeMed->SearchTreeNode(_TreeMed->GetRootS(), _MedTemp);
				if (auxMed != NULL) {
					SetWindowTextA(GetDlgItem(hWnd, IDC_NombreMedico), auxMed->Nombre);
					SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroMedico), auxMed->NumeroCel);
					SetWindowTextA(GetDlgItem(hWnd, IDC_CedulaMedico), auxMed->clave);
					for (short i = 0; i < 7; i++) {
						_MedTemp.DiasAt[i] = auxMed->DiasAt[i];
					}
					if (auxMed->DiasAt[0])
						CheckDlgButton(hWnd, IDC_Domingo, BST_CHECKED);
					if (auxMed->DiasAt[1])
						CheckDlgButton(hWnd, IDC_Lunes, BST_CHECKED);
					if (auxMed->DiasAt[2])
						CheckDlgButton(hWnd, IDC_Martes, BST_CHECKED);
					if (auxMed->DiasAt[3])
						CheckDlgButton(hWnd, IDC_Miercoles, BST_CHECKED);
					if (auxMed->DiasAt[4])
						CheckDlgButton(hWnd, IDC_Jueves, BST_CHECKED);
					if (auxMed->DiasAt[5])
						CheckDlgButton(hWnd, IDC_Viernes, BST_CHECKED);
					if (auxMed->DiasAt[6])
						CheckDlgButton(hWnd, IDC_Sabado, BST_CHECKED);
					int a = 0;
					a = SendMessageW(GetDlgItem(hWnd, IDC_COMBOEsp), CB_GETCOUNT, 0, 0);
					for (int i = 0; i < a; i++) {
						SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_GETLBTEXT, i, (LPARAM)_MedTemp.Especialidad);
						if (strcmp(auxMed->Especialidad, _MedTemp.Especialidad) == 0) {
							a = i;
							break;
						}
					}
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_SETCURSEL, a, 0);
					a = SendMessageW(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_GETCOUNT, 0, 0);
					for (int i = 0; i < a; i++) {
						SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_GETLBTEXT, i, (LPARAM)_MedTemp.HorarioAt[0]);
						if (strcmp(auxMed->HorarioAt[0], _MedTemp.HorarioAt[0]) == 0) {
							a = i;
							break;
						}
					}
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_SETCURSEL, a, 0);
					a = SendMessageW(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_GETCOUNT, 0, 0);
					for (int i = 0; i < a; i++) {
						SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_GETLBTEXT, i, (LPARAM)_MedTemp.HorarioAt[1]);
						if (strcmp(auxMed->HorarioAt[1], _MedTemp.HorarioAt[1]) == 0) {
							a = i;
							break;
						}
					}
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_SETCURSEL, a, 0);
					a = SendMessageW(GetDlgItem(hWnd, IDC_COMBOCons), CB_GETCOUNT, 0, 0);
					for (int i = 0; i < a; i++) {
						SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_GETLBTEXT, i, (LPARAM)Esp);
						if (atoi(Esp)==i+1) {
							a = i;
							break;
						}
					}
					_MedTemp.NumConsultorio = a + 1;
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_SETCURSEL, a, 0);
					strcpy_s(auxFoto2, "\\");
					AgregarMedicoBMP(auxFoto2,hWnd, IDC_MedicoFoto, Current);
					EnableWindow(GetDlgItem(hWnd, IDC_CedulaMedico), false);
					ShowWindow(GetDlgItem(hWnd, IDC_EditMedico), SW_SHOW);
					ShowWindow(GetDlgItem(hWnd, IDC_EliminarMed), SW_SHOW);
					ShowWindow(GetDlgItem(hWnd, IDC_GuardarMedico), SW_HIDE);
					ZeroMemory(&auxFoto2, sizeof(auxFoto2));
				}
				else
					MessageBoxA(hWnd, "Medico no encontrado", "El medico que ingreso no esta registrado", MB_ICONERROR);
			}
		}
			break;
		case IDC_EliminarMed: {
			if (_CitaTemp.ValCitMed(_MedTemp.clave, _ListCitas->Getfirst())) {
				_TreeMed->DeleteNode(_TreeMed->SearchTreeNode(_TreeMed->GetRootS(), _MedTemp), Current);
				RtlZeroMemory(&_MedTemp, sizeof(_MedTemp));
				MessageBoxA(hWnd, "Medico eliminado", "El medico ha sido eliminado", MB_OK);
				CleanMedWindowM(hWnd);
				SetWindowTextA(GetDlgItem(hWnd, IDC_MedicoBu), "");
				ShowWindow(GetDlgItem(hWnd, IDC_EliminarMed), 0);
				ShowWindow(GetDlgItem(hWnd, IDC_EditMedico), 0);
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarMedico), 1);
				EnableWindow(GetDlgItem(hWnd, IDC_CedulaMedico), true);
				char aux[5] = { '\0' };
				AgregarMedicoBMP(aux, hWnd, IDC_MedicoFoto, Current);
			}
			else {
				MessageBoxA(hWnd, "El medico tiene citas pendientes, no puede ser eliminado.", "Medico no eliminado", MB_ICONERROR);
				CleanMedWindowM(hWnd);
				SetWindowTextA(GetDlgItem(hWnd, IDC_MedicoBu), "");
				ShowWindow(GetDlgItem(hWnd, IDC_EliminarMed), 0);
				ShowWindow(GetDlgItem(hWnd, IDC_EditMedico), 0);
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarMedico), 1);
				EnableWindow(GetDlgItem(hWnd, IDC_CedulaMedico), true);
				char aux[5] = { '\0' };
				AgregarMedicoBMP(aux, hWnd, IDC_MedicoFoto, Current);
			}
		}
			break;
		case IDC_EditMedico: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_NombreMedico), _MedTemp.Nombre, 49);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NumeroMedico), _MedTemp.NumeroCel, 11);
			GetWindowTextA(GetDlgItem(hWnd, IDC_CedulaMedico), _MedTemp.clave, 11);
			if (validarDatosM(&_MedTemp, _TreeMed->GetRootS(), hWnd)) {
				if (_CitaTemp.ValCitMed(_MedTemp.clave, _ListCitas->Getfirst())) {
					_TreeMed->ModifyTreeNode(_TreeMed->SearchTreeNode(_TreeMed->GetRootS(), _MedTemp), _MedTemp);
						RtlZeroMemory(&_MedTemp, sizeof(_MedTemp));
						MessageBoxA(hWnd, "Los datos del médico han sido actualizados", "Medico editado", MB_OK);
						CleanMedWindowM(hWnd);
						SetWindowTextA(GetDlgItem(hWnd, IDC_MedicoBu), "");
						ShowWindow(GetDlgItem(hWnd, IDC_EliminarMed), 0);
						ShowWindow(GetDlgItem(hWnd, IDC_EditMedico), 0);
						ShowWindow(GetDlgItem(hWnd, IDC_GuardarMedico), 1);
						EnableWindow(GetDlgItem(hWnd, IDC_CedulaMedico), true);
				}
				else {
					MessageBoxA(hWnd, "El medico tiene citas pendientes, no puede editar sus datos.", "Medico no editado", MB_ICONERROR);
					SetWindowTextA(GetDlgItem(hWnd, IDC_NombreMedico), "");
					SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroMedico), "");
					SetWindowTextA(GetDlgItem(hWnd, IDC_CedulaMedico), "");
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_SETCURSEL, -1, 0);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_SETCURSEL, -1, 0);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_SETCURSEL, -1, 0);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_SETCURSEL, -1, 0);
					CheckDlgButton(hWnd, IDC_Domingo, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Lunes, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Martes, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Miercoles, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Jueves, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Viernes, BST_UNCHECKED);
					CheckDlgButton(hWnd, IDC_Sabado, BST_UNCHECKED);
					SetWindowTextA(GetDlgItem(hWnd, IDC_MedicoBu), "");
					ShowWindow(GetDlgItem(hWnd, IDC_EliminarMed), 0);
					ShowWindow(GetDlgItem(hWnd, IDC_EditMedico), 0);
					ShowWindow(GetDlgItem(hWnd, IDC_GuardarMedico), 1);
					EnableWindow(GetDlgItem(hWnd, IDC_CedulaMedico), true);
				}
			}
			else {
				MessageBoxA(hWnd, "Los datos que ingreso no son validos.", "Medico no editado", MB_ICONERROR);
			}
			
		}
			break;
		case IDC_SeleFoto: {
			ArchivosSelectorSave(hWnd, auxFoto);		
			if (strcmp(auxFoto, "") != 0) {
				AgregarMedicoBMP(auxFoto, hWnd, IDC_MedicoFoto, Current);
			}
			else
				MessageBoxA(hWnd, "No hay foto seleccionada", "Error en la foto", MB_ICONERROR);
		}
			break;
			//Manejo de combobox
		case IDC_COMBOHoraI: {
				pos = SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_GETCURSEL, 0, 0);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_GETLBTEXT, pos, (LPARAM)_MedTemp.HorarioAt[0]);
		}
			break;
		case IDC_COMBOHoraF: {
			pos = SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_GETLBTEXT, pos, (LPARAM)_MedTemp.HorarioAt[1]);
		}
			break;
		case IDC_COMBOCons: {
			pos = SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_GETCURSEL, 0, 0);
			_MedTemp.NumConsultorio = pos + 1;
		}
			break;
		case IDC_COMBOEsp: {
			pos = SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_GETLBTEXT, pos, (LPARAM)_MedTemp.Especialidad);
		}
			break;
			//Manejo de CheckBox
		case IDC_Domingo: {
			if (_MedTemp.DiasAt[0])
				_MedTemp.DiasAt[0] = false;
			else
				_MedTemp.DiasAt[0] = true;
		}
			break;
		case IDC_Lunes: {
			if(_MedTemp.DiasAt[1])
				_MedTemp.DiasAt[1] = false;
			else
				_MedTemp.DiasAt[1] = true;
		}
			break;
		case IDC_Martes: {
			if (_MedTemp.DiasAt[2])
				_MedTemp.DiasAt[2] = false;
			else
				_MedTemp.DiasAt[2] = true;
		}
			break;
		case IDC_Miercoles: {
			if (_MedTemp.DiasAt[3])
				_MedTemp.DiasAt[3] = false;
			else
				_MedTemp.DiasAt[3] = true;
		}
			break;
		case IDC_Jueves: {
			if (_MedTemp.DiasAt[4])
				_MedTemp.DiasAt[4] = false;
			else
				_MedTemp.DiasAt[4] = true;
		}
			break;
		case IDC_Viernes: {
			if (_MedTemp.DiasAt[5])
				_MedTemp.DiasAt[5] = false;
			else
				_MedTemp.DiasAt[5] = true;
		}
			break;
		case IDC_Sabado: {
			if (_MedTemp.DiasAt[6])
				_MedTemp.DiasAt[6] = false;
			else
				_MedTemp.DiasAt[6] = true;
		}
			break;
		}
		break;
	case WM_INITDIALOG: {
		if (strcmp(usuario.setClave(),"")==0) {
			_HLogin = CreateDialog(0, MAKEINTRESOURCE(IDD_Login), hWnd, (DLGPROC)LoginProc);
			EnableWindow(hWnd, false);
			ShowWindow(_HLogin, SW_SHOW);
		}
			AgregarMenu(hWnd);
			SetWindowTextA(GetDlgItem(hWnd, IDC_NombreUs), usuario.setNombre());
			if (!_ListEsp->Getfirst() == NULL) {
				Especialidad* NodEsp;
				NodEsp = _ListEsp->Getfirst();
				do {
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_ADDSTRING, 0, (LPARAM)NodEsp->Nombre);
					NodEsp = NodEsp->Sig;
				} while (NodEsp != NULL);
			}
			else {
				EnableWindow(GetDlgItem(hWnd, IDC_COMBOEsp), false);
			}
			char texto[10] = { '\0' };
			for (int i = 1; i < 6; i++) {
				StringCchPrintfA(texto, 5, " %d", i);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_ADDSTRING, 0, (LPARAM)texto);
			}
			for (int i = 8; i < 20; i++) {
				StringCchPrintfA(texto, 7, " %d:00", i);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_ADDSTRING, 0, (LPARAM)texto);
			}
			for (int i = 9; i < 21; i++) {
				StringCchPrintfA(texto, 7, " %d:00", i);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_ADDSTRING, 0, (LPARAM)texto);
			}
			char aux[5] = { '\0' };
			AgregarMedicoBMP(aux, hWnd, IDC_MedicoFoto, Current);
	}
	break;
	case WM_CLOSE: {
		int a = MessageBox(0, L"Se cerrara el programa, ¿quiere continuar de todas formas?",
			L"Advertencia", MB_YESNO | MB_ICONEXCLAMATION);
		if (a == IDYES) {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EspecialidadProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case Menu_VentanaMedico: {
			_HMedico = CreateDialog(0, MAKEINTRESOURCE(IDD_WMedicos), hWnd, (DLGPROC)MedicoProc);
			ShowWindow(_HMedico, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaMostrarCitas: {
			_HMostrarCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WMostrarCitas), hWnd, (DLGPROC)MostrarCitasProc);
			ShowWindow(_HMostrarCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaPaciente: {
			_HPaciente = CreateDialog(0, MAKEINTRESOURCE(IDD_WPaciente), hWnd, (DLGPROC)PacienteProc);
			ShowWindow(_HPaciente, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaCitas: {
			_HCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WCitas), hWnd, (DLGPROC)CitasProc);
			ShowWindow(_HCitas, SW_SHOW);
			EndDialog(hWnd, 0);
			}
			break;
		case Menu_RegistroMedico: {
			_TreeMed->quickSort(_TreeMed->GetRoot(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\EstructuraDatos\RegistroMedicos.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de médicos.", MB_OK);
		}
			break;
		case Menu_RegistroPaciente: {
			_PacTemp.FillArray(_ListPac->Getfirst(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroPacientes.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
		}
			break;
		case IDC_AgregarEspecial: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_IDEspecial), _EspTemp.clave, 10);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NomEspecial), _EspTemp.Nombre, 20);
			GetWindowTextA(GetDlgItem(hWnd, IDC_DesEspecial), _EspTemp.DescripcionEsp, 50);
			if (validarDatosE(_EspTemp, hWnd, -1)) {
				_ListEsp->AddNodo(_EspTemp);
				_EspTemp.MostrarDatosE(_ListEsp->Getlast(), hWnd, true, 0);
				MessageBoxA(hWnd, "Especialidad agregada", "Especialidad agregada correctamente", MB_OK);
				CleanEspWindowE(hWnd);
				EnableWindow(GetDlgItem(hWnd, IDC_ToEspecial), true);
				RtlZeroMemory(&_EspTemp, sizeof(_EspTemp));
				int a = SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_GETCOUNT, 0, 0);
				if (a == 2) {
					char Checar[42];
					SendMessageA(GetDlgItem(hWnd, IDC_ToEspecial), LB_GETTEXT, 0, (LPARAM)Checar);
					if(strcmp(Checar, "No se ha registrado ninguna especialidad.")==0)
					SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_DELETESTRING, 0, 0);
				}
			}
			else {
				MessageBoxA(hWnd, "Especialidad no ha sido agregada correctamente", "Especialidad no agregada", MB_OK | MB_ICONERROR);
			}
		}
			break;
		case IDC_BorrarEspecial: {
			if (pos != -1) {
				int longi = 0;
				if (_EspTemp.ChecarBorrar(_ListEsp->SearchNodo(pos), _TreeMed->GetRootS())) {
					_ListEsp->DeleteNodo(_ListEsp->SearchNodo(pos));
					SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_DELETESTRING, pos, 0);
					longi = SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_GETCOUNT, 0, 0);
					if (longi == 0) {
						SendMessageA(GetDlgItem(hWnd, IDC_ToEspecial), LB_ADDSTRING, 0, (LPARAM)"No se ha registrado ninguna especialidad.");
						EnableWindow(GetDlgItem(hWnd, IDC_ToEspecial), false);
						EnableWindow(GetDlgItem(hWnd, IDC_MostrarEsp), false);
						EnableWindow(GetDlgItem(hWnd, IDC_BorrarEspecial), false);
					}
				}
				else {
					MessageBoxA(hWnd, "Lo sentimos, hay un médico en esa especialidad", "Error", MB_ICONERROR);
				}
				CleanEspWindowE(hWnd);
				ShowWindow(GetDlgItem(hWnd, IDC_ModEspecial), false);
				ShowWindow(GetDlgItem(hWnd, IDC_MostrarEsp), true);
				pos = -1;
			}
			else MessageBoxA(hWnd, "No se ha seleccionado ninguna especialidad", "Error", MB_ICONERROR);
		}
			break;
		case IDC_MostrarEsp: {
			if (pos != -1) {
				ShowWindow(GetDlgItem(hWnd, IDC_ModEspecial), true);
				pos = SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_GETCURSEL, 0, 0);
				_EspTemp.MostrarDatosEdit(_ListEsp->SearchNodo(pos), hWnd);
				ShowWindow(GetDlgItem(hWnd, IDC_MostrarEsp), false);
			}
			else MessageBoxA(hWnd, "No se ha seleccionado ninguna especialidad", "Error", MB_ICONERROR);
		}
			break;
		case IDC_ModEspecial: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_IDEspecial), _EspTemp.clave, 10);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NomEspecial), _EspTemp.Nombre, 20);
			GetWindowTextA(GetDlgItem(hWnd, IDC_DesEspecial), _EspTemp.DescripcionEsp, 50);
			if (validarDatosE(_EspTemp, hWnd, pos)) {
				_ListEsp->ModifyNodo(_ListEsp->SearchNodo(pos), _EspTemp);
				SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_DELETESTRING, pos, 0);
				_EspTemp.MostrarDatosE(_ListEsp->SearchNodo(pos), hWnd, false, pos);
				ShowWindow(GetDlgItem(hWnd, IDC_MostrarEsp), true);
				ShowWindow(GetDlgItem(hWnd, IDC_ModEspecial), false);
				MessageBoxA(hWnd, "Cita editada con exito", "Cita editada", MB_OK);
				CleanEspWindowE(hWnd);
				ShowWindow(GetDlgItem(hWnd, IDC_ModEspecial), false);
				ShowWindow(GetDlgItem(hWnd, IDC_MostrarEsp), true);
			}
			else {
				MessageBoxA(hWnd, "NO", "NO", MB_OK);
			}		
		}
			break;
		case IDC_ToEspecial: {
			pos = SendMessage(GetDlgItem(hWnd, IDC_ToEspecial), LB_GETCURSEL, 0, 0);
			if (pos != -1) {
				EnableWindow(GetDlgItem(hWnd, IDC_BorrarEspecial), true);
				EnableWindow(GetDlgItem(hWnd, IDC_MostrarEsp), true);
				EnableWindow(GetDlgItem(hWnd, IDC_AgregarEspecial), true);
			}
		}
			break;
		}
		break;
	case WM_INITDIALOG: {
		AgregarMenu(hWnd);
		SetWindowTextA(GetDlgItem(hWnd, IDC_NombreUs), usuario.setNombre());
		if (!_ListEsp->Getfirst() == NULL) {
			Especialidad* NodEsp;
			NodEsp = _ListEsp->Getfirst();
			do {
				NodEsp->MostrarDatosE(NodEsp, hWnd, true, 0);
				NodEsp = NodEsp->Sig;
			} while (NodEsp != NULL);
			delete NodEsp;
		}
		else {
			SendMessageA(GetDlgItem(hWnd, IDC_ToEspecial), LB_ADDSTRING, 0, (LPARAM)"No se ha registrado ninguna especialidad.");
			EnableWindow(GetDlgItem(hWnd, IDC_ToEspecial), false);
		}
	}
	break;
	case WM_CLOSE:{
		int a = MessageBox(0, L"Se cerrara el programa, ¿quiere continuar de todas formas?",
			L"Advertencia", MB_YESNO | MB_ICONEXCLAMATION);
		if (a == IDYES) {
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK PacienteProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case Menu_VentanaMedico: {
			_HMedico = CreateDialog(0, MAKEINTRESOURCE(IDD_WMedicos), hWnd, (DLGPROC)MedicoProc);
			ShowWindow(_HMedico, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaMostrarCitas: {
			_HMostrarCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WMostrarCitas), hWnd, (DLGPROC)MostrarCitasProc);
			ShowWindow(_HMostrarCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VEsp: {
			_HEspecialidad = CreateDialog(0, MAKEINTRESOURCE(IDD_WEspecial), hWnd, (DLGPROC)EspecialidadProc);
			ShowWindow(_HEspecialidad, SW_SHOW);
			pos = -1;
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaCitas: {
			_HCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WCitas), hWnd, (DLGPROC)CitasProc);
			ShowWindow(_HCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_RegistroMedico: {
			_TreeMed->quickSort(_TreeMed->GetRoot(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\EstructuraDatos\RegistroMedicos.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de médicos.", MB_OK);
		}
			break;
		case Menu_RegistroPaciente: {
			_PacTemp.FillArray(_ListPac->Getfirst(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroPacientes.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
		}
			break;
		case IDC_Masculino: {
			CheckDlgButton(hWnd, IDC_Femenino, BST_UNCHECKED);
			_PacTemp.Genero = 1;
		}
			break;
		case IDC_Femenino: {
			CheckDlgButton(hWnd, IDC_Masculino, BST_UNCHECKED);
			_PacTemp.Genero = 2;
		}
			break;
		case IDC_SelectFecha: {
			GetSelectedDate(GetDlgItem(hWnd, IDC_FechaNacimiento), (_PacTemp.fechaNa));
			if (strcmp(_PacTemp.fechaNa, "000") != 0) {
				SetWindowTextA(GetDlgItem(hWnd, IDC_EdadPaciente), _PacTemp.CalcularEdad(_PacTemp.fechaNa) );
			}
			else
				MessageBoxA(hWnd, "Fecha invalida", "Esta fecha no pude ser seleccionada", MB_ICONERROR);
		}
			break;
		case IDC_BBuscarPa: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_BuscarPa), _PacTemp.Apellidos, 30);
			GetWindowTextA(GetDlgItem(hWnd, IDC_BuscarPaN), _PacTemp.Nombre, 30);
			auxPac = _PacTemp.SerchPaciente(_PacTemp, _ListPac->Getfirst());
			if (auxPac != 0) {
				ShowWindow(GetDlgItem(hWnd, IDC_EditPac), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_EliminarPa), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarPacien), SW_HIDE);
				SetWindowTextA(GetDlgItem(hWnd, IDC_NombrePaciente), auxPac->Nombre);
				SetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosPacien), auxPac->Apellidos);
				SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPaciente), auxPac->clave);
				SetWindowTextA(GetDlgItem(hWnd, IDC_PrimerMedico), auxPac->MedPas);
				SetWindowTextA(GetDlgItem(hWnd, IDC_ReferenciaPaciente), auxPac->Referencia);
				SetWindowTextA(GetDlgItem(hWnd, IDC_EdadPaciente), auxPac->Edad);
				if (auxPac->Genero == 2) 
					CheckDlgButton(hWnd, IDC_Femenino, BST_CHECKED);
				else
					CheckDlgButton(hWnd, IDC_Masculino, BST_CHECKED);
			}
			else
			MessageBoxA(hWnd, "Los apellidos del paciente no \ncoinciden con ninguno registrado", "No se encontro el paciente", MB_ICONERROR);
		}
			break;
		case IDC_EditPac: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_NombrePaciente), _PacTemp.Nombre, 30);
			GetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosPacien), _PacTemp.Apellidos, 30);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPaciente), _PacTemp.clave, 12);
			GetWindowTextA(GetDlgItem(hWnd, IDC_PrimerMedico), _PacTemp.MedPas, 50);
			GetWindowTextA(GetDlgItem(hWnd, IDC_ReferenciaPaciente), _PacTemp.Referencia, 70);
			GetWindowTextA(GetDlgItem(hWnd, IDC_EdadPaciente), _PacTemp.Edad, 11);
			if (validarDatosP(&_PacTemp, hWnd)) {
				_ListPac->ModifyNodo(_PacTemp.SerchPaciente(_PacTemp, _ListPac->Getfirst()), _PacTemp);
				RtlZeroMemory(&_PacTemp, sizeof(_PacTemp));
				RtlZeroMemory(&auxPac, sizeof(auxPac));
				MessageBoxA(hWnd, "Paciente editado", "Paciente editado correctamente", MB_OK);
				CleanPacWindowP(hWnd);
				ShowWindow(GetDlgItem(hWnd, IDC_EditPac), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_EliminarPa), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarPacien), SW_SHOW);
			}
			else {
				MessageBoxA(hWnd, "Paciente no sido ha sido editado correctamente", "Paciente no editado", MB_OK | MB_ICONERROR);
			}
		}
			break;
		case IDC_EliminarPa: {
			if (_PacTemp.searchCita(_PacTemp, _ListCitas->Getfirst())) {
				_ListPac->DeleteNodo(_PacTemp.SerchPaciente(_PacTemp, _ListPac->Getfirst()));
				MessageBoxA(hWnd, "El paciente fue eliminado.", "Paciente eliminado", MB_OK);
				CleanPacWindowP(hWnd);
				ShowWindow(GetDlgItem(hWnd, IDC_EditPac), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_EliminarPa), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarPacien), SW_SHOW);
				RtlZeroMemory(&_PacTemp, sizeof(_PacTemp));
				RtlZeroMemory(&auxPac, sizeof(auxPac));
			}
			else
				MessageBoxA(hWnd, "El paciente no puede ser eliminado \ntiene una cita pendiente.", "El paciente no ha sido eliminado", MB_ICONERROR);
		}
			break;
		case IDC_GuardarPacien: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_NombrePaciente), _PacTemp.Nombre, 30);
			GetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosPacien), _PacTemp.Apellidos, 30);
			GetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPaciente), _PacTemp.clave, 12);
			GetWindowTextA(GetDlgItem(hWnd, IDC_PrimerMedico), _PacTemp.MedPas, 50);
			GetWindowTextA(GetDlgItem(hWnd, IDC_ReferenciaPaciente), _PacTemp.Referencia, 70);
			GetWindowTextA(GetDlgItem(hWnd, IDC_EdadPaciente), _PacTemp.Edad, 11);
			if (validarDatosP(&_PacTemp, hWnd)) {
				_ListPac->AddNodo(_PacTemp);
				RtlZeroMemory(&_PacTemp, sizeof(_PacTemp));
				MessageBoxA(hWnd, "Paciente agregado", "Paciente agregado correctamente", MB_OK);
				CleanPacWindowP(hWnd);
				}
			else {
				MessageBoxA(hWnd, "Paciente no sido ha agregado correctamente", "Paciente no agregado", MB_OK | MB_ICONERROR);
			}
		}
			break;
		}
		break;
	case WM_INITDIALOG: {
		SetWindowTextA(GetDlgItem(hWnd, IDC_NombreUs), usuario.setNombre());
		AgregarMenu(hWnd);
	}
		break;
	case WM_CLOSE:{
		int a = MessageBox(0, L"Se cerrara el programa, ¿quiere continuar de todas formas?",
			L"Advertencia", MB_YESNO | MB_ICONEXCLAMATION);
		if (a == IDYES) {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK CitasProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case Menu_VentanaMedico: {
			_HMedico = CreateDialog(0, MAKEINTRESOURCE(IDD_WMedicos), hWnd, (DLGPROC)MedicoProc);
			ShowWindow(_HMedico, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaMostrarCitas: {
			_HMostrarCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WMostrarCitas), hWnd, (DLGPROC)MostrarCitasProc);
			ShowWindow(_HMostrarCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VEsp: {
			_HEspecialidad = CreateDialog(0, MAKEINTRESOURCE(IDD_WEspecial), hWnd, (DLGPROC)EspecialidadProc);
			ShowWindow(_HEspecialidad, SW_SHOW);
			pos = -1;
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaPaciente: {
			_HPaciente = CreateDialog(0, MAKEINTRESOURCE(IDD_WPaciente), hWnd, (DLGPROC)PacienteProc);
			ShowWindow(_HPaciente, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_RegistroMedico: {
			_TreeMed->quickSort(_TreeMed->GetRoot(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\EstructuraDatos\RegistroMedicos.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de médicos.", MB_OK);
		}
			break;
		case Menu_RegistroPaciente: {
			_PacTemp.FillArray(_ListPac->Getfirst(),Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroPacientes.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
		}
			break;
		case IDC_BuscarCita: {
			GetWindowTextA(GetDlgItem(hWnd, IDC_ClaveCita), _CitaTemp.ClaveCita, 20);
			auxCitas = _CitaTemp.SearchNodo(_CitaTemp.ClaveCita, _ListCitas->Getfirst());
			if (auxCitas != 0) {
				ShowWindow(GetDlgItem(hWnd, IDC_GuardarCita), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDC_EditCita), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_CancelarCita), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_CancelarStatic), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_CancelarText), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_RADIOPendiente), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_RADIOAtendida), SW_SHOW);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBPaciente), false);
				EnableWindow(GetDlgItem(hWnd, IDC_SelectPaciente), false);
				_CitaTemp.CopyNode(&_CitaTemp, auxCitas);
				strcpy_s(_MedTemp.clave, _CitaTemp.clave);
				auxMed = _TreeMed->SearchTreeNode(_TreeMed->GetRootS(), _MedTemp);
				char Medico[60] = { '\0' };
				StringCchPrintfA(Medico, 60, " %s: %s", auxMed->clave, auxMed->Nombre);
				int a = SendMessage(GetDlgItem(hWnd, IDC_COMBMedico),CB_GETCOUNT, 0, 0);
				for (short i = 0; i < a; i++) {
					char Med[60] = { '\0' };
					SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETLBTEXT, i, (LPARAM)Med);
					if (strcmp(Medico, Med) == 0) {
						a = i;
						break;
					}
				}
				SendMessage(GetDlgItem(hWnd, IDC_COMBMedico), CB_SETCURSEL, a, 0);
				StringCchPrintfA(Medico, 34, " %d", auxMed->NumConsultorio);
				SetWindowTextA(GetDlgItem(hWnd,IDC_Consult), Medico);
				strcpy_s(_PacTemp.Apellidos, _CitaTemp.ApellidoP);
				strcpy_s(_PacTemp.Nombre, _CitaTemp.NombreP);
				Paciente* aux = _PacTemp.SerchPaciente(_PacTemp, _ListPac->Getfirst());
				char NombreP[61] = { '\0' };
				StringCbPrintfA(NombreP, 61, "%s %s", aux->Apellidos, aux->Nombre);
				int b = SendMessage(GetDlgItem(hWnd, IDC_COMBPaciente), CB_GETCOUNT, 0, 0);
				for (short i = 0; i < b; i++) {
					char Pacientex[60] = { '\0' };
					SendMessageA(GetDlgItem(hWnd, IDC_COMBPaciente), CB_GETLBTEXT, i, (LPARAM)Pacientex);
					if (strcmp(NombreP, Pacientex) == 0) {
						b = i;
						break;
					}
				}
				SendMessageA(GetDlgItem(hWnd, IDC_COMBPaciente), CB_SETCURSEL, b, 0);
				SetWindowTextA(GetDlgItem(hWnd, IDC_TelefonoP), aux->clave);
				SetWindowTextA(GetDlgItem(hWnd, IDC_TelefonoP), _CitaTemp.ClaveUsuario);
				if (strcmp(_CitaTemp.Cancelar, "") != 0) {
					SetWindowTextA(GetDlgItem(hWnd, IDC_CancelarText), _CitaTemp.Cancelar);
				}
				if (_CitaTemp.getEstadoCita() == 1) {
					CheckDlgButton(hWnd, IDC_RADIOPendiente, BST_CHECKED);
				}
				else if (_CitaTemp.getEstadoCita() == 2) {
					CheckDlgButton(hWnd, IDC_RADIOAtendida, BST_CHECKED);
				}
				else
					CheckDlgButton(hWnd, IDC_CancelarCita, BST_CHECKED);
				strcpy_s(auxFoto2, "\\");
				strcat_s(auxFoto2, auxMed->FotoMedico);
				AgregarMedicoBMP(auxFoto2, hWnd, IDC_FotoMedicoCi, Current);
				ZeroMemory(&auxFoto2, sizeof(&auxFoto2));
				aux = 0;
				delete aux;
			}
			else
				MessageBoxA(hWnd, "La cita no ha sido encontrada.", "Cita no encontrada", MB_ICONWARNING);
		}
			break;
		case IDC_GuardarCita: {
			SYSTEMTIME fecha;
			ZeroMemory(&fecha, sizeof(SYSTEMTIME));
			GetLocalTime(&fecha);
			if (fecha.wDayOfWeek == 6 || fecha.wDayOfWeek == 0) {
				MessageBoxA(hWnd, "Lo sentimos los días sabados y domingos no se pueden agregar citas.",
					"No disponible", MB_ICONWARNING);
			}
			else {
				if (validarDatosC(_CitaTemp, _ListCitas->Getfirst(), hWnd)) {
					char MNCita[85] = { '\0' };
					if (ChecarSelectedDate(_CitaTemp)) {
						StringCbPrintfA(_CitaTemp.ClaveCita, 20, "%d%c%c%s%s", auxMed->NumConsultorio, _CitaTemp.NombreP[0], _CitaTemp.ApellidoP[0], _CitaTemp.HoraC, _CitaTemp.FechaC);
						StringCbPrintfA(MNCita, 85, "La cita ha sido agregada correctamente.\nLa clave de la citas es:%s", _CitaTemp.ClaveCita);
						strcpy_s(_CitaTemp.ClaveUsuario, usuario.setClave());
						_CitaTemp.setEstadoCita(1);
						_ListCitas->AddNodo(_CitaTemp);
						MessageBoxA(hWnd, MNCita, "Cita agregada", MB_OK);
						CleanCitWindowC(hWnd);
						ZeroMemory(&auxMed, sizeof(&auxMed));
						_CitaTemp.CleanNode(&_CitaTemp);
						ZeroMemory(&_CitaTemp, sizeof(&_CitaTemp));
						ZeroMemory(&auxFoto2, sizeof(&auxFoto2));
					}
					else {
						MessageBoxA(hWnd, "La hora de la cita no es valida.", "Error en la hora", MB_ICONERROR);
					}
				}
				else
					MessageBoxA(hWnd, "Los datos de la cita no son correctos", "Error en datos", MB_ICONERROR);
			}
		}
			break;
		case IDC_EditCita: {
			if (_CitaTemp.getEstadoCita() == 3) {
				GetWindowTextA(GetDlgItem(hWnd, IDC_CancelarText), _CitaTemp.Cancelar, 30);
				if (strcmp(_CitaTemp.Cancelar, "") != 0) {
					if (validarDatosC(_CitaTemp, _ListCitas->Getfirst(), hWnd)) {
						_ListCitas->ModifyNodo(_CitaTemp.SearchNodo(_CitaTemp.ClaveCita, _ListCitas->Getfirst()), _CitaTemp);
						ShowWindow(GetDlgItem(hWnd, IDC_GuardarCita), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_EditCita), SW_HIDE);
						ShowWindow(GetDlgItem(hWnd, IDC_CancelarStatic), SW_HIDE);
						ShowWindow(GetDlgItem(hWnd, IDC_CancelarText), SW_HIDE);
						EnableWindow(GetDlgItem(hWnd, IDC_COMBPaciente), true);
						EnableWindow(GetDlgItem(hWnd, IDC_SelectPaciente), true);
						CleanCitWindowC(hWnd);
						EnableWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), false);
						EnableWindow(GetDlgItem(hWnd, IDC_SelectFecha), false);
						EnableWindow(GetDlgItem(hWnd, IDC_COMBOHoraCita), false);
						CheckDlgButton(hWnd, IDC_RADIOPendiente, BST_UNCHECKED);
						CheckDlgButton(hWnd, IDC_RADIOAtendida, BST_UNCHECKED);
						CheckDlgButton(hWnd, IDC_CancelarCita, BST_UNCHECKED);
						ZeroMemory(&auxMed, sizeof(&auxMed));
						ZeroMemory(&_CitaTemp, sizeof(&_CitaTemp));
					}
				}
				else
					MessageBoxA(hWnd, "Es necesario un motivo para cancelar cita.", "No se puede cancelar", MB_ICONERROR);
			}
			else {
				if (validarDatosC(_CitaTemp, _ListCitas->Getfirst(), hWnd)) {
					_ListCitas->ModifyNodo(_CitaTemp.SearchNodo(_CitaTemp.ClaveCita, _ListCitas->Getfirst()), _CitaTemp);
					ShowWindow(GetDlgItem(hWnd, IDC_GuardarCita), SW_SHOW);
					ShowWindow(GetDlgItem(hWnd, IDC_EditCita), SW_HIDE);
					ShowWindow(GetDlgItem(hWnd, IDC_CancelarStatic), SW_HIDE);
					ShowWindow(GetDlgItem(hWnd, IDC_CancelarText), SW_HIDE);
					EnableWindow(GetDlgItem(hWnd, IDC_COMBPaciente), true);
					EnableWindow(GetDlgItem(hWnd, IDC_SelectPaciente), true);
					CleanCitWindowC(hWnd);
					EnableWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), false);
					EnableWindow(GetDlgItem(hWnd, IDC_SelectFecha), false);
					EnableWindow(GetDlgItem(hWnd, IDC_COMBOHoraCita), false);
					ZeroMemory(&auxMed, sizeof(&auxMed));
					ZeroMemory(&_CitaTemp, sizeof(&_CitaTemp));
				}
			}
		}
			break;
		case IDC_RADIOPendiente: {
			_CitaTemp.setEstadoCita(1);
		}
			break;
		case IDC_RADIOAtendida: {
			_CitaTemp.setEstadoCita(2);
		}
			break;
		case IDC_CancelarCita: {
			_CitaTemp.setEstadoCita(3);
		}
			break;
		case IDC_COMBOHoraCita: {
			int a = SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_GETLBTEXT, a, (LPARAM)_CitaTemp.HoraC);
		}
			break;
		case IDC_Select: {
			int b = SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_GETLBTEXT, b, (LPARAM)Esp);
			int i = 0;
			if (strcmp(Esp, "") != 0 && strcmp(Esp, "Todas") != 0) {
				i = SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETCOUNT, 0, 0);
				for (short j = 0; j < i; j++) {
					SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_DELETESTRING, 0, 0);
				}
				_MedTemp.SetMedicos(_TreeMed->GetRootS(), Esp, false, hWnd, IDC_COMBMedico);
				strcpy_s(Esp, "");
			}
			if (strcmp(Esp, "Todas") == 0) {
				i = SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETCOUNT, 0, 0);
				for (short j = 0; j < i; j++) {
					SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_DELETESTRING, 0, 0);
				}
				_MedTemp.SetMedicos(_TreeMed->GetRootS(), Esp, true, hWnd, IDC_COMBMedico);
			}
			int a = SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETCOUNT, 0, 0);
			if (a == 0) 
				EnableWindow(GetDlgItem(hWnd, IDC_COMBMedico), false);
			else
				EnableWindow(GetDlgItem(hWnd, IDC_COMBMedico), true);
		}
			break;
		case IDC_SelMedico: {
			char DatMedico[80] = { '\0' };
			int a = SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETLBTEXT, a, (LPARAM)DatMedico);
			if (strcmp(DatMedico, "") != 0) {
				for (short j = 11; j < 70; j++) {
					DatMedico[j] = '\0';
				}
				for (short i = 0; i < 11; i++) {
					_MedTemp.clave[i] = DatMedico[i+1];
				}
				auxMed = _TreeMed->SearchTreeNode(_TreeMed->GetRootS(), _MedTemp);
				strcpy_s(_CitaTemp.clave, auxMed->clave);
				char HoED[3] = { '\0' }, HoSD[3] = { '\0' };
				for (short i = 0; i < 3; i++) {
					if (isdigit(auxMed->HorarioAt[0][i + 1]))
						HoED[i] = auxMed->HorarioAt[0][i + 1];
					if (isdigit(auxMed->HorarioAt[1][i + 1]))
						HoSD[i] = auxMed->HorarioAt[1][i + 1];
				}
				int HorED, HorSD;
				HorED = atoi(HoED);
				HorSD = atoi(HoSD);
				for (HorED; HorED < HorSD; HorED++) {
					StringCbPrintfA(DatMedico, 10, "%d:00", HorED);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_ADDSTRING, a, (LPARAM)DatMedico);
					StringCbPrintfA(DatMedico, 10, "%d:20", HorED);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_ADDSTRING, a, (LPARAM)DatMedico);
					StringCbPrintfA(DatMedico, 10, "%d:40", HorED);
					SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_ADDSTRING, a, (LPARAM)DatMedico);
				}
				strcpy_s(_CitaTemp.Especialidad, auxMed->Especialidad);
				StringCbPrintfA(HoSD,5, "%d", auxMed->NumConsultorio);
				SetWindowTextA(GetDlgItem(hWnd, IDC_Consult), HoSD);
				EnableWindow(GetDlgItem(hWnd, IDC_SelectFecha), true);
				EnableWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), true);
				strcpy_s(auxFoto2, "\\");
				strcat_s(auxFoto2, auxMed->FotoMedico);
				AgregarMedicoBMP(auxFoto2, hWnd, IDC_FotoMedicoCi, Current);
			}
		}
			break;
		case IDC_SelectFecha: {
			short a = GetSelectedDateCita(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), auxMed, &_CitaTemp);
			if (a == 1) {
				EnableWindow(GetDlgItem(hWnd, IDC_COMBOHoraCita), true);
			}
			else if (a == 2) {
				MessageBoxA(hWnd, "El medico no trabaja el día seleccionado", "No disponible", MB_ICONWARNING);
			}
			else
				MessageBoxA(hWnd, "La fecha seleccionada no es valida", "No disponible", MB_ICONWARNING);
		}
			break;
		case IDC_SelectPaciente: {
			pos = SendMessageA(GetDlgItem(hWnd, IDC_COMBPaciente), CB_GETCURSEL, 0, 0);
			auxPac = _ListPac->SearchNodo(pos);
			SetWindowTextA(GetDlgItem(hWnd, IDC_TelefonoP), auxPac->clave);
			strcpy_s(_CitaTemp.ApellidoP, auxPac->Apellidos);
			strcpy_s(_CitaTemp.NombreP, auxPac->Nombre);
		}
			break;
	}
		break;
	case WM_INITDIALOG: {
		AgregarMenu(hWnd);
		SetWindowTextA(GetDlgItem(hWnd, IDC_NombreUs), usuario.setNombre());
		SetWindowTextA(GetDlgItem(hWnd, IDC_ClaveUsuario), usuario.setClave());
		Especialidad* auxE = _ListEsp->Getfirst();
		if (auxE != NULL) {
			SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_ADDSTRING, 0, (LPARAM)"Todas");
			while (auxE != NULL) {
				
				SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_ADDSTRING, 0, (LPARAM)auxE->Nombre);
				auxE = auxE->Sig;
			}
			delete auxE;
		}
		else
			EnableWindow(GetDlgItem(hWnd, IDC_COMBEsp), false);
		Paciente* auxP = _ListPac->Getfirst();
		if (auxP == NULL) {
			EnableWindow(GetDlgItem(hWnd, IDC_COMBPaciente), false);
		}
		else {
			char NombreP[61];
			while (auxP != NULL) {
				StringCbPrintfA(NombreP, 61, "%s %s", auxP->Apellidos, auxP->Nombre);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBPaciente), CB_ADDSTRING, 0, (LPARAM)NombreP);
				ZeroMemory(&NombreP, sizeof(&NombreP));
				auxP = auxP->Sig;
			}
		}
		auxP = 0;
		delete auxP;
		SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_SETCURSEL, 0, 0);
		_MedTemp.SetMedicos(_TreeMed->GetRootS(), _MedTemp.Especialidad, true, hWnd, IDC_COMBMedico);
		int a= SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_GETCOUNT, 0, 0);
		if (a == 0) {
			EnableWindow(GetDlgItem(hWnd, IDC_COMBMedico), false);
		}
	}
		break;
	case WM_CLOSE:{
		int a = MessageBox(0, L"Se cerrara el programa, ¿quiere continuar de todas formas?",
			L"Advertencia", MB_YESNO | MB_ICONEXCLAMATION);
		if (a == IDYES) {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK MostrarCitasProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case Menu_VentanaMedico: {
			_HMedico = CreateDialog(0, MAKEINTRESOURCE(IDD_WMedicos), hWnd, (DLGPROC)MedicoProc);
			ShowWindow(_HMedico, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VEsp: {
			_HEspecialidad = CreateDialog(0, MAKEINTRESOURCE(IDD_WEspecial), hWnd, (DLGPROC)EspecialidadProc);
			ShowWindow(_HEspecialidad, SW_SHOW);
			pos = -1;
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaPaciente: {
			_HPaciente = CreateDialog(0, MAKEINTRESOURCE(IDD_WPaciente), hWnd, (DLGPROC)PacienteProc);
			ShowWindow(_HPaciente, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_VentanaCitas: {
			_HCitas = CreateDialog(0, MAKEINTRESOURCE(IDD_WCitas), hWnd, (DLGPROC)CitasProc);
			ShowWindow(_HCitas, SW_SHOW);
			EndDialog(hWnd, 0);
		}
			break;
		case Menu_RegistroMedico: {
			_TreeMed->quickSort(_TreeMed->GetRoot(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\EstructuraDatos\RegistroMedicos.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de médicos.", MB_OK);
		}
			break;
		case Menu_RegistroPaciente: {
			_PacTemp.FillArray(_ListPac->Getfirst(), Current);
			char direccion[120] = { '\0' };
			StringCchPrintfA(direccion, 120, "La dirección del registro es: %s\\EstructuraDatos\\RegistroPacientes.txt", Current);
			MessageBoxA(hWnd, direccion, "Registro de pacientes.", MB_OK);
		}
			break;
		case IDC_MedMes: {
			pos = 1;
		}
			break;
		case IDC_EspSemana: {
			pos = 2;
		}
			break;
		case IDC_TodasSem: {
			pos = 3;
		}
			break;
		case IDC_SeleccionarF: {
			int a = SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_GETCOUNT, 0, 0);
			if (a != 0) {
				for (short i = 0; i < a; i++) {
					SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_DELETESTRING, i, 0);
				}
			}
			EnableWindow(GetDlgItem(hWnd, IDC_LISTCitas), true);
			if (pos == 1) {
				GetWindowTextA(GetDlgItem(hWnd, IDC_MedicoID), _CitaTemp.clave, 12);
				Tree_Medico* aux = _TreeMed->BinarySearch(_CitaTemp.clave);
				if (aux != 0) {
					GetSelectedDate(IDC_DATETIMEPICKERCitas, hWnd, _CitaTemp.FechaC);
					_CitaTemp.SetCitasMonth(GetDlgItem(hWnd, IDC_LISTCitas), _ListCitas->Getfirst(), _CitaTemp);
				}
				else {
					MessageBoxA(hWnd, "La clave que ingreso no pertenece a ningun médico", "Médico no encontrado", MB_ICONERROR);
				}
				aux = 0;
				delete aux;
			}
			else if (pos == 2) {
				SendMessageA(GetDlgItem(hWnd, IDC_COMBEspecial), CB_GETLBTEXT, a, (LPARAM)_CitaTemp.NombreP);
				if (strcmp(_CitaTemp.NombreP, "") != 0) {
					int dias[2][2] = { '\0' };
					GetSelectedDateW(IDC_DATETIMEPICKERCitas, hWnd, dias);
					_CitaTemp.SetCitasSemanaEsp(dias, GetDlgItem(hWnd, IDC_LISTCitas), _ListCitas->Getfirst(), _CitaTemp.NombreP);
				}
			}
			else if (pos == 3) {
				int dias[2][2] = { '\0' };
				GetSelectedDateW(IDC_DATETIMEPICKERCitas, hWnd, dias);
				_CitaTemp.SetCitasSemana(dias, GetDlgItem(hWnd, IDC_LISTCitas), _ListCitas->Getfirst());
			}
			a = SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_GETCOUNT, 0, 0);
			if (a == 0) {
				SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_INSERTSTRING, 0, (LPARAM)"No hay ninguna cita");
				EnableWindow(GetDlgItem(hWnd, IDC_LISTCitas), false);
			}
		}
			break;
		case IDC_Seleccionar: {
			ShowWindow(GetDlgItem(hWnd, IDC_MedicoID), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERCitas), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_COMBEspecial), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarF), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarRegistro), SW_HIDE);
			SetWindowTextA(GetDlgItem(hWnd, IDC_STATICEleg), "");
			SetWindowTextA(GetDlgItem(hWnd, IDC_STATICCedula), "");
			if (pos == 1) {
				SetWindowTextA(GetDlgItem(hWnd, IDC_STATICEleg), "Ingrese cédula y mes:");
				SetWindowTextA(GetDlgItem(hWnd, IDC_STATICCedula), "Cédula:");
				ShowWindow(GetDlgItem(hWnd, IDC_MedicoID), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERCitas), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarF), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarRegistro), SW_SHOW);
			}
			else if (pos == 2) {
				SetWindowTextA(GetDlgItem(hWnd, IDC_STATICEleg), "Ingrese especialidad y semana:");
				SetWindowTextA(GetDlgItem(hWnd, IDC_STATICCedula), "Especialidad:");
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarF), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarRegistro), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_COMBEspecial), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERCitas), SW_SHOW);
				Especialidad* aux = _ListEsp->Getfirst();
				while (aux != 0) {
					SendMessageA(GetDlgItem(hWnd, IDC_COMBEspecial), CB_ADDSTRING, 0, (LPARAM)aux->Nombre);
					aux = aux->Sig;
				}
				aux = 0;
				delete aux;
			}
			else if (pos == 3) {
				SetWindowTextA(GetDlgItem(hWnd, IDC_STATICEleg), "Ingrese la semana:");
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarF), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_SeleccionarRegistro), SW_SHOW);
				ShowWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERCitas), SW_SHOW);
			}
		}
			break;
		case IDC_SeleccionarRegistro: {
			if (pos == 1) {
				GetWindowTextA(GetDlgItem(hWnd, IDC_MedicoID), _CitaTemp.clave, 12);
				Tree_Medico* aux = _TreeMed->BinarySearch(_CitaTemp.clave);
				if (aux != 0) {
					GetSelectedDate(IDC_DATETIMEPICKERCitas, hWnd, _CitaTemp.FechaC);
					_CitaTemp.SetCitasMonthRegistro(hWnd, _ListCitas->Getfirst(), _CitaTemp, Current, "\\EstructuraDatos\\CitasPorMes.txt");
				}
				else {
					MessageBoxA(hWnd, "La clave que ingreso no pertenece a ningun médico", "Médico no encontrado", MB_ICONERROR);
				}
				aux = 0;
				delete aux;
			}
			else if (pos == 2) {
				int a = SendMessage(GetDlgItem(hWnd, IDC_COMBEspecial), CB_GETCURSEL, 0, 0);
				SendMessageA(GetDlgItem(hWnd, IDC_COMBEspecial), CB_GETLBTEXT, a, (LPARAM)_CitaTemp.NombreP);
				if (strcmp(_CitaTemp.NombreP, "") != 0) {
					int dias[2][2] = { '\0' };
					GetSelectedDateW(IDC_DATETIMEPICKERCitas, hWnd, dias);
					_CitaTemp.SetCitasSemanaRegistroEsp(dias, GetDlgItem(hWnd, IDC_LISTCitas), _ListCitas->Getfirst(), _CitaTemp.NombreP, Current, "\\EstructuraDatos\\CitasSemanaXEspecialidad.txt");
				}
			}
			else if (pos == 3) {
				int a = SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_GETCOUNT, 0, 0);
				if (a != 0) {
					for (short i = 0; i < a; i++) {
						SendMessageA(GetDlgItem(hWnd, IDC_LISTCitas), LB_DELETESTRING, i, 0);
					}
				}
				int dias[2][2] = { '\0' };
				GetSelectedDateW(IDC_DATETIMEPICKERCitas, hWnd, dias);
				_CitaTemp.SetCitasSemanaRegistro(dias, GetDlgItem(hWnd, IDC_LISTCitas), _ListCitas->Getfirst(), Current, "\\EstructuraDatos\\CitasXSemana.txt");
			}
		}
			 break;
		}
		break;
	case WM_INITDIALOG: {
		AgregarMenu(hWnd);
	}
		break;
	case WM_CLOSE:{
		int a = MessageBox(0, L"Se cerrara el programa, ¿quiere continuar de todas formas?",
			L"Advertencia", MB_YESNO | MB_ICONEXCLAMATION);
		if (a == IDYES) {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

template<class T>
bool ChecarNumeros(char* Num, short cant,short posO) {
	T* aux = _ListEsp->Getfirst();
	bool checar = true;
	short Ccant = strlen(Num), i = 0;
	if (Ccant == cant) {
		while (aux != 0) {
			if (strcmp(aux->clave, Num) == 0) {
				if (i != posO) {
					checar = false;
					break;
				}
			}
			aux = aux->Sig;
			i++;
		}
	}
	else {
		checar = false;
	}

	return checar;
}

template<class T>
bool ChecarNombre(char* Nom, short cant, short posO) {
	T* aux = _ListEsp->Getfirst();
	bool checar = true;
	short Ccant = strlen(Nom), i = 0;
	if (Ccant >= cant) {
		for (short i = 0; i < Ccant; i++) {
			if (!isalpha(Nom[i])) {
				if (Nom[i] != ' ') {
					checar = false;
					break;
				}
			}
		}
		if (checar) {
			while (aux != 0) {
				if (strcmp(aux->Nombre, Nom) == 0) {
					if (i != posO) {
						checar = false;
						break;
					}
				}
				aux = aux->Sig;
				i++;
			}
			if (checar) {
				for (short i = 0; i < Ccant; i++) {
					Nom[i] = toupper(Nom[i]);
				}
			}
		}
	}
	else {
		checar = false;
	}

	return checar;
}

bool ChecarDes(char* Des, short cant) {
	bool checar = true;
	short Ccant = strlen(Des), i = 0;
	if (Ccant >= cant)
	Des[0] = toupper(Des[0]);
	else 
		checar = false;

	return checar;
}

bool ChecarNombre(char* Nombre, int cant, int Cesp) {
	int Len = strlen(Nombre);
	short esp = 0;
	if (Len > cant) {
		for (short i = 0; i < Len; i++) {
			if (!isalpha(Nombre[i]) || Nombre[i]=='ñ') {
				if (Nombre[i] == ' ')
					esp++;
				else return false;
			}
		}
		if (esp >= Cesp) {
			for (short i = 1; i < Len; i++) {
				if (Nombre[i - 1] == ' ') {
					Nombre[i] = toupper(Nombre[i]);
				}
				else {
					Nombre[i] = tolower(Nombre[i]);
				}
			}
			Nombre[0] = toupper(Nombre[0]);
			return true;
		}
		else return false;
	}
	else return false;
}

bool ChecarTel(char* telefono, Tree_Medico* tree, Tree_Medico Actual) {
	int cant;
	cant = strlen(telefono);
	if (cant == 8 || cant == 10) {
		return ChecarTelefonos(telefono, tree, Actual);
	}
	else
	return false;
}

bool ChecarTelefonos(char* telefono, Tree_Medico* tree, Tree_Medico actual) {
	if (tree != NULL) {
		if (strcmp(actual.clave, tree->clave) !=0 ) {
			if (!ChecarTelefonos(telefono, tree->RL, actual))
				return false;
			if (strcmp(tree->NumeroCel, telefono) == 0) {
				return false;
			}
			if (!ChecarTelefonos(telefono, tree->RR, actual))
				return false;
		}
		else return true;
	}
	else return true;
}

bool ChecarCed(char* cedula, Tree_Medico* tree) {
	int cant;
	cant = strlen(cedula);
	if (cant == 10) {
		return ChecarCedulas(cedula, tree);
	}
	else
		return false;
}

bool ChecarCedulas(char* cedula, Tree_Medico* tree) {
	if (tree != NULL) {
		if (!ChecarCedulas(cedula, tree->RL))
			return false;
		if (strcmp(tree->clave, cedula) == 0) {
			return false;
		}
		if (!ChecarCedulas(cedula, tree->RR))
			return false;
	}
	else return true;
}

void GetSelectedDate(HWND hMonthCal, char FechaN[20]) {
	SYSTEMTIME fecha, hoy;
	char Year[10], Month[10], Day[10];
	
	ZeroMemory(&fecha, sizeof(SYSTEMTIME));
	SendMessage(hMonthCal, MCM_GETCURSEL, 0, (LPARAM)&fecha);
	GetLocalTime(&hoy);
	if (hoy.wMonth <= fecha.wMonth && hoy.wYear <= fecha.wYear && hoy.wDay < fecha.wDay) {
		StringCbPrintfA((FechaN), 20, "%d%d%d", 0, 0, 0);
	}
	else
	StringCbPrintfA((FechaN), 20, "%d/%d/%d", fecha.wDay, fecha.wMonth, fecha.wYear);	
}

void GetSelectedDate(int hMonth, HWND hWnd, char FechaN[20]) {
	SYSTEMTIME fecha;
	ZeroMemory(&fecha, sizeof(SYSTEMTIME));
	SendMessage(GetDlgItem(hWnd,hMonth), MCM_GETCURSEL, 0, (LPARAM)&fecha);
	StringCbPrintfA((FechaN), 10, "%d", fecha.wMonth);
}

void GetSelectedDateW(int hMonth, HWND hWnd, int dias[2][2]) {
	SYSTEMTIME fecha;
	int dia;
	ZeroMemory(&fecha, sizeof(SYSTEMTIME));
	SendMessage(GetDlgItem(hWnd, hMonth), MCM_GETCURSEL, 0, (LPARAM)&fecha);
	dia = fecha.wDay;
	dias[1][0] = dia - (fecha.wDayOfWeek);
	dias[0][0] = fecha.wMonth;
	if (dias[1][0] < 1) {
		dias[0][0] = fecha.wMonth - 1;
		if (fecha.wMonth == 1 || fecha.wMonth == 3 || fecha.wMonth == 5 || fecha.wMonth == 7 || fecha.wMonth == 8 || fecha.wMonth == 10 || fecha.wMonth == 12) {
			dias[1][0] = 31 + dias[1][0];
		}
		else if (fecha.wMonth == 4 || fecha.wMonth == 6 || fecha.wMonth == 9 || fecha.wMonth == 11) {
			dias[1][0] = 30 + dias[1][0];
		}
		else
			dias[1][0] = 28 + dias[1][0];
	}
	dias[1][1] = dia + (6 - fecha.wDayOfWeek);
	dias[0][1] = fecha.wMonth;
	if (dias[1][1] > 28) {
		if (fecha.wMonth == 1 || fecha.wMonth == 3 || fecha.wMonth == 5 || fecha.wMonth == 7 || fecha.wMonth == 8 || fecha.wMonth == 10 || fecha.wMonth == 12) {
			if (dias[1][1] > 31) {
				dias[0][1] = fecha.wMonth + 1;
				dias[1][1] = dias[1][1] - 31;
			}
		}
		else if (fecha.wMonth == 4 || fecha.wMonth == 6 || fecha.wMonth == 9 || fecha.wMonth == 11) {
			if (dias[1][1] > 30) {
				dias[0][1] = fecha.wMonth + 1;
				dias[1][1] = dias[1][1] - 30;
			}
		}
		else
			if (dias[1][1] > 28) {
				dias[0][1] = fecha.wMonth + 1;
				dias[1][1] = dias[1][1] - 28;
			}
	}
}

short GetSelectedDateCita(HWND hMonthCal, Tree_Medico* Doc, Citas* FCita) {
	SYSTEMTIME fecha,hoy;

	ZeroMemory(&fecha, sizeof(SYSTEMTIME));
	ZeroMemory(&hoy, sizeof(SYSTEMTIME));
	SendMessage(hMonthCal, MCM_GETCURSEL, 0, (LPARAM)&fecha);
	GetLocalTime(&hoy);
	if (fecha.wMonth >= hoy.wMonth && fecha.wYear >= hoy.wYear) {
		if (fecha.wMonth == hoy.wMonth) {
			if (fecha.wDay >= hoy.wDay){
				if (Doc->DiasAt[fecha.wDayOfWeek] == true) {
					StringCbPrintfA((FCita->FechaC), 10, "%d/%d/%d", fecha.wDay, fecha.wMonth, fecha.wYear);
					return 1;
				}
				else return 2;
			}
		}
		else {
			if (Doc->DiasAt[fecha.wDayOfWeek] == true) {
				StringCbPrintfA((FCita->FechaC), 10, "%d/%d/%d", fecha.wDay, fecha.wMonth, fecha.wYear);
				return 1;
			}
			else return 2;
		}
	}
	else return 3;	
}

bool ChecarSelectedDate(Citas CitaFH) {
	SYSTEMTIME hoy, fechaC;
	char Dato[5] = { '\0' };
	int dato, parentesis, j;
	ZeroMemory(&hoy, sizeof(&hoy));
	ZeroMemory(&fechaC, sizeof(&fechaC));
	GetLocalTime(&hoy);
	j = 0;
	parentesis = 0;
	for (short i = 0; i < strlen(CitaFH.FechaC); i++) {
		if (CitaFH.FechaC[i] == '/')
			parentesis++;
		if (parentesis == 0) {
			if (isdigit(CitaFH.FechaC[i])) {
				Dato[j] = CitaFH.FechaC[i];
				j++;
			}
		}
		if (parentesis == 1)
			break;
	}
	dato = atoi(Dato);
	fechaC.wDay = dato;
	Dato[1] = '\0';
	parentesis = 0;
	j = 0;
	for (short i = 0; i < strlen(CitaFH.FechaC); i++) {
		if (CitaFH.FechaC[i] == '/')
			parentesis++;
		if (parentesis == 1) {
			if (isdigit(CitaFH.FechaC[i])) {
				Dato[j] = CitaFH.FechaC[i];
				j++;
			}
		}
		if (parentesis == 2)
			break;
	}
	dato = atoi(Dato);
	fechaC.wMonth = dato;
	Dato[1] = '\0';
	parentesis = 0;
	j = 0;
	for (short i = 0; i < strlen(CitaFH.FechaC); i++) {
		if (CitaFH.FechaC[i] == '/')
			parentesis++;
		if (parentesis == 2) {
			if (isdigit(CitaFH.FechaC[i])) {
				Dato[j] = CitaFH.FechaC[i];
				j++;
			}
		}
		if (j == 4) {
			break;
		}
	}
	dato = atoi(Dato);
	fechaC.wYear = dato;
	j = 0;
	Dato[1] = '\0';
	for (short i = 0; i < strlen(CitaFH.HoraC); i++) {
		if (isdigit(CitaFH.HoraC[i])) {
			Dato[j] = CitaFH.HoraC[i];
			j++;
		}
		if (CitaFH.HoraC[i] == ':')
			break;
	}
	dato = atoi(Dato);
	fechaC.wHour = dato;
	parentesis = 0;
	Dato[1] = '\0';
	j = 0;
	for (short i = 0; i < strlen(CitaFH.HoraC); i++) {
		if (parentesis == 1) {
			if (isdigit(CitaFH.HoraC[i])) {
				Dato[j] = CitaFH.HoraC[i];
				j++;
			}
		}
		if (CitaFH.HoraC[i] == ':')
			parentesis++;
	}
	dato = atoi(Dato);
	fechaC.wMinute = dato;
	Dato[4] = '\0';
	dato = 0;
	if (fechaC.wYear >= hoy.wYear && fechaC.wMonth >= hoy.wMonth) {
		if (fechaC.wMonth == hoy.wMonth) {
			if (fechaC.wDay >= hoy.wDay) {
				return true;
			}
			else if (fechaC.wDay == hoy.wDay) {
				if (fechaC.wHour > hoy.wHour) {
					return true;
				}
				else return false;
			}
		}
		else {
			return true;
		}
	}
	return false;
}

void AgregarMedicoBMP(char foto[100], HWND hWnd, int IDFoto, char* current) {
	HBITMAP imagen;
	char dirfoto[120] = { '\0' };
	strcpy_s(dirfoto, current);
	if(strcmp(foto,"")!=0)
	strcat(dirfoto, foto);
	ZeroMemory(&imagen, sizeof(imagen));
	imagen = (HBITMAP)LoadImageA(NULL, dirfoto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
	SendMessageA(GetDlgItem(hWnd, IDFoto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
}

void ArchivosSelectorSave(HWND hWnd, char foto[]) {
	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"0BMP\0*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if (GetSaveFileName(&ofn) == TRUE) {
		CvtUniAnsi(ofn.lpstrFile, (foto));
	}
}

void CvtUniAnsi(wchar_t* cad, char foto[]) {
	int size = wcslen(cad) + 1;
	for (int i = 0; i < size; i++) {
		(foto[i]) = cad[i];
	}
}

void AgregarMenu(HWND hWnd) {
	HMENU Medic, Pacient, Menu,Cit;
	Cit = CreateMenu();
	Medic = CreateMenu();
	Pacient = CreateMenu();
	Menu = CreateMenu();
	AppendMenuA(Medic, MF_POPUP, Menu_VentanaMedico, "M&edicos");
	AppendMenuA(Medic, MF_POPUP, Menu_RegistroMedico, "Reporte Me&dicos");
	AppendMenuA(Menu, MF_POPUP, (UINT_PTR)Medic, "&Medico");
	AppendMenuA(Pacient, MF_POPUP, Menu_VentanaPaciente, "&Pacientes");
	AppendMenuA(Pacient, MF_POPUP, Menu_RegistroPaciente, "Reporte P&aciente");
	AppendMenuA(Menu, MF_POPUP, (UINT_PTR)Pacient, "Pa&ciente");
	AppendMenuA(Cit, MF_POPUP, Menu_VentanaCitas, "C&itas");
	AppendMenuA(Cit, MF_POPUP, Menu_VentanaMostrarCitas, "M&ostrar Citas");
	AppendMenuA(Menu, MF_POPUP, (UINT_PTR)Cit, "&Citas");
	AppendMenuA(Menu, MF_POPUP, Menu_VEsp, "&Especialidad");
	SetMenu(hWnd, Menu);
}

bool validarDatosM(Tree_Medico* _Med, Tree_Medico* first, HWND hWnd) {
	bool Nombr, Esp, Tel, Ced, Cons, Hor, Dia, HorarioTodos, Foto;
	Tree_Medico aux;
	aux.CopyNodeS(&aux, _Med);
	Nombr = ChecarNombre(_Med->Nombre, 16, 2);
	Esp = strcmp(_Med->Especialidad, "") != 0;
	Tel = ChecarTel(_Med->NumeroCel, _TreeMed->GetRootS(), aux);
	Ced = ChecarCed(_Med->clave, _TreeMed->GetRootS());
	Cons = _Med->NumConsultorio != 0;
	Hor = _Med->ValidarHorario(_MedTemp.HorarioAt);
	Dia = _Med->ValidarDias(_Med->DiasAt);
	HorarioTodos = _Med->validarHorarioMedicos(first, aux);
	Foto = strcmp(auxFoto, "") != 0;
	if (!Nombr || !Esp || !Tel || !Ced || !Cons || !Hor || !Dia || !HorarioTodos || !Foto) {
		if (!Nombr)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NombreError), "*");
		if (!Esp)
			SetWindowTextA(GetDlgItem(hWnd, IDC_EspecialidadError), "*");
		if (!Tel)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroError), "*");
		if (!Ced)
			SetWindowTextA(GetDlgItem(hWnd, IDC_CedulaError), "*");
		if (!Cons)
			SetWindowTextA(GetDlgItem(hWnd, IDC_ConsultorioError), "*");
		if (!Hor)
			SetWindowTextA(GetDlgItem(hWnd, IDC_HorasError), "*");
		if (!Dia)
			SetWindowTextA(GetDlgItem(hWnd, IDC_DiasError), "*");
		if (!HorarioTodos) {
			SetWindowTextA(GetDlgItem(hWnd, IDC_HorasError), "*");
			SetWindowTextA(GetDlgItem(hWnd, IDC_DiasError), "*");
			SetWindowTextA(GetDlgItem(hWnd, IDC_ConsultorioError), "*");
		}
		if (!Foto)
			SetWindowTextA(GetDlgItem(hWnd, IDC_FotoError), "*");
		return false;
	}
	else
		return true;
}

void CleanMedWindowM(HWND hWnd) {
	SetWindowTextA(GetDlgItem(hWnd, IDC_NombreMedico), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroMedico), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_CedulaMedico), "");
	SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraI), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraF), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBOCons), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBOEsp), CB_SETCURSEL, -1, 0);
	if (_MedTemp.DiasAt[0])
		CheckDlgButton(hWnd, IDC_Domingo, BST_UNCHECKED);
	if (_MedTemp.DiasAt[1])
		CheckDlgButton(hWnd, IDC_Lunes, BST_UNCHECKED);
	if (_MedTemp.DiasAt[2])
		CheckDlgButton(hWnd, IDC_Martes, BST_UNCHECKED);
	if (_MedTemp.DiasAt[3])
		CheckDlgButton(hWnd, IDC_Miercoles, BST_UNCHECKED);
	if (_MedTemp.DiasAt[4])
		CheckDlgButton(hWnd, IDC_Jueves, BST_UNCHECKED);
	if (_MedTemp.DiasAt[5])
		CheckDlgButton(hWnd, IDC_Viernes, BST_UNCHECKED);
	if (_MedTemp.DiasAt[6])
		CheckDlgButton(hWnd, IDC_Sabado, BST_UNCHECKED);
		SetWindowTextA(GetDlgItem(hWnd, IDC_NombreError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_EspecialidadError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_CedulaError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_ConsultorioError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_HorasError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_DiasError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_HorasError), "");
		SetWindowTextA(GetDlgItem(hWnd, IDC_FotoError), "");
}

bool validarDatosP(Paciente* _Pac, HWND hWnd) {
	bool Nombr, Apellido, Ref, MedNom, Genero, FechaNac, Edad, Numero;
	Nombr = ChecarNombre(_Pac->Nombre, 9, 0);
	Apellido = ChecarNombre(_Pac->Apellidos, 9, 1);
	Ref = ChecarDes(_Pac->Referencia, 10);
	MedNom = ChecarNombre(_Pac->MedPas, 15, 2);
	Genero = _Pac->Genero != 0;
	FechaNac = strcmp(_Pac->fechaNa, "000");
	if (strlen(_Pac->clave) == 8 || strlen(_Pac->clave) == 10)
		Numero = true;
	else Numero = false;
	Edad = strcmp(_PacTemp.Edad, "") != 0;
	if (!Nombr || !Apellido || !Ref || !MedNom || !Genero || !FechaNac || !Edad || !Numero) {
		if (!Nombr)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NombrePError), "*");
		if (!Apellido)
			SetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosError), "*");
		if (!Ref)
			SetWindowTextA(GetDlgItem(hWnd, IDC_RefError), "*");
		if (!MedNom)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NMedError), "*");
		if (!Genero)
			SetWindowTextA(GetDlgItem(hWnd, IDC_GeneroError), "*");
		if (!FechaNac)
			SetWindowTextA(GetDlgItem(hWnd, IDC_FechaPError), "*");
		if (!Edad)
			SetWindowTextA(GetDlgItem(hWnd, IDC_EdadError), "*");
		if (!Numero)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPError), "*");
		return false;
	}
	else
		return true;
}

void CleanPacWindowP(HWND hWnd) {
	SetWindowTextA(GetDlgItem(hWnd, IDC_NombrePaciente), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosPacien), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPaciente), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_PrimerMedico), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_ReferenciaPaciente), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_EdadPaciente), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NombrePError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_ApellidosError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_RefError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NMedError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_GeneroError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_FechaPError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_EdadError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NumeroPError), "");
	SYSTEMTIME SETACTUALDATE;
	GetLocalTime(&SETACTUALDATE);
	SendMessageA(GetDlgItem(hWnd, IDC_FechaNacimiento), MCM_SETCURSEL, 0, (LPARAM)&SETACTUALDATE);
	CheckDlgButton(hWnd, IDC_Masculino, BST_UNCHECKED);
	CheckDlgButton(hWnd, IDC_Femenino, BST_UNCHECKED);
}

bool validarDatosC(Citas Cita, Citas* first, HWND hWnd) {
	bool Apellido, Hora, Clave, CitaA;
	Apellido = strcmp(Cita.ApellidoP, "") != 0;
	Hora = strcmp(Cita.HoraC, "") != 0;
	Clave = strcmp(Cita.clave, "") != 0;
	CitaA = Cita.ValCitMed(Cita, first);
	if (!Apellido || !Hora || !Clave || !CitaA) {
		if (!Apellido)
			SetWindowTextA(GetDlgItem(hWnd, IDC_PacError), "*");
		if (!Clave)
			SetWindowTextA(GetDlgItem(hWnd, IDC_MedError), "*");
		if (!CitaA)
			SetWindowTextA(GetDlgItem(hWnd, IDC_FechaCError), "*");
		if (!Hora)
			SetWindowTextA(GetDlgItem(hWnd, IDC_HoraCError), "*");
		return false;
	}
	else
		return true;
}

void CleanCitWindowC(HWND hWnd) {
	SYSTEMTIME fecha;
	ZeroMemory(&fecha, sizeof(fecha));
	GetLocalTime(&fecha);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBOHoraCita), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBEsp), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBMedico), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_COMBPaciente), CB_SETCURSEL, -1, 0);
	SendMessageA(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), MCM_SETCURSEL, 0, (LPARAM)&fecha);
	SetWindowTextA(GetDlgItem(hWnd, IDC_TelefonoP), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_Consult), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_PacError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_MedError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_FechaCError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_HoraCError), "");
	EnableWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERF), false);
	EnableWindow(GetDlgItem(hWnd, IDC_COMBOHoraCita), false);
	ShowWindow(GetDlgItem(hWnd, IDC_RADIOPendiente), SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_RADIOAtendida), SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_CancelarCita), SW_HIDE);
	char aux[5] = { '\0' };
	AgregarMedicoBMP(aux, hWnd, IDC_FotoMedicoCi, Current);
}

bool validarDatosE(Especialidad Esp, HWND hWnd, int a) {
	bool Clave, Nombre, Des;
	Des = ChecarDes(_EspTemp.DescripcionEsp, 10);
	Clave = ChecarNumeros<Especialidad>(_EspTemp.clave, 5, a);
	Nombre = ChecarNombre<Especialidad>(_EspTemp.Nombre, 7, a);
	if (!Nombre || !Des || !Clave) {
		if (!Nombre)
			SetWindowTextA(GetDlgItem(hWnd, IDC_NEspError), "*");
		if (!Clave)
			SetWindowTextA(GetDlgItem(hWnd, IDC_IDError), "*");
		if (!Des)
			SetWindowTextA(GetDlgItem(hWnd, IDC_DesEspError), "*");
		return false;
	}
	else
		return true;
}

void CleanEspWindowE(HWND hWnd) {
	SetWindowTextA(GetDlgItem(hWnd, IDC_IDEspecial), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NomEspecial), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_DesEspecial), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_NEspError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_IDError), "");
	SetWindowTextA(GetDlgItem(hWnd, IDC_DesEspError), "");
}