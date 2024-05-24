#include "hackw.h"

int main(int argc,char *argv[]){
    srand((unsigned)(time(NULL)/2+ time(NULL)*3));
    unsigned char UID[4]={rand()%256,(rand()*2)%256,(rand()*3)%256,(rand()/3)%256};
    AREA icdump[16],area0,area8;
    for (int i = 0; i < 16; ++i) memcpy(&icdump[i],&blank, sizeof(AREA));
    /**
     * command:
     * 0-> no option,change it to 2
     * X = b?1+d?2+h?4 (1-7)
     *
     * state:
     * 0 : didn't read price yet,
     * 1 : have already read price,
     * 2 : input format error
     * 3 : input price too big
     * 4 : program error
     */
    int command = 0, price = 5000, state=0,type=1;
    for (int i = 1; i < argc; ++i) {
        int temp = countNum(argv[i],&command);
        if(temp == 0) {
            if (allNum(argv[i])){
                temp = tran(i);
                if(temp<16384&&temp>0){state = 1;price = temp;}
                else {state=3;break;}
            }else if (strcmp(argv[i],"FULL")==0){
                price = 16383;state = 1;
            }else {state=2;break;}
        }else if (temp == 1 && state==0){
        }else if (temp == 2 && state==1){
            i++;
            if (sam(i,"UID")) type=0;
            else if (sam(i,"CUID")||sam(i,"FUID")||sam(i,"UFUID"));
            else {state=2;break;}
        }else if (temp == 3 && state==1 && allHex(argv[++i])){
            if (hexToB(argv[i],UID));
            else {state=4;break;}
        }else if (temp == 4){
            printHelp();
            return 0;
        } else {state=2;break;}
    }
    if (command==0||command==4) command += 2;
    /* debugger
    printf("%d\t%d\t%d\t%d\t",command,price,state,type);
    for (int i = 0; i < 4; ++i) {
        printf("%02x",UID[i]);
    }
    */
    if(state==1) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[32mProcessing...\033[0m\n");
        area0=calculate0(UID,type);
        area8= calculate8(UID,price,command/4,01);
        memcpy(&icdump[0], &area0, sizeof(AREA));
        memcpy(&icdump[8], &area8, sizeof(AREA));
        if(command%2){//bin
            FILE *fbin= fopen("output.bin","wb");
            if(fwrite(icdump, sizeof(AREA),16,fbin)!=16) printf("\033[31mError: Failed to write data\033[0m\n");
            else PRINT_DIR(cwd,"bin")
            fclose(fbin);
        }
        if((command%4)/2){
            FILE *fdump= fopen("output.dump","wb");
            if(fwrite(icdump, sizeof(AREA),16,fdump)!=16) printf("\033[31mError: Failed to write data\033[0m\n");
            else PRINT_DIR(cwd,"dump")
            fclose(fdump);
        }
    }else if(state==0) printf("\033[33mWarning: You must provide a lawful price\033[0m\n");
    else if(state==2) {
        printf("\033[31mError: representation error.\033[0m\n");
        printHelp();
    }else if(state==3) printf("\033[31mError: it's required that 0 < PRICE < 163.84\033[0m, you can try \"FULL\"\n");
    else printf("\033[31mError: program fault.\033[0m\n");
    return 1;
}