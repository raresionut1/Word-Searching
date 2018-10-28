//311CD Constantin Rares
//Tema2 SD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 1024
#define ELEMENT_TREE_LENGTH 3

#include "AVLTree.h"

typedef struct Range{
	int *index;
	int size;
	int capacity;
}Range;

void printFile(char* fileName){
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;
	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		printf("%s",buf);
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printWordsInRangeFromFile(Range* range, char* fileName){
	if(fileName == NULL || range == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;
	char *buf = (char*) malloc(BUFLEN+1);
	for(int i = 0; i < range->size;i++){
		fseek(file,range->index[i],SEEK_SET);
		if(fgets(buf,BUFLEN,file) != NULL){
			char* token = strtok(buf," .,\n");
			printf("%d. %s:%d\n",i+1, token, range->index[i]);
		}
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printTreeInOrderHelper(TTree* tree, TreeNode* node){
	if(node != NULL){
		printTreeInOrderHelper(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",*((int*)begin->info),((char*)begin->elem));
			begin = begin->next;
		}
		printTreeInOrderHelper(tree, node->rt);
	}
}

void printTreeInOrder(TTree* tree){
	if(tree == NULL) return;
	printTreeInOrderHelper(tree, tree->root);
}


void* createStrElement(void* str){
	//functie care aloca dinamic primele ELEMENT_TREE_LENGTH caractere ale unui
	//string
	char *s = malloc( (ELEMENT_TREE_LENGTH + 1) * sizeof(char));

	if(!s)
		return NULL;

	strncpy( s , ((char*) (str)), ELEMENT_TREE_LENGTH );
	s[ELEMENT_TREE_LENGTH]='\0';

	return s;
}

void destroyStrElement(void* elem){
	//functie care elibereaza memoria folosita pentru un element de tip string
	free((char*)elem);
}


void* createIndexInfo(void* index){
	//functie care aloca dinamic un index de tip long
	long *l = malloc(sizeof(long));
	*l = *((long*) (index));
	return l;
}

void destroyIndexInfo(void* index){
	//functie care elibereaza memoria folosita pentru un index de tip long
	free((long*)index);
}

int compareStrElem(void* str1, void* str2){
	//functie care compara 2 elemente de tip string
	return (int)strcmp( (char*)str1, (char*)str2);
}


TTree* buildTreeFromFile(char* fileName){
	//functie care construieste dictionarul pe baza cuvintelor din fisier
	 FILE *f = fopen(fileName, "rt");

	 if(!f){
	 	printf("File was not opened");
	 	return NULL;
	 }

	 TTree* tree = createTree(createStrElement, destroyStrElement,
			createIndexInfo, destroyIndexInfo,
			compareStrElem);

	 //calculul marimii fisierului (bytes)
	 fseek(f, 0, SEEK_END);
	 long marime = ftell(f);


	 char text[marime];
	 long index = 0;

	 //reseteaza pointerul
	 rewind(f);

	 fread(text, marime, 1, f);

	 //impartirea cuvintelor in tokenuri
	 char* p = strtok(text, " .,\r\n");


	 while(p != NULL){

	 	insert(tree, p, &index);
	 
	 	index = index + strlen(p) + 1;

	 	p = strtok(NULL, " .,\r\n");
	 }

	return tree;

	 
}


Range* singleKeyRangeQuery(TTree* tree, char* q){
	//functie prin care se obtin toti indecsii cuvintelor care incep cu un
	//anumit string
	 Range* range = malloc(sizeof(Range));
	 range->index = malloc(sizeof(int));
	 range->size = 0;
	 range->capacity = 1;

	 TreeNode* node = minimum(tree, tree->root);

	 int length = strlen(q);

	 while(node != NULL){

	 	char* elem;
	 	strncpy(elem , (char*)node->elem, length );
		elem[length] = '\0';

	 	if(tree->compare(q, elem) == 0){

	 		if(range->size == range->capacity){

	 			range->capacity = 2 * range->capacity;
	 			range->index = realloc(range->index, 
	 								  (range->capacity)*sizeof(int));
	 		}

	 		range->size++;

	 		range->index[range->size-1] = *(int*)node->info;

	 		node = node->next;
			continue;
	 	}

	 	if(tree->compare(q, elem) == -1)
	 		break;

	 	node = node->next;
	 }

	 return range;
}

Range* multiKeyRangeQuery(TTree* tree, char* q, char* p){
	//functie prin care se obtin toti indecsii cuvintelor care se afla din
	//punct de vedere lexicografic intr-un anumit range, intre 2 stringuri
	 Range* range = malloc(sizeof(Range));
	 range->index = malloc(sizeof(int));
	 range->size = 0;
	 range->capacity = 1;

	 TreeNode* node = minimum(tree, tree->root);

	 int length1 = strlen(q);
	 int length2 = strlen(p);

	 while(node != NULL){

	 	char* elem1;
	 	char* elem2;

	 	strncpy(elem1 , (char*)node->elem, length1 );
		elem1[length1] = '\0';

		strncpy(elem2 , (char*)node->elem, length2 );
		elem2[length2] = '\0';

	 	if( (tree->compare(q, elem1) <= 0) &&
	 		(tree->compare(p, elem2) >= 0)) {

	 		if(range->size == range->capacity){

	 			range->capacity = 2 * range->capacity;
	 			range->index = realloc(range->index, 
	 								  (range->capacity)*sizeof(int));
	 		}

	 		range->size++;

	 		range->index[range->size-1] = *(int*)node->info;

	 		node = node->next;
			continue;
	 	}

	 	if(tree->compare(p, elem2) == -1)
	 		break;

	 	node = node->next;
	 }

	 return range;
}


int main(void) {

	printf("The text file:\n");
	printFile("text.txt");

	TTree* tree = buildTreeFromFile("text.txt");
	printf("Tree In Order:\n");
	printTreeInOrder(tree);
	printf("\n\n");

	printf("Single search 'v':\n");
	Range *range = singleKeyRangeQuery(tree,"v");
	printWordsInRangeFromFile(range,"text.txt");

	printf("Single search 'p':\n");
	range = singleKeyRangeQuery(tree,"p");
	printWordsInRangeFromFile(range,"text.txt");

	printf("Single search 'pr':\n");
	range = singleKeyRangeQuery(tree,"pr");
	printWordsInRangeFromFile(range,"text.txt");

	printf("Single search 'pri':\n");
	range = singleKeyRangeQuery(tree,"pri");
	printWordsInRangeFromFile(range,"text.txt");

	printf("Multi search 'j--pr':\n");
	Range *range2 = multiKeyRangeQuery(tree,"j","pr");
	printWordsInRangeFromFile(range2,"text.txt");

	if(range != NULL) free(range->index);
	free(range);

	if(range2 != NULL) free(range2->index);
	free(range2);

	destroyTree(tree);
	return 0;
}





