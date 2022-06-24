#include "Tree.h"
#include "DatMedico.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

	template<typename Node>
	Node** Tree<Node>::GetRoot() {
		return &this->Root;
	}

	template<typename Node>
	Node* Tree<Node>::GetRootS() {
		return this-> Root;
	}

	template<typename Node>
	Tree<Node>::Tree(const char* fileA, char* current) {
		char fileC[100] = { '\0' };
		strcpy_s(fileC, current);
		strcat_s(fileC, fileA);
		strcpy_s(this->file, fileC);
		Node aux;
		ifstream file_tree;
		file_tree.open(this->file, ios::in);
		if (file_tree.is_open()) {
			file_tree.read((char*)&aux, sizeof(Node));
			while (!file_tree.eof()) {
				AddTreeNode(&this->Root, aux);
				file_tree.read((char*)&aux, sizeof(Node));
			}
		}
		file_tree.close();
	}

	template<typename Node>
	void Tree<Node>::GuardarTree(Node* aux, ofstream& tree_file) {
		if (aux != 0) {
			tree_file.write((char*)aux, sizeof(Node));
			this->GuardarTree(aux->RL, tree_file);
			this->GuardarTree(aux->RR, tree_file);
		}
	}

	template<typename Node>
	Tree<Node>::~Tree() {
		ofstream file_tree;
		file_tree.open(this->file, ios::binary);
		if (file_tree.is_open()) {
			this->GuardarTree(this->Root, file_tree);
		}
		file_tree.close();
	}

	template<typename Node>
	void Tree<Node>::DeleteNode(Node* NodeDel, char* current) {
		char dir[100];
		strcpy_s(dir, current);
		strcat_s(dir, "\\");
		if (NodeDel->RR && NodeDel->RL) {
			Node* aux = NodoMenor(NodeDel->RR);
			aux->CopyNodeDel(NodeDel, aux);
			DeleteNode(aux,current);
		}
		else if (NodeDel->RL) {
			Replace(NodeDel, NodeDel->RL);
			strcat_s(dir, NodeDel->FotoMedico);
			DeleteFileA(dir);
			DestroyNode(NodeDel);
		}
		else if (NodeDel->RR) {
			Replace(NodeDel, NodeDel->RR);
			strcat_s(dir, NodeDel->FotoMedico);
			DeleteFileA(dir);
			DestroyNode(NodeDel);
		}
		else {
			Replace(NodeDel, NULL);
			strcat_s(dir, NodeDel->FotoMedico);
			DeleteFileA(dir);
			DestroyNode(NodeDel);
		}
	}

	template<typename Node>
	void Tree<Node>::DestroyNode(Node* nodo) {
		nodo->RR = NULL;
		nodo->RL = NULL;
		nodo->PP = NULL;

		delete nodo;
		nodo = NULL;
	}

	template<typename Node>
	void Tree<Node>::Replace(Node* Tree, Node* nodo) {
		if (Tree->PP != NULL) {
			if (Tree->PP->RL != NULL) {
				if (strcmp(Tree->clave, Tree->PP->RL->clave) == 0) {
					Tree->PP->RL = nodo;
				}
			}
			else if (strcmp(Tree->clave, Tree->PP->RR->clave) == 0) {
				Tree->PP->RR = nodo;
			}
		}
		else {
			if (Tree->RL != NULL && Tree->RR == NULL)
				Root = Tree->RL;
			else if (Tree->RL == NULL && Tree->RR != NULL)
				Root = Tree->RR;
			else
				Root = NULL;
		}
		if (nodo) {
			nodo->PP = Tree->PP;
		}
	}

	template<typename Node>
	Node* Tree<Node>::NodoMenor(Node* nodo) {
		if (nodo == NULL) {
			return NULL;
		}
		if (nodo->RL) {
			return NodoMenor(nodo->RL);
		}
		else {
			return nodo;
		}
	}
	
	template<typename Node>
	void Tree<Node>::AddTreeNode(Node** nodo, Node dato) {
		Node* NuevoNodo = new Node;
		NuevoNodo->CopyNode(NuevoNodo, dato);
		if (*nodo == NULL) {
			*nodo = NuevoNodo;
		}
		else {
			AddNode(nodo, NuevoNodo, NuevoNodo);
		}
	}

	template<typename Node>
	void Tree<Node>::AddNode(Node** nodo, Node* NuevoDato, Node* padre) {
		Node* aux = *nodo;
		if (aux == NULL) {
			*nodo = NuevoDato;
			(*nodo)->PP = padre;
		}
		else {
			/*int Nnuevo = atoi(NuevoDato->Clave);
			int Nnodo = atoi(aux->Clave);*/
			if (strcmp(NuevoDato->clave, aux->clave) > 0) {
				AddNode(&(aux->RR), NuevoDato, aux);
			}
			else if (strcmp(NuevoDato->clave, aux->clave) < 0) {
				AddNode(&(aux->RL), NuevoDato, aux);
			}
			//else
				//cout << "Error";
		}
	}

	template<typename Node>
	void Tree<Node>::ModifyTreeNode(Node* pos, Node cambio) {
		pos->ModifyNode(pos, cambio);
	}

	template<typename Node>
	Node* Tree<Node>::SearchTreeNode(Node* nodo, Node Dato) {
		if (nodo != NULL) {
			if (strcmp(nodo->clave, Dato.clave) == 0) {
				return nodo;
			}
			else {
				if (strcmp(nodo->clave, Dato.clave) < 0) {
					return SearchTreeNode(nodo->RR, Dato);
				}
				if (strcmp(nodo->clave, Dato.clave) > 0) {
					return SearchTreeNode(nodo->RL, Dato);
				}
			}
		}
		return 0;
	}

	template<typename Node>
	void Tree<Node>::ConvertToLinkedList(Node* Nodo) {
		if (Nodo != NULL) {
			ConvertToLinkedList(Nodo->RL);
			Node* aux = Nodo;
			aux->prev = aux->sig = NULL;
			if (First == NULL) {
				First = aux;
				Last = aux;
			}
			else {
				Last->sig = aux;
				aux->prev = Last;
				Last = aux;
			}
			aux = 0;
			delete aux;
			ConvertToLinkedList(Nodo->RR);
		}
	}

	template<typename Node>
	void Tree<Node>::quickSort(Node** FirstRef, char CurrentDirectory[100]) {
		this->ConvertToLinkedList(this->Root);
		(*FirstRef) = this->quickSortReAct(*FirstRef, getRefTail(*FirstRef));
		(*FirstRef)->FillDocMed(this->First, CurrentDirectory);
		this->CleanLinkedList(this->Root);
		this->First = 0;
		this->Last = 0;
	}

	template<typename Node>
	Node* Tree<Node>::getRefTail(Node* Act) {
		while (Act != NULL && Act->sig != NULL)
			Act = Act->sig;
		return Act;
	}

	template<typename Node>
	Node* Tree<Node>::partition(Node* firstN, Node* end, Node** NewFirst, Node** NewLast) {
		Node* pivot = end;
		Node* prev = NULL, * Act = firstN, * RefTail = pivot;

		while (Act != pivot)
		{
			if (strcmp(Act->clave, pivot->clave) < 0) {
				if ((*NewFirst) == NULL)
					(*NewFirst) = Act;

				prev = Act;
				Act = Act->sig;
			}
			else {
				if (prev)
					prev->sig = Act->sig;
				Node* temp = Act->sig;
				Act->sig = NULL;
				RefTail->sig = Act;
				RefTail = Act;
				Act = temp;
				/*temp = 0;
				delete temp;*/
			}
		}
		if ((*NewFirst) == NULL)
			(*NewFirst) = pivot;
		(*NewLast) = RefTail;
		return pivot;
	}

	template<typename Node>
	Node* Tree<Node>::quickSortReAct(Node* firstN, Node* end){
		if (!firstN || firstN == end)
			return firstN;

		Node* NewFirst = NULL, * NewLast = NULL;

		Node* pivot = partition(firstN, end, &NewFirst, &NewLast);

		if (NewFirst != pivot) {
			Node* temp = NewFirst;
			while (temp->sig != pivot)
				temp = temp->sig;
			temp->sig = NULL;

			NewFirst = quickSortReAct(NewFirst, temp);

			temp = getRefTail(NewFirst);
			temp->sig = pivot;
		}

		pivot->sig = quickSortReAct(pivot->sig, NewLast);

		return NewFirst;
	}

	template<typename Node>
	void Tree<Node>::CleanLinkedList(Node* Nodo) {
		if (Nodo != 0) {
			CleanLinkedList(Nodo->RL);
			Nodo->sig = 0;
			Nodo->prev = 0;
			CleanLinkedList(Nodo->RR);
		}
	}

	template<typename Node>
	Node* Tree<Node>::middle(Node* start, Node* last) {
		if (start == NULL)
			return NULL;
		Node* slow = start;
		Node* fast = start->sig;
		while (fast != last) {
			fast = fast->sig;
			if (fast != last){
				slow = slow->sig;
				fast = fast->sig;
			}
		}

		return slow;
	}

	template<typename Node>
	Node* Tree<Node>::binarySearch(Node* head, char* clave) {
		Node* start = head;
		Node* last = NULL;

		do {
			Node* mid = this->middle(start, last);
			if (mid == NULL)
				return NULL;
			if (strcmp(mid->clave, clave) == 0)
				return mid;
			else if (strcmp(mid->clave, clave) <= 0)
				start = mid->sig;
			else
				last = mid;
		} while (last == NULL || last != start);

		return NULL;
	}

	template<typename Node>
	Node* Tree<Node>::BinarySearch(char* clave) {
		Node* aux = 0;
		this->ConvertToLinkedList(this->Root);
		aux = binarySearch(this->First, clave);
		this->CleanLinkedList(this->Root);
		this->First = 0;
		this->Last = 0;
		return aux;
	}

template Tree<Tree_Medico>;