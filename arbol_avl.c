#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
	int key, height;
	struct Node *left, *right; 
} Node;

int Max(int a, int b){
	return (a > b) ? a : b;
}

int nodesCount(Node *root) {
	if(!root)
		return(0);
	return(nodesCount(root->left) + 1 + nodesCount(root->right)); 
}

void deleteTree(Node *root) {
	if(root) {
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}

Node *searchKey(Node *root, int key) {
	if(!root) 
		return root;
	if(key < root->key) 
		return(searchKey(root->left, key)); 
	else if(key > root->key) 
		return(searchKey(root->right, key)); 
	else 
		return root;
}

int height(Node *node){
	if(!node) 
		return(0); 
	return node->height; 
}

int getBalance(Node *node) {
	if(!node)
		return 0;
	return height(node->left) - height(node->right);
}

Node *rightRotation(Node *node) {
	Node *aux = node->left;
	node->left = aux->right;
	aux->right = node;
	node->height = Max(height(node->left), height(node->right)) + 1;
	aux->height = Max(height(aux->left), height(aux->right)) + 1;
	return aux; 
}

Node *leftRotation(Node *node){
	Node *aux = node->right;
	node->right = aux->left;
	aux->left = node;
	node->height = Max(height(node->left), height(node->right)) + 1;
	aux->height = Max(height(aux->left), height(aux->right)) + 1;
	return aux;
}

Node *doubleRightRotation(Node *node) {
	node->left = leftRotation(node->left);
	return rightRotation(node);
}

Node *doubleLeftRotation(Node *node) {
	node->right = rightRotation(node->right); 
	return leftRotation(node);
}

Node *insertNode(Node *root, int key) {
	if(!root) {
		root = (Node *)malloc(sizeof(Node));
		root->key = key;
		root->height = 1;
		root->left = NULL;
		root->right = NULL;
		return root;
	}
	if(key < root->key) 
		root->left = insertNode(root->left, key);
	else if(key > root->key) 
		root->right = insertNode(root->right, key);
	else
		return root;
	root->height = 1 + Max(height(root->left), height(root->right));
	int balance = getBalance(root);
	if (balance > 1 && key < root->left->key) 
        return rightRotation(root); 
    if (balance > 1 && key > root->right->key) 
        return leftRotation(root);
    if (balance > 1 && key > root->left->key) 
        return doubleRightRotation(root); 
    if (balance > 1 && key < root->right->key) 
        return doubleLeftRotation(root);
	return root;
}

void displayTree(Node *root, int level) {
	if(root) {
		int i;
		displayTree(root->right, level + 1);
		printf("\n\n");
		for(i = 0; i < level; i++) 
			printf("\t");
		printf("%d",root->key);
		displayTree(root->left, level + 1);
	}
}

/*Devuelve el nodo con el dato menor del árbol*/
Node * minValueNode(Node *root) { 
    Node* min = root; 
    while (root->left) 
        min = min->left; 
    return min; 
} 

Node* deleteNode(Node *root, int key) { 
    if (!root) 
        return root;  
    if (key < root->key ) 
        root->left = deleteNode(root->left, key); 
    else if(key > root->key ) 
        root->right = deleteNode(root->right, key); 
    else {
		Node *tmp = NULL;
        if( (!root->left) || (!root->right) ) { 
            tmp = root->left ? root->left : root->right; 
            if (!tmp) { 
                tmp = root; 
                root = NULL; 
            } 
            else
            	*root = *tmp;
            free(tmp); 
        } else { 
            tmp = minValueNode(root->right); 
            root->key = tmp->key; 
            root->right = deleteNode(root->right, tmp->key); 
        } 
    }
    if (!root)
      	return root;
    root->height = 1 + Max(height(root->left), height(root->right));
    int balance = getBalance(root); 
    if (balance > 1 && getBalance(root->left) >= 0) 
        return rightRotation(root); 
    if (balance > 1 && getBalance(root->left) < 0) 
		return doubleRightRotation(root);
    if (balance < -1 && getBalance(root->right) <= 0) 
        return leftRotation(root);
    if (balance < -1 && getBalance(root->right) > 0) 
		return doubleLeftRotation(root);
    return root; 
} 

void main() {
	int n, i, key, op;
	Node *root = NULL, *result = NULL;
	while(1){
		printf("\n\n\tOperaciones con Árboles AVL\n\n"); 
		printf("1. Llenar un árbol de forma aleatoria\n"); 
		printf("2. Llenar un árbol de forma manual\n"); 
		printf("3. Buscar un dato\n");
		printf("4. Contar Nodos y Altura\n");
		printf("5. Ver árbol\n");
		printf("6. Borrar nodo\n");
		printf("7. Salir\n");
		printf("Elija una Opción : ");
		scanf("%d",&op);
		switch(op) {
			case 1:
				printf("Numero de nodos del árbol: "); 
				scanf("%d",&n);
				srand(time(NULL));
				for(i = 0; i < n; i++) {
					printf("\n\tElemento No. %d: ",i + 1);
					key = rand()/100000000; 
					printf("%d", key);
					root = insertNode(root, key);
				} 
				break;
			case 2:
				printf("Numero de nodos del árbol: "); 
				scanf("%d",&n);
				for(i = 0; i < n; i++) {
					printf("\n\tElemento No. %d: ",i+1);
					scanf("%d", &key);
					root = insertNode(root, key);
				}
            	break;
			case 3:
				printf("\n\tDato a Buscar: "); 
				scanf("%d", &key);
				result = searchKey(root, key);
				if(result) 
					printf("Se encontró el dato %d", key); 
				else 
					printf("No se encontró el dato %d", key); 
				break;
			case 4:
				printf("\n\n\tNodos : %d\n\n\tAltura =%d", nodesCount(root), height(root)); 
				break;
      		case 5:
      			printf("Árbol AVL"); 
      			displayTree(root, 0);
      			break;
			case 6:
				printf("\n\tDato a borrar: "); 
				scanf("%d", &key);
				result = searchKey(root, key);
				if(result){
					root = deleteNode(root, key);
					printf("Se borró el dato %d", key); 
				} else
					printf("No se encontró el dato %d", key); 
				break;
			case 7:
				deleteTree(root);
				deleteTree(result);
				free(root);
				free(result);
				exit(0);
				break;
			default:
				printf("Opción no válida");
				break;
		}
	}
	deleteTree(root);
	deleteTree(result);
	free(root);
	free(result);
}
