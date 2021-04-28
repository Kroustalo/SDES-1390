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
void Split(int n, int bits, int* split); //spilt custom

int P10_perm(int key); // function to calclulate P10
void LS_1(int* split); //Shifts the 5bit keys array once to the left (with loop)
int K_perm(int split[2]); //Create K1 key

int IP_perm(int message); // function to calclulate IP
int EP_perm(int right); //function to calclulate EP perm

int grid(int S, int array[4][4]); //extract elements from array
int P4_perm(int parts[2]); //P4 perm after array

int IP_REV_perm(int message); // function to calclulate REVERSED IP PERM

int init(int key, int* k1, int* k2); //Gets k1 and k2 from key
int process(int LR[2], int k, int* output); //process function
int encrypt(int message, int k1, int k2);  //encrypt
int decrypt(int message, int k1, int k2);  //decrypt

int hack(int message, int encrypted); //bruteforce all keys to find the right ones


int main(void) {

    int option =3; //encrypt 1 | decrypt 2 | hack 3
/*
    printf("1) Encrypt\n2) Decrypt\n3)Hack\nSelect Option:");
    scanf("%d", &option);
    while (option != 1 && option != 2 && option != 3) {
        printf("Error: Not Vaild Option, select again:\n1) Encrypt\n2) Decrypt\n3)Hack\nSelect Option:");
        scanf("%d", &option);
    }
    */
    int  key, message, output, k1,k2, encrypted;
    /*
    char name[11];
    printf("Enter name: ");
    gets(name);
    printf("Your name is %s\n", name);
    */

    message = 0b11110011;
    key = 0b1010000010; //642
    // 0b1000001100 524

    message = 0b00010101;
    key = 0b0101101000; //360
   
    message = 0b00000000;
    key = 0b1111111111;

    message = 200;
    key = 550;

    encrypted = 131;
    
    //message
    printf("8bit message:\nDecimal: %d\nBinary: ", message);
    print_bin(message, 8);

    //OG key
    printf("\n10-bit key:\nDecimal: %d\nBinary: ", key);
    print_bin(key, 10);
   
    if (init(key,&k1,&k2) == -1) { //saftey
        return 1;
    }
/*---------------------------------*/
    if (option == 1) {
        output = encrypt(message, k1, k2);
        if (output == -1) { //saftey
            return 1;
        }
        printf("\nEncoded Message:\nDecimal: %d\nBinary: ", output);
        print_bin(output, 8);
    }
/*---------------------------------*/
/*---------------------------------*/
    else if (option == 2) {
        output = decrypt(message, k1, k2);
        if (output == -1) { //saftey
            return 1;
        }
        printf("\nDecoded:\nDecimal %d\nBinary: ", output);
        print_bin(output, 8);
    }
/*---------------------------------*/
    else {
        printf("\nI'm about to hack Your Mom!\n");
        printf("\nEncoded Message:\nDecimal: %d\nBinary: ", encrypted);
        print_bin(encrypted, 8);
        
        int check = hack(message, encrypted);
        if (check == -1) //saftey
            return 1;
        else if(check == 0)
            printf("\nI Hacked Your Mom!\n");
        else if(check == 1)
            printf("Your mom is so big, that's shes unhackable\n");
    }

    return 0;

}

//aux print in bin
void print_bin(int n, int bits) {
    unsigned i;
    for (i = (1 << (bits - 1)); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
    printf("\n");
}

//aux function for packing shi(f)t commands
static inline int bin(int bits, int shift) {
    return (1 << (bits - 1)) >> shift;
}

//spilt custom in half (even bit)
void Split(int n, int bits, int* split) {
    *split = n >> (bits / 2); //upper part 
    split++;
    *split = ((1 << bits / 2) - 1) & n; //lower part
}

//function to calclulate P10
int P10_perm(int key) {
    if (key > 1023) {
        printf("value is greater than 10 bits");
        return -1;
    }

    int temp = 0;
    int P10_GUIDE[10] = { 4,1,-1,3,-4,3,-1,2,-1,-6 }; //Shift positions according to pdf
    for (int i = 0; i < 10; i++) {
        if (P10_GUIDE[i] >= 0)
            temp += (key & (1 << i)) << P10_GUIDE[i];
        else
            temp += (key & (1 << i)) >> -P10_GUIDE[i];
    }
    return temp;

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

    int k = 0;
    int P8_GUIDE[8] = { 1,-1,1,2,3,-3,-2,-1}; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        if (P8_GUIDE[i] >= 0)
            k += (temp & (1 << i)) << P8_GUIDE[i];
        else
            k += (temp & (1 << i)) >> -P8_GUIDE[i];
    }

    return k;
}

