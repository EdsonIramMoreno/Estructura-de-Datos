#ifndef _Linked_List
#define _Linked List

template<class Nodo>
class Linked_List{
		Nodo* first;
		Nodo* last;
		char file[100];

	public:
		Nodo* Getfirst();

		Nodo* Getlast();

		Linked_List(const char* fileA, char* current);

		~Linked_List();

		void DeleteNodo(Nodo* pos);

		void ModifyNodo(Nodo* pos, Nodo cambio);

		//void MostrarLista(char opc, HWND hWnd, Nodo* first);

		void AddNodo(Nodo datos);

		Nodo* SearchNodo(short pos);

};
#endif