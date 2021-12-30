//LZW decompression
#include "dictionary.h"
#include "common.h"

//This function access msb nibble as short value.
short getmsbnibbleasshort(unsigned char c)
{
    short t = c;
    t = t & 0xF0;
    return t;
}

//This function access lsb nibble as charactor value.
unsigned char getlsbnibbleaschar(unsigned char c)
{
    short t = c;
    t = t & 0x0F;
    return (unsigned char)t;
}

//This function makes code from higher bytes
short makecode(unsigned char highbyte, unsigned char lowbyte)
{
    short code = highbyte & 0xFF;
    short lowbytecode = lowbyte & 0xFF;

    code = code << 4;
    code = code | lowbytecode;
    return code;
}

//This function access LZW code from input file.
short getLZWCode(FILE* fpin)
{
    short code = -1;
    static short remainingbits = 0;
    static unsigned char remainingdata = 1;  //if some data is left out from previous op, then it would always be msb nibble
    unsigned char c1, c2, c;

    if (feof(fpin))
        return code;

    //read first 1 or 2 bytes depending upon remainingbits
    if (remainingbits){
        if(fread(&c1, 1, 1, fpin))
        code = makecode(remainingdata<<4, c1);
        remainingdata = 0;
        remainingbits = 0;
    }
    else
    if (fread(&c1, 1, 1, fpin)){
        if (feof(fpin))
            return code;
        if (fread(&c2, 1, 1, fpin)){
            short c3 = getmsbnibbleasshort(c2);
            code = makecode(c1, c3 >> 4);
            remainingbits = 4;
            remainingdata = getlsbnibbleaschar(c2);
        }
    }

    return code;
}

//This function decodes LZW data from file.
void decodeLZWData(FILE* fpin, FILE* fpout, dictionary *d)
{
    unsigned char c1, c2, c;
    short new = 0;

    short old = getLZWCode(fpin);
    //write code's translation
    bindata tmp = getFromDic(d, old);
    bindata C = { 0 };
    bindata S = { 0 };

    fwrite(tmp.data, 1, tmp.s, fpout);
	copydata(&C, tmp.data[0]);
    while ((new = getLZWCode(fpin)) != -1)
    {
        if (d->code[new].s == 0){
            S = getFromDic(d, old);
            addbinarydata(&S, C);
            //fwrite(S.data, 1, S.s, fpout);
        }
        else
			S = getFromDic(d, new);
		fwrite(S.data, 1, S.s, fpout);
		copydata(&C, S.data[0]);
		tmp = getFromDic(d, old);
		addbinarydata(&tmp, C);
		addToDic(tmp, d);
		old = new;

    }
}

//This function takes the input file and decompresses it usingthe lzw algorithm and saves the output file
errorcode LZWD(char* infile, char* outfile)
{
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
    decodeLZWData(fpin, fpout, &d1);

    //save data
    //encodeData(infile, outfile, d1);
	freeDic(&d1);
    fclose(fpin);
    fclose(fpout);
    printsize(outfile);
    return SUCCESS;
}
