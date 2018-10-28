//311CD Constantin Rares
//Tema2 SD

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))
#define HEIGHT(x) ((x)?((x)->height):(0))

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just assume
  some type Item was previously defined.
 */
// -----------------------------------------------------------------------------

typedef struct node{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;

void preOrderPrint(TTree* tree, TreeNode* x){
	//functie folosita pentru debugging

	if (x != NULL) {
		printf("%li ", *((long*)x->elem) );
		preOrderPrint(tree, x->lt);
		preOrderPrint(tree, x->rt);
	}

}

void printList(TTree* tree, TreeNode* x){
	//functie folosita pentru debugging
	if(x != NULL){
		printf("%li ", *((long*)x->elem) );
		printList(tree, x->next);
	}
}


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){
	//crearea arborelului si atribuirea pointerilor
	 TTree* arbore = malloc(sizeof(TTree));
	if(!arbore)
		return NULL;

	arbore->root = NULL;
	arbore->createElement = createElement;
	arbore->destroyElement = destroyElement;
	arbore->createInfo = createInfo;
	arbore->destroyInfo = destroyInfo;
	arbore->compare = compare;
	arbore->size = 0;

	return arbore;
}

void adjustHeight(struct TTree* tree, TreeNode* aici){
	//functie folosita pentru a ajusta inaltimea unui arbore care este
	//modificat in urma unui insert sau unui delete
	TreeNode* node = aici;

	while(node != NULL){

		long left, right;

		if(!node->lt)
			left = 0;
		else
			left = node->lt->height;

		if(!node->rt)
			right = 0;
		else
			right = node->rt->height;


		node->height = 1 + MAX(left, right);
		
		
		node = node->pt;
			
	}
}

int isEmpty(TTree* tree){
	//functie care verifica daca un arbore este gol
	if(tree->size == 0)
		return 1;

	return 0;
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info){	
	//functie care creeaza un nod de arbore si aloca memorie pentru
	//element si info
	TreeNode* nou = malloc(sizeof(TreeNode));
	if(!nou)
		return NULL;

	 nou->pt = NULL;
	 nou->rt = NULL;
	 nou->lt = NULL;

	 nou->next = NULL;
	 nou->prev = NULL;
	 nou->end = nou;

	 nou->elem = tree->createElement(value);
	 nou->info = tree->createInfo(info);
	 nou->height = 0;

	 return nou;
}




TreeNode* search(TTree* tree, TreeNode* x, void* elem){
	//functie care cauta in arborele cu radacina x un anumit element
	TreeNode *node = x;

	while(node != NULL){

		if(tree->compare(elem, node->elem) == 0)
			return node;
		

		if(tree->compare(elem, node->elem) == -1){
			node = node->lt;
			continue;
		}
		

		if(tree->compare(elem, node->elem) == 1)
			node = node->rt;
		
	}

	return NULL;
}

TreeNode* minimum(TTree*tree, TreeNode* x){
	//functie care cauta nodul cu cel mai mic element din arborele cu
	//radacina x
	TreeNode* y = NULL;

	while (x != NULL){
		y = x;
		x = x->lt;
	}

	return y;
}

TreeNode* maximum(TTree* tree, TreeNode* x){
	//functie care cauta nodul cu cel mai mare element din arborele cu
	//radacina x
	TreeNode* y = NULL;

	while(x != NULL){
		y = x;
		x = x->rt;
	}

	return y;
}

TreeNode* successor(TTree* tree, TreeNode* x){
	//functie care cauta succesorul nodului x in arbore
	TreeNode* node = minimum(tree, x->rt);

	if(node)
		return node;

	node = x;

	while(node->pt != NULL){

		if(tree->compare(node->pt->elem, node->elem) == 1)
			return node->pt;

		node = node->pt;
	}

	return NULL;
}

