//Huffman decompression

#include "common.h"
#include "tree.h"

//Read codes and freq from file
errorcode readCodes(FILE* fp, hcode codes[], int freq[], int maxsize, int* lastbitused)
{
    unsigned char character = '\0';
    fread(lastbitused, 1, 1, fp);

    while (!feof(fp)){
        //read character
        if (fread(&character, 1, 1, fp)){
            int f = 0;
            if (fread(&f, sizeof(int), 1, fp)){
            	if(character == 0 && f == 0) break;
                freq[character] = f;
            }
            else {
                fclose(fp);
                return FILECORRUPTED;
            }
        }
    }
    return SUCCESS;
}

//This function truncates file.
void truncatefile(char* file, int reducecount)
{
    //get file size first
    FILE* fp = fopen(file, "r");
    fseek(fp, 0, SEEK_END);                    // seek to end of file
    int size = ftell(fp);                      // get current file pointer
    fclose(fp);

    //get new file size after truncation
    size = size - reducecount;
    truncate(file, size);
}

//This functions decodes data and saves to output file
void decodeData(FILE* fpin, char* outfile, tree* t, hcode codes[], int maxsize, int lastusedbits)
{
    FILE* fpout = fopen(outfile, "wb+");

    node* root = t->pnodes[0];
    node* currentnode = root;
    int possibleextrabyte = 0;
    int numextrabyteswritten = 0;

    while (!feof(fpin)){
        unsigned char rtemp = 0;
        if (fread(&rtemp, 1, 1, fpin)){
            //read bit by bit and keep travesring the tree..when leaf node is found then write to file and reset current node
            possibleextrabyte = 0;
            numextrabyteswritten = 0;
            for (int i = 0; i < 8; i++){

                int value = GetBit(&rtemp, i);
                if (value != 0){
                    currentnode = currentnode->right;
                }
                else{
                    currentnode = currentnode->left;
                }
                if (currentnode->left == NULL && currentnode->right == NULL){
                    //we have found leafnode..save the character to output file and reset current node to root
                    fwrite(&currentnode->data, 1, 1, fpout);
                    if (i == (lastusedbits - 1)){
                        //if this is last byte of file then we stop
                        possibleextrabyte = 1;
                    }
                    if(possibleextrabyte)
                        numextrabyteswritten++;

                    currentnode = root;
                }
            }
        }
    }
    fclose(fpin);
    fclose(fpout);
    //remove last extra bytes (numextrabyteswritten-1)
    if(numextrabyteswritten!=0)
   		 truncatefile(outfile, numextrabyteswritten - 1);

}

//This function takes the input file and decompresses it using the huffman algorithm and saves the output file
errorcode HuffmanD(char* infile, char* outfile)
{
    errorcode err = SUCCESS;
    FILE *fp;
    fp = fopen(infile, "rb");
    if (fp == NULL) return FILENOTFOUNDERROR;
    hcode codes[MAX] = { 0 };
    int freq[MAX] = { 0 };
    int lastusedbits = 0;                             //bits used from last byte

    //read codes and freq from file
    err = readCodes(fp, codes, freq, MAX, &lastusedbits);
    if (err != SUCCESS)
        return err;

    //Build huffman tree from codes and frequency
    unsigned char character[MAX] = { 0 };
    for (int i = 0; i < MAX; i++){
        character[i] = i;
    }
    tree *t = buildHuffmanTree(character, freq, MAX);

    //save data
    decodeData(fp, outfile, t, codes, MAX, lastusedbits);

    deletetree(t);
    fclose(fp);
    printsize(outfile);

    return err;
}
