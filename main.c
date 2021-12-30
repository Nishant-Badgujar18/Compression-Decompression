/*
Name : Nishant Suresh Badgujar
MIS : 111903053
Div : 1
*/

//infix evaluation

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

//This function checks if given file exist or not
int checkFile(char* filepath)
{
    if (access(filepath, 0) != -1){
        // file exists
        return 0;
    }
    else{
        // file doesn't exist
        return 1;
    }
}

int main(int argc, char * argv[])
{
    errorcode ret = SUCCESS;

    if (argc != 4)
        return onError(INPUTERROR, argv[0]);

    if ((strcmp(argv[1], "-c1") != 0) && (strcmp(argv[1], "-c2") != 0) && (strcmp(argv[1], "-uc1") != 0) && (strcmp(argv[1], "-uc2") != 0))
        return onError(INPUTERROR, argv[2]);

    if (checkFile(argv[2]) != 0)
        return onError(FILENOTFOUNDERROR, argv[2]);

    if (strcmp(argv[1], "-c1") == 0){
        ret = HuffmanC(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-c2") == 0){
        ret = LZWC(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-uc1") == 0){
        ret = HuffmanD(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-uc2") == 0){
        ret = LZWD(argv[2], argv[3]);
    }

    if (!ret)
        return onError(ret, argv[2]);

    return ret;
}
