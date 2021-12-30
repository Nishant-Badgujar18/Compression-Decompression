//Huffman compression

#include "common.h"
#include "tree.h"

void SaveBit(unsigned char *c, int nbit, int value)
{
    //Create bitmask for bit position
    unsigned char mask = 0x80;
    for (int i = 0; i < nbit; i++){
        mask = mask >> 1;                //right shift
    }
    if (value){
        *c = *c | mask;
    }
    else{
        *c = *c & ~mask;
    }
}


int GetBit(unsigned char *c, int nbit)
{
    int value = 0;
    //Create bitmask for bit position
    unsigned char mask = 0x80;
    for (int i = 0; i < nbit; i++){
        mask = mask >> 1; //right shift
    }
    value = *c & mask;
    return value;
}

void getCodeArr(int arr[], hcode* codes, int n)
{
    int i;
    for (i = 0; i < n; ++i){
        int ndx = i / (sizeof(char) * 8);        //index of carr in which bit has to be saved
        int nbit = i % (sizeof(char) * 8);       //bit position

        SaveBit(&(codes->carr[ndx]), nbit, arr[i]);
        codes->size++;
    }
}

void getCodes(node* root, int arr[], hcode codes[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left){
        arr[top] = 0;
        getCodes(root->left, arr, codes, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right){
        arr[top] = 1;
        getCodes(root->right, arr, codes, top + 1);
    }

    if (root->left == root->right){
        getCodeArr(arr, &codes[root->data], top);
    }
}

void savelastbytebits(FILE* fp, unsigned char value)
{

    fwrite(&value, 1, 1, fp);
}

errorcode saveCodes(char* outfile, hcode codes[], int freq[], int maxsize)
{
    FILE* fp = fopen(outfile, "wb");
    int codelen = 0;
    if (fp == NULL)
        return FILEWRITEFAILED;
    //save 0 as used bits of last byte
    savelastbytebits(fp, 8);

    for (int i = 0; i < maxsize; i++){
        if (codes[i].size != 0){
            fwrite(&i, 1, 1, fp);
            fwrite(&(freq[i]), sizeof(int), 1, fp);
        }
    }
    //write null character to indicate end of codes
    fwrite("", 1, 1, fp);
    int i = 0;
    fwrite(&i, sizeof(int), 1, fp);
    fclose(fp);
    return SUCCESS;
}

//This functions encodes data and saves to output file
void encodeData(char* infile, char* outfile, hcode codes[], int maxsize)
{
    FILE* fpin = fopen(infile, "rb");
    FILE* fpout = fopen(outfile, "ab+");

    int wbitcount = 0;
    unsigned char wtemp = 0;

    while (!feof(fpin)){
        unsigned char rtemp = 0;
        if (fread(&rtemp, 1, 1, fpin)){
            //read bit by bit and keep saving in temp..when temp reaches max size(8 bits)..then write to file and reset temp
            int bytecount = 0;
            int rbitcount = 0;
            for (int i = 0; i < codes[rtemp].size; i++){
                int value = GetBit(&(codes[rtemp].carr[bytecount]), rbitcount++);
                if (rbitcount == 8){
                    rbitcount = 0;
                    bytecount++;
                }
                SaveBit(&wtemp, wbitcount++, value);
                if (wbitcount == 8){
                    wbitcount = 0;
                    fwrite(&wtemp, 1, 1, fpout);
                }
            }
        }
    }
    if (wbitcount != 0){
        fwrite(&wtemp, 1, 1, fpout);
    }

    fclose(fpin);
    fclose(fpout);

    //save used bits of last byte to file
    FILE* fp = fopen(outfile, "rb+");
    if (fp != NULL){
            savelastbytebits(fp, wbitcount);
        fclose(fp);
    }
}

//This function takes the input file and compresses it using the huffman algorithm and saves the output file
errorcode HuffmanC(char* infile, char* outfile)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

	FILE *fp;
	fp=fopen(infile,"rb");
	if(fp == NULL) return FILENOTFOUNDERROR;
	unsigned char character[MAX] = {0};
	int freq[MAX] = {0};
	unsigned char temp = '\0';
    while (!feof(fp)){
        if (fread(&temp, 1, 1, fp)){
            character[temp] = temp;
            freq[temp]++;
        }
	}
	//Build Huffman tree
    tree* t = buildHuffmanTree(character, freq, MAX);

    //Get huffman codes in memory
    int arr[MAXCODE] = {0}, top = 0;
    hcode codes[MAX] = {0};
    getCodes(t->pnodes[0], arr, codes, top);

    //Save codes in file
    saveCodes(outfile, codes, freq, MAX);

    //save data
    encodeData(infile, outfile, codes, MAX);

    deletetree(t);

    fclose(fp);

    gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
	printf("Time elpased is %ld seconds and %ld micros\n", seconds, micros);

    float ratio = getratio(infile, outfile);
    printf("Compression Ratio using Huffman Compression Algorithm is : %f\n", ratio);
	return SUCCESS;
}


