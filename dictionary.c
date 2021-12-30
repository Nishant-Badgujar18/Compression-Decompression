#include "dictionary.h"

//Create first 256 entries
void InitDictionary(dictionary *d)
{
    int i = 0;
    d->dicCount = 0;
    while (i < MAX){
        unsigned char* tmp = (unsigned char*)calloc(1, 1);
        *tmp = i;
        d->code[i].data = tmp;
        d->code[i].s = 1;
        d->dicCount++;
        i++;
    }
    i = MAX;
    while (i < MAXLZW){
        d->code[i].data = NULL;
        d->code[i].s = 0;
        i++;
    }
}

//This function will free dynamic dictionary after encoding and decoding.
void freeDic(dictionary *d)
{
	int i = 0;
	while(i < MAXLZW){
		if(d->code[i].s)
			free(d->code[i].data);
		else
			break;
		i++;
	}
}

//This function initialises the dictionary
int isInDic(bindata s, dictionary *d)
{
    int ndx = MAX;
    while ((d->code[ndx].data != NULL) && (ndx < MAXLZW)){
        if ((d->code[ndx].s == s.s) && memcmp(s.data, d->code[ndx].data, d->code[ndx].s) == 0){
            return 1;
        }
        ndx++;
    }
    return 0;
}

//This function accesses code from dictionary
int getCode(bindata s, dictionary* d)
{
    int ndx = 0;
    while ((d->code[ndx].data != NULL) && (ndx < MAXLZW)){
        if ((d->code[ndx].s == s.s) && memcmp(s.data, d->code[ndx].data, d->code[ndx].s) == 0)
            return ndx;
        ndx++;
    }
    return 0;
}

//This function adds data to dictionary.
void addToDic(bindata s, dictionary* d)
{
    if (d->dicCount >= MAXLZW)
        return;

    unsigned char *tmp = (unsigned char*)calloc(s.s, 1);
    memcpy(tmp, s.data, s.s);

    d->code[d->dicCount].data = tmp;
    d->code[d->dicCount].s = s.s;
    d->dicCount++;
}

//This function access binary data by copying from dictionary.
bindata getFromDic(dictionary* d, short ndx)
{
    bindata b = { 0 };
    copybinarydata(&b, d->code[ndx]);
    return b;
}

//This function copies binary data from dictionary
void copybinarydata(bindata *P, bindata C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(C.s, 1);
    else
        P->data = (unsigned char*)realloc(P->data, C.s);
    memcpy(P->data, C.data, C.s);
    P->s = C.s;
}

//This functiona copies data to dictionary
void copydata(bindata *P, unsigned char C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(1, 1);
    else
        P->data = (unsigned char*)realloc(P->data, 1);
    memcpy(P->data, &C, 1);
    P->s = 1;
}

//This function adds data to dictionary
void adddata(bindata *P, unsigned char C)
{
    if(P->data == NULL)
        P->data = (unsigned char*)calloc(1, 1);
    else
        P->data = (unsigned char*)realloc(P->data, P->s + 1);
    memcpy((P->data + P->s), &C, 1);
    P->s++;
}

//This function adds binary data to dictionary
void addbinarydata(bindata *P, bindata C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(C.s, 1);
    else
        P->data = (unsigned char*)realloc(P->data, P->s + C.s);
    memcpy((P->data + P->s), C.data, C.s);
    P->s = P->s + C.s;
}

//This function frees binary data.
void freebindata(bindata *P)
{
	if(P->s)
		free(P->data);
}