// function to calclulate IP
int IP_perm(int message){
    if (message > 255) {
        printf("value is greater than 8 bits");
        return -1;
    }

    int temp = 0;
    int IP_GUIDE[8] = { 2,-1,4,-2,-1,0,1,-3}; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        if (IP_GUIDE[i] >= 0)
            temp += (message & (1 << i)) << IP_GUIDE[i];
        else
            temp += (message & (1 << i)) >> -IP_GUIDE[i];
    }
    return temp;

}

//function to calclulate EP perm
int EP_perm(int right) {
    if (right > 15) {
        printf("value is greater than 4 bits");
        return -1;
    }

    int temp = 0;

    //lower 
    int EP_GUIDE[4] = { 3,-1,-1,-1 }; //Shift positions according to pdf
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 4; i++) {
            if (EP_GUIDE[i] >= 0)
                temp += (right & (1 << i)) << EP_GUIDE[i];
            else
                temp += (right & (1 << i)) >> -EP_GUIDE[i];
        }
        if (j == 0) {//for lower part
            EP_GUIDE[0] = 1;
            EP_GUIDE[1] = 1;
            EP_GUIDE[2] = 1;
            EP_GUIDE[3] = -3;

            temp = temp << 4;
        }
    }


    return temp;

}

int grid(int S, int array[4][4]) { //extract elements from array

    int row, col;
    row = ((0b1000 & S) >> 2) + (0b0001 & S); //first and fourth
    col = (0b0110 & S) >> 1;//second and third

    return array[row][col];

}

//P4 perm after array
int P4_perm(int parts[2]) {
    if (parts[0] > 3) {
        printf("part 1 is greater than 2 bits");
        return -1;
    }
    if (parts[1] > 3) {
        printf("part 2 is greater than 2 bits");
        return -1;
    }


    int value = (parts[0] << 2) + parts[1];
    int temp = 0;
    //higher part 
    int P4_GUIDE1[4] = { 2,0,1,-3 }; //Shift positions according to pdf
    for (int i = 0; i < 4; i++) {
        if (P4_GUIDE1[i] >= 0)
            temp += (value & (1 << i)) << P4_GUIDE1[i];
        else
            temp += (value & (1 << i)) >> -P4_GUIDE1[i];
    }


    return temp;
}


//encrypting function
int process(int LR[2], int k, int* LFR) {

    int EP, XOROUT1;

    int s0[4][4] = { {1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2} };
    int s1[4][4] = { {0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3} };

    int S[2]; //for s0 and s1 respectivly
    int parts[2]; //returns from s0 and s1
    int p4;
    
    /*
    printf("\nLEFT: %d\n", LR[0]);
    print_bin(LR[0], 4);
    printf("RIGHT: %d\n", LR[1]);
    print_bin(LR[1], 4);
    */

    //EP
    EP = EP_perm(LR[1]);
    if (EP == -1) { //saftey
        return -1;
    }
    
    /*
    printf("\nEP: %d\n", EP);
    print_bin(EP, 8);
    */

    //XOR OUT
    XOROUT1 = EP ^ k;
    /*
    printf("\nEP XOR k1 out: %d\n", XOROUT1);
    print_bin(XOROUT1, 8);
    */


    //SPLIT 8 #2
    Split(XOROUT1, 8, S);
    /*
    printf("\nS0  PART: %d\n", S[0]);
    print_bin(S[0], 4);
    printf("S1 PART: %d\n", S[1]);
    print_bin(S[1], 4);
    */

    //GRID
    parts[0] = grid(S[0], s0);
    parts[1] = grid(S[1], s1);

    /*
    printf("\nS0  return: %d\n", parts[0]);
    print_bin(parts[0], 2);
    printf("S1 return: %d\n", parts[1]);
    print_bin(parts[1], 2);
    */

    //P4
    p4 = P4_perm(parts);
    if (p4 == -1) { //saftey
        return -1; //not really worth it
    }
    
    /*
    printf("\nP4: %d\n", p4);
    print_bin(p4, 4);
    */

    //XOR OUT 2
    *LFR = p4 ^ LR[0]; //result
    /*
    printf("\np4(LF) XOR k out: %d\n", *LFR);
    print_bin(*LFR, 4);

    printf("\n(R)Right: %d\n", LR[1]);
    print_bin(LR[1], 4);
    */
    LFR++;
    *LFR = LR[1]; //right

    return 0;
}

