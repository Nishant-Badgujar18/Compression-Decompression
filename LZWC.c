//LZW compression
#include "common.h"
#include "dictionary.h"

//This function access lsb byte from code.
unsigned char getlsbbyte(short code)
{
    short tmp = code & 0x00FF;
    return (unsigned char)tmp;
}

//This function access msb byte from code.
unsigned char getmsbbyte(short code)
{
    short tmp = code & 0x0FF0;
    tmp = tmp >> 4;
    return (unsigned char)tmp;
}

//This function access msb nibble from code.
unsigned char getmsbnibble(short code)
{
    short tmp = code & 0x0F00;
    tmp = tmp >> 8;
    return (unsigned char)tmp;
}

//This function access lsb nibble from code.
unsigned char getlsbnibble(short code)
{
    short tmp = code & 0x000F;
    return (unsigned char)tmp;
}

//This function creates bytes from nibble.
unsigned char createbytefromnibble(unsigned char highnibble, unsigned char lownibble)
{
    short tmp = highnibble;
    tmp = tmp << 4;
    tmp = tmp | lownibble;
    return (unsigned char)tmp;
}

//This function writes byte data from code.
void writebytedata(FILE* fpout, short code, int finish)
{
    static int remainingbits = 0;
    static unsigned char remainingdata = 0;  //if some data is left out from previous op, then it would always be lsb nibble

    //we write MSB first and then LSB
    unsigned char c1 = getmsbbyte(code);
    unsigned char c2 = getlsbnibble(code);

    if (finish == 1){
        //write remaining data if any
        if (remainingbits){
            unsigned char tmp = createbytefromnibble(getlsbnibble(remainingdata), getmsbnibble(code));
            fwrite(&tmp, 1, 1, fpout);
        }
        return;
    }
    if (remainingbits == 0){
        //write lsb as it is
        fwrite(&c1, 1, 1, fpout);
        remainingdata = c2;
        remainingbits = 4;
    }
    else {
        unsigned char tmp = createbytefromnibble(getlsbnibble(remainingdata), getmsbnibble(code));
        fwrite(&tmp, 1, 1, fpout);
        tmp = getlsbbyte(code);
        fwrite(&tmp, 1, 1, fpout);
        remainingdata = 0;
        remainingbits = 0;
    }

}

//This function encodes data from file.
void encodeLZWData(FILE* fpin, FILE* fpout, dictionary *d)
{
    unsigned char C = '\0';
    bindata P = { 0 };

    //read data from file and create table dynamically
    if (fread(&C, 1, 1, fpin)){
        adddata(&P, C);
        while (!feof(fpin)){
            if (fread(&C, 1, 1, fpin)){
                bindata temp = { 0 };
                addbinarydata(&temp, P);
                adddata(&temp, C);
                if (isInDic(temp, d)){
                    copybinarydata(&P, temp);
                }
                else {
                   // output the code for P
                    short code = getCode(P, d);
                    //write 12 bits of code
                    writebytedata(fpout, code, 0);
                    addToDic(temp, d);
                    copydata(&P, C);
                }
		         //free data
		        freebindata(&temp);
            }
        }
        int code = getCode(P, d);
        writebytedata(fpout, code, 0);
        writebytedata(fpout, 0, 1);               //write remaining data if any
    }
    freebindata(&P);
}

//This function takes the input file and compresses it using the lzw algorithm and saves the output file
errorcode LZWC(char* infile, char* outfile)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    dictionary d1;
    FILE *fpin, *fpout;
    fpin = fopen(infile, "rb");
    if (fpin == NULL) return FILENOTFOUNDERROR;

    fpout = fopen(outfile, "wb");
    if (fpout == NULL){
        fclose(fpin);
        return FILEWRITEFAILED;
    }
    //Init dictionary
    InitDictionary(&d1);

    //Build LZW code table
    encodeLZWData(fpin, fpout, &d1);

	freeDic(&d1);
    fclose(fpin);
    fclose(fpout);

	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
	printf("Time elpased is %ld seconds and %ld micros\n", seconds, micros);

    float ratio = getratio(infile, outfile);
    printf("Compression Ratio using LZW Compression Algorithm is : %f\n", ratio);

    return SUCCESS;
}
