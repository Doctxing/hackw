#include <stdio.h>

int hex_char_to_int(const char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;  // when error
}

int string_to_char_array(const char* str, char* result) {
    for (int i = 0; i < 4; i++) {
        int high = hex_char_to_int(str[i*2]);
        const int low = hex_char_to_int(str[i*2+1]);
        if (high == -1 || low == -1) {
            return -1;
        }
        high = high << 4;
        result[i] = high | low;
    }
    return 0;
}

int main(int argc,char *argv[]) {
    unsigned char UID[4];
    int result = string_to_char_array(argv[1],UID);
    if (result == -1) {
        printf("Error\n");
        return -1;
    }
    const unsigned char A = UID[0]-0x3d;
    const unsigned char B = UID[1]-0x0f;
    const unsigned char C = UID[2]|0x39;
    const unsigned char D = (UID[3]-0xdb)^0x29;
    const unsigned char E = (UID[0]+0x11)^0x65;
    const unsigned char F = (UID[3]+0x30)&0x77;
    const unsigned char keys[6]={A,B,C,D,E,F};
    printf("keys: ");
    for (int i = 0; i < 6; ++i) {
        printf("%02X",keys[i]);
    }
    printf("\n");
    return 0;
}