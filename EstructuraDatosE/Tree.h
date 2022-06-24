#ifndef _Tree
#define _Tree
#include <fstream>

using namespace std;

template<class Node>
class Tree{
	Node* Root = 0;
	char file[100];
	Node* First = 0;
	Node* Last = 0;

	void GuardarTree(Node* aux, ofstream& tree_file);

	void AddNode(Node** nodo, Node* NuevoDato, Node* padre);

	Node* NodoMenor(Node* nodo);

	void Replace(Node* Tree, Node* nodo);

	void DestroyNode(Node* nodo);

	void CleanLinkedList(Node* Nodo);

	Node* quickSortReAct(Node* firstN, Node* end);

	Node* partition(Node* firstN, Node* end, Node** NewFirst, Node** NewLast);

	Node* getRefTail(Node* Act);

	void ConvertToLinkedList(Node* Nodo);

	Node* middle(Node* start, Node* last);

	Node* binarySearch(Node* head, char* clave);


public:
	Node** GetRoot();

	Node* GetRootS();

	Tree(const char*, char*);

	~Tree();

	void AddTreeNode(Node** nodo, Node dato);

	void ModifyTreeNode(Node* pos, Node cambio);

	Node* SearchTreeNode(Node* nodo, Node Dato);

	void DeleteNode(Node* NodeDel, char*);

	void quickSort(Node** FirstRef, char CurrentDirectory[100]);

	Node* BinarySearch(char* clave);
};

#endif