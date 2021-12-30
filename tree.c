#include "tree.h"

//initialise tree.
void inittree(tree *t)
{
	t->pnodes = NULL;
    t->capacity = t->size = 0;
}

//initialise node.
void initnode(node *n)
{
	n->data = '\0';
	n->freq = 0;
	n->left = NULL;
	n->right = NULL;
}

//check if tree is empty.
int isempty(tree *t)
{
	return (!t->size);
}

//check if tree is full .
int isfull(tree *t)
{
	return 0;
}

//creates node.
node* newnode(unsigned char c, int f)
{
	node * temp = (node *)malloc(sizeof(node));
	if(temp == NULL) return temp;
	initnode(temp);
	temp->data = c;
	temp->freq = f;
	return temp;
}

tree * maketree(unsigned char * character, int * freq, int size)
{
    tree* t = (tree*)malloc(sizeof(tree));
    t->size = 0;
    t->capacity = size;

	int ndx = 0;
	t->pnodes = (node **)malloc(size * sizeof(node*));
	for(int ndx = 0; ndx < size; ndx++){
		//for each character with non zero freuency, we create node
		if(freq[ndx] !=0){
			node* tmp = newnode(character[ndx], freq[ndx]);
            t->pnodes[t->size] = tmp;
            t->size++;
		}
	}
	return t;
}

//delete a node.
void deletenode(node* n)
{
    if (n == NULL){
        return;
    }
    if (n->left != NULL){
        deletenode(n->left);
        n->left = NULL;
    }
    if (n->right != NULL){
        deletenode(n->right);
        n->right = NULL;
    }
    free(n);
}

//delete minheap tree along will huffman tree.
void deletetree(tree* t)
{
    int i = 0;
    while (i < t->size){
        deletenode(t->pnodes[i]);
        i++;
    }
    free(t->pnodes);
    free(t);
}

//get minimum frequency node from minheap and remove it from min heap.
node* getmin(tree* t)
{
    sort(t, 0);
    node* temp = t->pnodes[0];
    t->pnodes[0] = t->pnodes[t->size-1];
    --(t->size);
    return temp;
}

//sort the min heap such that minimum freq. node is at 0 position.
void sort(tree* t, int i)
{
    int min = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < t->size && t->pnodes[left]->freq < t->pnodes[min]->freq)
        min = left;

    if (right < t->size && t->pnodes[right]->freq < t->pnodes[min]->freq)
        min = right;

    if (min != i){
        swap(&t->pnodes[min], &t->pnodes[i]);
        sort(t, min);
    }
}

//swap the 2 nodes
void swap(node** a, node** b)
{
    node* t = *a;
    *a = *b;
    *b = t;
}

//Insert node into minheap in incresing order
void insertnode(tree* t, node* n)
{
    ++(t->size);
    int i = t->size - 1;
    while (i && n->freq < t->pnodes[(i - 1) / 2]->freq){
        t->pnodes[i] = t->pnodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    t->pnodes[i] = n;
}

//This function will build huffman tree for encoding
tree* buildHuffmanTree(unsigned char * character, int * freq, int size)
{
    tree* t = maketree(character, freq, size);
    node* left, *right, *top;

    while (t->size != 1){
        left = getmin(t);
        right = getmin(t);
        top = newnode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertnode(t, top);
    }

    return t;
}