TreeNode* predecessor(TTree* tree, TreeNode* x){
	//functie care cauta predecesorul nodului x in arbore
	TreeNode* node = maximum(tree, x->lt);

	if(node)
		return node;

	node = x;

	while(node->pt != NULL){

		if(tree->compare(node->pt->elem, node->elem) == -1)
			return node->pt;

		node = node->pt;
	}

	return NULL;
}

void avlRotateLeft(TTree* tree, TreeNode* x){
	//functie care efectueaza o rotatie la stanga a nodului x
	if(x->rt == NULL)
		return;

	TreeNode* y = x->rt;

	if(x->pt){
		if(x->pt->lt == x)
			x->pt->lt = y;
		else
			x->pt->rt = y;
	}
	else
		tree->root = y;

	y->pt = x->pt;
	x->pt = y;

	x->rt = y->lt;
	if(y->lt)
		y->lt->pt = x;

	y->lt = x;

	long left, right;

	if(!x->lt)
		left = 0;
	else
		left = x->lt->height;

	if(!x->rt)
		right = 0;
	else
		right = x->rt->height;

	x->height = 1 + MAX(left, right);

	if(!y->lt)
		left = 0;
	else
		left = y->lt->height;

	if(!y->rt)
		right = 0;
	else
		right = y->rt->height;

	y->height = 1 + MAX(left, right);
}


void avlRotateRight(TTree* tree, TreeNode* y){
	//functie care efectueaza o rotatie la dreapta a nodului y
	if(y->lt == NULL)
		return;

	TreeNode* x = y->lt;

	if(y->pt){
		if(y->pt->rt == y)
			y->pt->rt = x;
		else
			y->pt->lt = x;
	}
	else
		tree->root = x;

	x->pt = y->pt;
	y->pt = x;

	y->lt = x->rt;
	if(x->rt)
		x->rt->pt = y;

	x->rt = y;

	long left, right;

	if(!y->lt)
		left = 0;
	else
		left = y->lt->height;

	if(!y->rt)
		right = 0;
	else
		right = y->rt->height;

	y->height = 1 + MAX(left, right);

	if(!x->lt)
		left = 0;
	else
		left = x->lt->height;

	if(!x->rt)
		right = 0;
	else
		right = x->rt->height;

	x->height = 1 + MAX(left, right);
}


long avlGetBalance(TTree* tree, TreeNode *x){
	//functie care calculeaza diferenta de inaltimi a copiilor nodului x
	long left, right;

	if (x == NULL)
		return 0;

	if(x->lt == NULL)
		left = 0;
	else
		left = x->lt->height;

	if(x->rt == NULL)
		right = 0;
	else
		right = x->rt->height;

	return left - right;
}


void avlFixUp(TTree* tree, TreeNode* y){
	//functie care echilibreaza primul nod dezechilibrat de la nodul y spre
	//radacina arborelui
	TreeNode* node = y;

	while(node != NULL){

			long balance = avlGetBalance(tree, node);
			

			if( balance > 1 ){
				if(avlGetBalance(tree, node->lt) >= 0)
					avlRotateRight(tree, node);
				else{
					avlRotateLeft(tree, node->lt);
					avlRotateRight(tree, node);
				}
				return;
			}

			if( balance < -1 ){
				if(avlGetBalance(tree, node->rt) <= 0)
					avlRotateLeft(tree, node);
				else{
					avlRotateRight(tree, node->rt);
					avlRotateLeft(tree, node);
				}
				return;
			}

			node = node->pt;

		}
}

void insertInList(TTree* tree, TreeNode* nou){
	//functie care realizeaza legaturile pentru lista de noduri
	TreeNode* node = minimum(tree, tree->root);

	while(node){
		if(tree->compare(node->elem, nou->elem) == 1){
			nou->next = node;
			if(node->prev){
				nou->prev = node->prev;
				node->prev->next = nou;
			}
			node->prev = nou;
			return;
		}

		if(!node->next){
			node->next = nou;
			nou->prev = node;
			return;
		}

	node = node->next;

	}
	
}


