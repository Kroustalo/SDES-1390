//Dimitrios Sideris AM: 1390 S-DES
#include <stdio.h>
#include <stdlib.h>

/*
• Είσοδος:   00010101
• Κλειδί:  0101101000
• Έξοδος:    11001111
*/

//Prototypes

static inline int bin(int bits, int shift); //aux function for packing shi(f)t commands
void print_bin(int n, int bits); //aux print in bin
int P10_perm(int key); // function to calclulate P10
void Split_10(int n, int* split); //Spilt 10 bit key to two 5 bit ones in half
void LS_1(int* split); //Shifts the 5bit keys array once to the left (with loop)
int K_perm(int split[2]); //Create K1 key

int IP_perm(int message); // function to calclulate IP
void Split_8(int n, int* split); //Spilt 8 bit key to two 4 bit ones in half
int ER_perm(int right); //function to calclulate ER perm

int grid(int S[2], int array[4][4]); //extract elements from array

int main(void) {

    int  key, output, k1, k2;
    int split[2];
    

    int message, ER,XOROUT;
    int LR[2];

    int s0[4][4] = { {1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2} };
    int s1[4][4] = { {0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3} };

    int S[2]; //for s0 and s1 respectivly
    int parts[2]; //returns from s0 and s1


    message = 0b11110011;
    key = 0b1010000010; //642
    // 0b1000001100 524

    //OG
    printf("10-bit key: %d\n", key);
    print_bin(key, 10);
    
    //P10
    key = P10_perm(key);

    if (key == -1) { //saftey
        return 1;
    }

    
    printf("\nP10: %d\n", key);
    print_bin(key, 10);
    

    //SPLIT 10
    Split_10(key, split);
     printf("\nsplit1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("split2: %d\n", split[1]);
    print_bin(split[1], 5);
    

    //K1
    LS_1(split);
    printf("\nls1 shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("ls1 shift2: %d\n", split[1]);
    print_bin(split[1], 5);

    k1 = K_perm(split);
    printf("\nK1: %d\n", k1);
    print_bin(k1, 8);

    //K2
    LS_1(split);
    LS_1(split);
    printf("\nls2 shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("ls2 shift2: %d\n", split[1]);
    print_bin(split[1], 5);

    k2 = K_perm(split);
    printf("\nK2: %d\n", k2);
    print_bin(k2, 8);

    //message
    printf("\n8bit message: %d\n", message);
    print_bin(message, 8);

    //ip
    message = IP_perm(message);

    if (message == -1) { //saftey
        return 1;
    }

    printf("\nIP: %d\n", message);
    print_bin(message, 8);

    //SPLIT 8
    Split_8(message, LR);
    printf("\nLEFT: %d\n", LR[0]);
    print_bin(LR[0], 4);
    printf("RIGHT: %d\n", LR[1]);
    print_bin(LR[1], 4);

    //ER
    ER = ER_perm(LR[1]);
    if (ER == -1) { //saftey
            return 1;
    }

    printf("\nER: %d\n", ER);
    print_bin(ER, 8);

    //XOR OUT
    XOROUT = ER ^k1;
    printf("\nER XOR k1 out: %d\n", XOROUT);
    print_bin(XOROUT, 8);



    //SPLIT 8 #2
    Split_8(XOROUT, S);
    printf("\nS0  PART: %d\n", S[0]);
    print_bin(S[0], 4);
    printf("S1 PART: %d\n", S[1]);
    print_bin(S[1], 4);


    //GRID
    parts[0] = grid(S[0], s0);
    parts[1] = grid(S[1], s1);

    printf("\nS0  return: %d\n", parts[0]);
    print_bin(parts[0], 2);
    printf("S1 return: %d\n", parts[1]);
    print_bin(parts[1], 2);

    return 0;

}

//aux print in bin
void print_bin(int n, int bits) {
    unsigned i;
    for (i = 1 << bits - 1; i > 0; i = i / 2)
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

    int temp = 0, mafs;
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

int K_perm(int split[2]) { //Create K keys with specific input
    int temp = (0b0000000000 | split[0]) << 5;
    temp = temp + split[1]; //combine to extract 8 lower bits

    //print_bin(temp, 10); //test
    temp = temp & 0b11111111; // get 8bit number
    //print_bin(temp, 8); //test

    int k = 0, mafs;
    int P8_GUIDE[8] = { 6,4,2,7,5,3,0,1 }; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        mafs = bin(8, i);
        k += (temp & mafs) / mafs * 1 << P8_GUIDE[i];
    }

    return k;
}

// function to calclulate IP
int IP_perm(int message){
    if (message > 255) {
        printf("value is greater than 8 bits");
        return -1;
    }

    int temp = 0, mafs;
    int IP_GUIDE[8] = { 4,7,5,3,1,6,0,2 }; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        mafs = bin(8, i);
        temp += (message & mafs) / mafs * 1 << IP_GUIDE[i];
    }
    return temp;

}

//Spilt 8 bit key to two 4 bit ones in half
void Split_8(int n, int* split) {
    *split = (0b11110000 & n) >> 4;
    split++;
    *split = 0b00001111 & n;
}

//function to calclulate ER perm
int ER_perm(int right) {
    if (right > 15) {
        printf("value is greater than 4 bits");
        return -1;
    }

    int temp = 0, mafs;

    //higher part 
    int ER_GUIDE1[4] = { 2, 1, 0, 3 }; //Shift positions according to pdf
    for (int i = 0; i < 4; i++) {
        mafs = bin(4, i);
        temp += (right & mafs) / mafs * 1 << ER_GUIDE1[i];
    }

    temp = temp << 4; 

    //lower part
    int ER_GUIDE2[4] = { 0,3,2,1 }; //Shift positions according to pdf
    for (int i = 0; i < 4; i++) {
        mafs = bin(4, i);
        temp += (right & mafs) / mafs * 1 << ER_GUIDE2[i];
    }

    return temp;

}

int grid(int S, int array[4][4]) { //extract elements from array

    int row, col;
    row = ((0b1000 & S) >> 2) + (0b0001 & S); //first and fourth
    col = (0b0110 & S) >> 1;//secibd and third

    return array[row][col];

}
