//error handling file

#include "common.h"

//This function prints usage of the 'program'.
void printUsage()
{
    printf("Usage : \nprogram -c1 <file> <compressed-file-name>\n");
    printf("program -c2 <file> <compressed-file-name>\n");
    printf("program -uc1 <compressed-file-name> <uncompressed-file-name>\n");
    printf("program -uc2 <compressed-file-name> <uncompressed-file-name>\n");
    printf("Where :\n-c1 and -uc1 options are for Huffman compression technique\n");
    printf("-c2 and -uc2 options are for LZW compression technique\n");
}

//This function prints error message for different error codes
int onError(errorcode error, char* filename)
{
    switch (error)
    {
    case INPUTERROR:
        printf("Error : Invalid input parameters.\n");
        printUsage();
        break;
    case FILENOTFOUNDERROR:
        printf("Error : %s - No such file found.\n", filename);
        printUsage();
        break;
    case DISKFULLERROR:
        printf("Error : No space left on device. Free up space by deleting some files and then retry.\n");
        break;
    case FILECORRUPTED:
        printf("Error : Input file is corrupted.\n");
        break;
    case SUCCESS:
        break;
    default:
        printf("Error : Error occured.\n");
        break;
    }

    return 1;
}
