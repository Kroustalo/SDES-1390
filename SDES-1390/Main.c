//Dimitrios Sideris AM: 1390 S-DES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



/*
• Είσοδος:   00010101
• Κλειδί:  0101101000
• Έξοδος:    11001111
*/


//Prototypes
int P10_perm(int key); // function to calclulate P10
void Split_10(int n, int* split); //Spilt 10 bit key to two 5 bit ones in half
static inline int bin(int bits, int shift); //aux function for packing shi(f)t commands
void print_bin(int n, int bits);//print in bin
void LS_1(int* split); //Shifts the 5bit keys array once to the left (with loop)
int K(int split[2]); //Create K1 key

int main(void) {

    int input, key, output,k1,k2;
    int split[2];

    input = 0b00010101;
    key = 0b1010000010; //642
          // 0b1000001100 524
   
    //OG
    printf("10-bit key: %d\n", key);
    print_bin(key, 10);
    key=P10_perm(key);

    if (key == -1) { //saftey
        return 1;
    }

    //P10
    printf("P10: %d\n", key);
    print_bin(key, 10);

    //SPLIT
    Split_10(key, split);
    printf("split1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("split2: %d\n", split[1]);
    print_bin(split[1], 5);


    //K1
    LS_1(split);
    printf("shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("shift2: %d\n", split[1]);
    print_bin(split[1], 5);

    k1=K(split);
    printf("K1: %d\n", k1);
    print_bin(k1, 8);



    //K2
    LS_1(split);
    LS_1(split);
    printf("shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("shift2: %d\n", split[1]);
    print_bin(split[1], 5);

    k2 = K(split);
    printf("K2: %d\n", k2);
    print_bin(k2, 8);

    return 0;

}

//print in bin
void print_bin(int n,int bits){
    unsigned i;
    for (i = 1 << bits-1; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
    printf("\n");
}

//aux function for packing shi(f)t commands
static inline int bin(int bits, int shift) {
    return (1 << bits - 1) >> shift;
}


//function to calclulate P10
int P10_perm(int key) {
    if (key > 1023) {
        printf("value is greater than 10 bits");
        return -1;
    }

    int temp = 0,mafs;
    int P10_GUIDE[10] = { 3,7,9,5,8,0,6,1,2,4 }; //Shift positions according to pdf
    for (int i = 0; i < 10; i++) {
        mafs = bin(10, i);
        temp += (key & mafs) / mafs * 1 << P10_GUIDE[i];
    }
    return temp;

}

//Spilt 10 bit key to two 5 bit ones in half
void Split_10(int n, int* split) { //Spilt 10 bit key to two 5 bit ones in half
    *split = (0b1111100000 & n) >> 5;
    split++;
    *split = 0b0000011111 & n;
}

void LS_1(int* split) { //Shifts the 5bit keys array once to the left (with loop)
    int temp;
    for (int i = 0; i < 2; i++) {
        temp = *split << 1;
        if (temp >= 32) {
            temp = (0b011110 & temp) + 1; //keep 1-4 bits and add 1
        }
        *split = temp;
        split++;
    }

}

int K(int split[2]) { //Create K1 key
    int temp = (0b0000000000 | split[0]) << 5;
    temp = temp + split[1]; //combine to extract 8 lower bits

    //print_bin(temp, 10);

    temp = temp & 0b11111111; // get 8bit number
    //print_bin(temp, 8);

    int k = 0, mafs;
    int P8_GUIDE[8] = { 6,4,2,7,5,3,0,1}; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        mafs = bin(8, i);
        k += (temp & mafs) / mafs * 1 << P8_GUIDE[i];
    }

    return k;
}