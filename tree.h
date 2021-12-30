#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

typedef struct node{
    unsigned char data;  //char data.
    int freq;  //frequency of each charactor.
    struct node *left;
    struct node *right;
}node;

typedef struct tree{
    int size;  //current size of tree (number of nodes)
    int capacity;  //allocated size of array
    node** pnodes;
}tree;

#define MAXCODE 32  //256/8 will be max code size for 1 unsigned char

typedef struct hcode{
    unsigned char carr[MAXCODE];  //array of chars to store code
    int  size;  //size of code in bits
}hcode;

//This functions related to ADT tree.
void inittree(tree *t);
int isempty(tree *t);
int isfull(tree *t);
void branch(tree *t, int a, int b);
void initnode(node *n);
int remnode(tree *t);
node* newnode(unsigned char c, int f);

tree * maketree(unsigned char * character, int * freq, int size);
void deletetree(tree* t);
node* getmin(tree* t);
void swap(node** a, node** b);
void sort(tree* t, int i);
void insertnode(tree* t, node* n);
tree* buildHuffmanTree(unsigned char * character, int * freq, int size);
int GetBit(unsigned char *c, int nbit);
