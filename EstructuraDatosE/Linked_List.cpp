#include "Linked_List.h"
#include "Especialidad.h"
#include "Paciente.h"
#include "Citas.h"
#include <iostream>
#include <fstream>

using namespace std;


template<typename Nodo>
	Nodo* Linked_List<Nodo>::Getfirst() {
		return first;
	}

template<typename Nodo>
	Nodo* Linked_List<Nodo>::Getlast() {
		return last;
	}

template<typename Nodo>
	void Linked_List<Nodo>::AddNodo(Nodo datos) {
		Nodo* aux = new Nodo;
		*aux = datos;

		aux->Prev = aux->Sig = NULL;

		if (first == NULL) {
			first = aux;
			last = aux;
		}
		else {
			last->Sig = aux;
			aux->Prev = last;
			last = aux;
		}
	}
	
template<typename Nodo>
	Linked_List<Nodo>::Linked_List(const char* fileA, char* current) {
		char fileC[100] = { '\0' };
		strcpy_s(fileC, current);
		strcat_s(fileC, fileA);
		strcpy_s(this->file, fileC);
		Nodo aux;
		ifstream file_list;
		file_list.open(this->file, ios::binary);
		if (file_list.is_open()) {
			file_list.read((char*)&aux, sizeof(Nodo));
				while (!file_list.eof()) {
					AddNodo(aux);
					file_list.read((char*)&aux, sizeof(Nodo));
				}
		}
		file_list.close();
	}

	//Lista(const char* file) {
	//	Nodo aux;
	//	this->file = file;
	//	ifstream file_list;
	//	file_list.open(file, ios::binary);
	//	if (file_list.is_open()) {
	//		file_list.read((char*)&aux, sizeof(Nodo));
	//		while (!file_list.eof()) {
	//			AgregarNodo(aux);
	//			file_list.read((char*)&aux, sizeof(Nodo));
	//		}
	//	}
	//	file_list.close();
	//}

template<typename Nodo>
	Linked_List<Nodo>::~Linked_List() {
		Nodo* aux = this->first;
		ofstream file_list;
		file_list.open(this->file, ios::binary);
		if (file_list.is_open()) {
			while (aux != 0) {
				file_list.write((char*)aux, sizeof(Nodo));
				//delete aux;
				aux = aux->Sig;
			}
		}
		file_list.close();
		delete aux;
}

	//~Lista() {
	//	Nodo* aux = first;
	//	ofstream file_list;
	//	file_list.open(file, ios::binary);
	//	if (file_list.is_open()) {
	//		while (aux != 0) {
	//			file_list.write((char*)aux, sizeof(Nodo));
	//			//delete aux;
	//			aux = aux->Sig;

	//		}
	//	}
	//	file_list.close();
	//}

template<typename Nodo>
	void Linked_List<Nodo>::DeleteNodo(Nodo* pos) {
		Nodo* aux = pos;
		if (aux == first) {
			if (aux->Sig == NULL) {
				delete aux;
				aux = first = last = NULL;
			}
			else {
				first = aux->Sig;
				aux->Sig->Prev = first;
				delete aux;
			}
		}
		else if (aux == last) {
			last = aux->Prev;
			last->Sig = NULL;
			delete aux;
		}
		else {
			aux->Sig->Prev = aux->Prev;
			aux->Prev->Sig = aux->Sig;
			delete aux;
		}
}

	template<typename Nodo>
	void Linked_List<Nodo>::ModifyNodo(Nodo* pos, Nodo cambio) {
		pos->CopyNode(pos, &cambio);
	}

//template<typename Nodo>
//	void Linked_List<Nodo>::MostrarLista(char opc,HWND hWnd,Nodo* first) {
//		if (opc == 'E') {
//			
//		}
//	}

	
	template<typename Nodo>
	Nodo* Linked_List<Nodo>::SearchNodo(short pos) {
		Nodo* aux = first;
		short i = 0;
		while (i != pos) {
			aux = aux->Sig;
			i++;
		}
		return aux;
		delete aux;
	}

	template Linked_List<Especialidad>;
	template Linked_List<Citas>;
	template Linked_List<Paciente>;

	/*

	void sortedInsert(Node** NodoRef, Node* newNode) {
		Node* Actual = NULL;
		if (*NodoRef == NULL)
			*NodoRef = newNode;
		else if ((*NodoRef)->precio >= newNode->precio) {
			newNode->Sig = *NodoRef;
			newNode->Sig->ant = newNode;
			*NodoRef = newNode;
		}
		else {
			Actual = *NodoRef;
			while (Actual->Sig != NULL && Actual->Sig->precio < newNode->precio) {
				Actual = Actual->Sig;
			}
			newNode->Sig = Actual->Sig;
			if (Actual->Sig != NULL)
				newNode->Sig->ant = newNode;
			Actual->Sig = newNode;
			newNode->ant = Actual;
		}
	}

	

	

	void insertionSort(Node** NodoRef) {
		Node* Actual = *NodoRef;
		Node* sorted = 0;
		while (Actual != 0) {
			Node* Sig = Actual->Sig;
			Actual->ant = Actual->Sig = NULL;
			sortedInsert(&sorted, Actual);
			Actual = Sig;
		}
		*NodoRef = sorted;
	}*/