TreeNode* justInsert(TTree* tree, void* elem, void* info){
	//functie care insereaza un nod in arbore, fara a-l echilibra
	TreeNode *nou = createTreeNode(tree, elem, info);
	if(!nou)
		return NULL;

	insertInList(tree, nou);

	if(isEmpty(tree)){
		tree->root = nou;
		nou->pt = NULL;
		tree->size ++;
		adjustHeight(tree, nou);
		return nou;
	}

	TreeNode *node = tree->root;

	while(1){

		if(tree->compare(elem, node->elem) == 0){
			node->end = nou;

			return nou;
		}

		if(tree->compare(elem, node->elem) == -1){

			if(node->lt == NULL){
				node->lt = nou;
				nou->pt = node;
				tree->size ++;
				adjustHeight(tree, nou);
				return nou;
			}
			else
				node = node->lt;
		}

		if(tree->compare(elem, node->elem) == 1){

			if(node->rt == NULL){
				node->rt = nou;
				nou->pt = node;
				tree->size ++;
				adjustHeight(tree, nou);
				return nou;
			}
			else
				node = node->rt;
		}
	}
}



void insert(TTree* tree, void* elem, void* info) {
	//functie care insereaza un nod in arbore si il si echilibreaza
	TreeNode* node = justInsert(tree, elem, info);
	if(!node)
		return;

	avlFixUp(tree, node);

	
}

void destroyTreeNode(TTree *tree, TreeNode* node){
	//functie care sterge un nod din arbore
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);
	free(node);
}

void deleteFromList(TTree *tree, TreeNode* x){
	//functie care elimina un nod din lista
	if(x->prev != NULL)
		x->prev->next = x->next;
	if(x->next != NULL)
		x->next->prev = x->prev;

}

void deleteNoDuplicate(TTree* tree, TreeNode* x){
	//functie care elimina si sterge un nod neduplicat din arbore
	TreeNode* aux;

	if(!x->lt || !x->rt){

		deleteFromList(tree, x);

		if(x->lt)
			aux = x->lt;
		else
			aux = x->rt;

		if(aux)
			aux->pt = x->pt;

		//e chiar radacina
		if(tree->root == x){
			tree->root = aux;
			destroyTreeNode(tree, x);
			tree->size --;

			return;
		}
		
		if(x->pt->rt == x)
			x->pt->rt = aux;
		else
			x->pt->lt = aux;
		
		TreeNode* node = x->pt;
		destroyTreeNode(tree, x);

		adjustHeight(tree, node);

		do{
			avlFixUp(tree, node);
			node = node->pt;

		}while(node != NULL);

		tree->size --;

		return;
	}

	if(x->lt && x->rt){
		//cazul in care nodul are 2 copii
		TreeNode* succ = successor(tree, x);
		tree->destroyElement(x->elem);
		tree->destroyInfo(x->info);
		x->elem = tree->createElement(succ->elem);
		x->info = tree->createInfo(succ->info);

		if(succ->next)
			succ->next->prev = x;
		x->next = succ->next;

		if(succ->end != succ)
			x->end = succ->end;

		deleteNoDuplicate(tree, succ);
		
	}

}

void delete(TTree* tree, void* elem){
	//functie care sterge un nod cu un anumit element din arbore/lista
	TreeNode* x = search(tree, tree->root, elem);

	if(x->end != x){
		TreeNode* aux = x->end;
		x->end = aux->prev;
		deleteFromList(tree, aux);
		destroyTreeNode(tree, aux);
	 	return;
	}

	deleteNoDuplicate(tree, x);

}

void destroyTree(TTree* tree){
	//functie care elibereaza toata memoria ocupata de arbore si de noduri
	TreeNode* node = minimum(tree, tree->root);

	while(node){

		TreeNode* aux = node;

		node = node->next;

		destroyTreeNode(tree, aux);
	}

	free(tree);
}




#endif /* AVLTREE_H_ */
