#ifndef HACKW_HACKW_H
#define HACKW_HACKW_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define tran(X) (int)(strtod(argv[X], NULL)*100)
#define sam(X,Y) strcmp(argv[X],Y) == 0
#ifdef _WIN32
#define PRINT_DIR(X,Y) printf("\033[32msuccessfully written to %s\\output.%s \033[0m\n",X,Y);
#else
#define PRINT_DIR(X,Y) printf("\033[32msuccessfully written to %s/output.%s \033[0m\n",X,Y);
#endif

/** data struct
 * 1,2,3,4,5,6,7,9,10,11,12,13,14,15 blocks are blank
 * 0 need to write identify data;
 * 8 need to write water card data
 */
typedef struct {
    unsigned char data[16];
}BLOCK;
typedef struct {
    BLOCK blocks[3];
    BLOCK key;
}AREA;
extern const AREA blank;

void printHelp();
int allHex(const char *str);
int allNum(const char *str);
int countNum(const char *str,int *num);
int hexToB(const char *hexString, unsigned char *byteArray);
AREA calculate0(const unsigned char *UID,int cardType);
AREA calculate8(const unsigned char *UID,int num,int ifHalf,unsigned char times) ;

#endif