// function to calclulate REVERSED IP PERM
int IP_REV_perm(int message) {
    if (message > 255) {
        printf("value is greater than 8 bits");
        return -1; 
    }

    int temp = 0;
    int IP_GUIDE[8] = { 1,2,-2,1,3,0,-4,-1 }; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) {
        if (IP_GUIDE[i] >= 0)
            temp += (message & (1 << i)) << IP_GUIDE[i];
        else
            temp += (message & (1 << i)) >> -IP_GUIDE[i];
    }
    return temp;
}

//Gets k1 and k2 from key
int init(int key, int *k1, int *k2) {
    int split[2];
    //P10
    key = P10_perm(key);

    if (key == -1) { //saftey
        return -1;
    }
    /*
    printf("\nP10: %d\n", key);
    print_bin(key, 10);
    */

    //SPLIT 10
    Split(key, 10, split);
    /*
    printf("\nsplit1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("split2: %d\n", split[1]);
    print_bin(split[1], 5);
    */

    //K1
    LS_1(split);
    /*
    printf("\nls1 shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("ls1 shift2: %d\n", split[1]);
    print_bin(split[1], 5);
    */
    *k1 = K_perm(split);
    /*
    printf("\nK1: %d\n", k1);
    print_bin(k1, 8);
    */

    //K2
    LS_1(split);
    LS_1(split);
    /*
    printf("\nls2 shift1: %d\n", split[0]);
    print_bin(split[0], 5);
    printf("ls2 shift2: %d\n", split[1]);
    print_bin(split[1], 5);
    */

    *k2 = K_perm(split);
    /*
    printf("\nK2: %d\n", k2);
    print_bin(k2, 8);
    */

    return 0;
}
int encrypt(int message, int k1, int k2) {
    int output, LR[2];
    int enc1[2], enc2[2];

    //ip perm
    message = IP_perm(message);

    if (message == -1) { //saftey
        return -1;
    }
    /*
    printf("\nIP: %d\n", message);
    print_bin(message, 8);
    */

    //SPLIT 8
    Split(message, 8, LR);

    //ENCRYPT 1
    //printf("\nENCRYPT #1\n");
    if (process(LR, k1, enc1) == -1) { //saftey
        return -1;
    }
    

    //switch
    LR[0] = enc1[1];
    LR[1] = enc1[0];

    //ENCRYPT 2
    //printf("\nENCRYPT #2\n");
    if (process(LR, k2, enc2) == -1) { //saftey
        return -1;
    }

    //OUTPUT
    output = 0;
    output += (enc2[0] << 4) + enc2[1];

    //REVERSED PERMUTATION
    output = IP_REV_perm(output);

    return output;
}

int decrypt(int message, int k1, int k2) {
    int output, LR[2];
    int enc1[2], enc2[2];

    //ip perm
    message = IP_perm(message);

    if (message == -1) { //saftey
        return -1;
    }
    /*
    printf("\nIP: %d\n", message);
    print_bin(message, 8);
    */

    //SPLIT 8
    Split(message, 8, LR);

    //DECRYPT 1
    //printf("\nDECRYPT #1\n");
    if (process(LR, k2, enc1) == -1) { //saftey
        return -1;
    }
    //switch
    LR[0] = enc1[1];
    LR[1] = enc1[0];

    //DECRYPT 2
    //printf("\nDECRYPT #2\n");
    if (process(LR, k1, enc2) == -1) { //saftey
        return -1;
    }

    //OUTPUT
    output = 0;
    output += (enc2[0] << 4) + enc2[1];

    //REVERSED PERMUTATION
    output = IP_REV_perm(output);

    return output;
}

//bruteforce all keys to find the right ones
int hack(int message, int encrypted) {
    int output,k1,k2;
    int j = 1;
    
    int flag=0;

    for (int i = 0; i < 1024; i++) {
        if (init(i,&k1,&k2) == -1) { //saftey
            return -1;
        }
        output=encrypt(message, k1, k2);
        if (output == encrypted) {
            printf("\n10-bit key Found (#%d):\nDecimal: %d\nBinary: ", j,i);
            print_bin(i, 10);
            j++;
        }
    }

    if (flag) {
        return 1;
    }

    return 0;
}