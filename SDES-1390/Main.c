//Dimitrios Sideris AM: 1390 S-DES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h>

#define BUFFERSIZE 20

/* example
 Input:  00010101   021
 Key:    0101101000 360
 Output: 11001111   207
*/

//Prototypes
void print_bin(int n, int bits); //Print Decimal numbers in Binary
void Split(int n, int bits, int* split); //Spilt even bits in pairs

int P10_perm(int key); // Function to calclulate P10
void LS_1(int* split); //Shifts the 5-bit keys array once to the left (with loop)
int K_perm(int split[2]); //Create K1 key

int IP_perm(int message); //Function to calclulate IP perm
int EP_perm(int right); //Function to calclulate EP perm

int grid(int S, int array[4][4]);//Extract elements from array using S
int P4_perm(int parts[2]); //P4 perm after finding numbers from grid

int IP_REV_perm(int message); // function to calclulate REVERSED IP PERM

void init(int key, int* k1, int* k2); //Gets k1 and k2 from key
void process(int LR[2], int k, int* output); //process function
int encrypt(int message, int k1, int k2);  //encrypt
int decrypt(int message, int k1, int k2);  //decrypt

int hack(int message, int encrypted); //bruteforce all keys to find the right ones

int read_num(int bits);//reads input

int main(void) {

    char datain[BUFFERSIZE];
    int option;//encrypt 1 | decrypt 2 | hack 3
    int  key, message, encrypted, k1, k2;

    //intro
    printf("Welcome to S-DES program!!!\n");
    //Select Option (char 1,2 or 3)
    do{
        printf("1) Encrypt\n2) Decrypt\n3) Hack\nSelect Option: ");
        fgets(datain,20,stdin);
        if(sscanf(datain, "%d", &option) != 1){
            printf("Error: Value not read Try again\n"); //read again
            option=-1;
        }
        if(option != 1 && option != 2 && option != 3)
            printf("\nError: Not Vaild Option, select again:\n");
    }
    while (option != 1 && option != 2 && option != 3);

    //MESSAGE
    if(option == 1 || option == 3){
         //message
        printf("\nGive 8-bit message (dec/bin): ");
        message=read_num(8);

        printf("8-bit message:\nDecimal: %d\nBinary:  ", message);
        print_bin(message, 8);
    }

    //ENCRPYTED MESSAGE
    if(option == 2 || option == 3){
        //encrypted message
        printf("\nGive 8-bit encrypted message (dec/bin): ");
        encrypted=read_num(8);

        printf("8-bit encrypted message:\nDecimal: %d\nBinary:  ", encrypted);
        print_bin(encrypted, 8);

    }

    //ENCRYPTION KEY
    if(option == 1 || option == 2){
       //key
        printf("\nGive 10-bit key (dec/bin): ");
        key=read_num(10);

        printf("10-bit key:\nDecimal: %d\nBinary:  ", key);
        print_bin(key, 10);
    }


    if (option == 1) { //Encrypt
        printf("\nEncrypting!\n");
        init(key, &k1, &k2);

        encrypted = encrypt(message, k1, k2);

        printf("\nEncoded Message:\nDecimal: %d\nBinary: ", encrypted);
        print_bin(encrypted, 8);
    }
    else if (option == 2) { //Decrypt
        printf("\nDecrypting!\n");
        init(key, &k1, &k2);

        message = decrypt(encrypted, k1, k2);

        printf("\nDecoded:\nDecimal %d\nBinary: ", message);
        print_bin(message, 8);
    }
    else { //Bruteforce Hacking
        printf("\nBrute Force Initializing!\n");

        int found = hack(message, encrypted);
        if(found != 0)
            printf("\n(%d) Keys Found!\n",found);
        else
            printf("\nKeys Not Found\n");
    }

     printf("\nProgram Done\nMade by Dimitrios Sideris 1390\n");
    return 0;

}

//Print Decimal numbers in Binary
void print_bin(int n, int bits) {
    int i;
    for (i = (1 << (bits - 1)); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
    printf("\n");
}

//Spilt even bits in pairs
void Split(int n, int bits, int* split) {
    *split = n >> (bits / 2); //upper part
    split++;
    *split = ((1 << bits / 2) - 1) & n; //lower part
}

// Function to calclulate P10
int P10_perm(int key) {
    int temp = 0;
    int P10_GUIDE[10] = { 4,1,-1,3,-4,3,-1,2,-1,-6 }; //Shift positions according to pdf
    for (int i = 0; i < 10; i++) { //PERMUTATE
        if (P10_GUIDE[i] >= 0)
            temp += (key & (1 << i)) << P10_GUIDE[i]; //move bit left from current pos
        else
            temp += (key & (1 << i)) >> -P10_GUIDE[i]; //move bit right from current pos
    }
    return temp;
}

void LS_1(int* split) { //Shifts the 5-bit keys array once to the left (with loop)
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
    temp = temp & 0b11111111; // get 8-bit number from the two 5-bit numbers
    //print_bin(temp, 8); //test

    int k = 0;
    int P8_GUIDE[8] = { 1,-1,1,2,3,-3,-2,-1}; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) { //PERMUTATE
        if (P8_GUIDE[i] >= 0)
            k += (temp & (1 << i)) << P8_GUIDE[i];
        else
            k += (temp & (1 << i)) >> -P8_GUIDE[i];
    }
    return k;
}

//Function to calclulate IP perm
int IP_perm(int message){
    int temp = 0;
    int IP_GUIDE[8] = { 2,-1,4,-2,-1,0,1,-3}; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) { //PERMUTATE
        if (IP_GUIDE[i] >= 0)
            temp += (message & (1 << i)) << IP_GUIDE[i];
        else
            temp += (message & (1 << i)) >> -IP_GUIDE[i];
    }
    return temp;

}

//Function to calclulate EP perm
int EP_perm(int right) {
    int temp = 0;

    //lower
    int EP_GUIDE[4] = { 3,-1,-1,-1 }; //Shift positions according to pdf
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 4; i++) { //PERMUTATE
            if (EP_GUIDE[i] >= 0)
                temp += (right & (1 << i)) << EP_GUIDE[i];
            else
                temp += (right & (1 << i)) >> -EP_GUIDE[i];
        }
        if (j == 0) {//for lower part kinda hacky but it works
            EP_GUIDE[0] = 1;
            EP_GUIDE[1] = 1;
            EP_GUIDE[2] = 1;
            EP_GUIDE[3] = -3;

            temp = temp << 4;
        }
    }
    return temp;
}

int grid(int S, int array[4][4]) { //Extract elements from array using S
    int row, col;
    row = ((0b1000 & S) >> 2) + (0b0001 & S); //First and Fourth Bits
    col = (0b0110 & S) >> 1; //Second and Third Bits

    return array[row][col];
}

//P4 perm after finding numbers from grid
int P4_perm(int parts[2]) {
    int value = (parts[0] << 2) + parts[1];
    int temp = 0;
    //higher part
    int P4_GUIDE1[4] = { 2,0,1,-3 }; //Shift positions according to pdf
    for (int i = 0; i < 4; i++) { //PERMUTATE
        if (P4_GUIDE1[i] >= 0)
            temp += (value & (1 << i)) << P4_GUIDE1[i];
        else
            temp += (value & (1 << i)) >> -P4_GUIDE1[i];
    }
    return temp;
}


//encrypting function
void process(int LR[2], int k, int* LFR) {

    int EP, XOROUT;

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
    /*
    printf("\nEP: %d\n", EP);
    print_bin(EP, 8);
    */

    //XOR OUT
    XOROUT = EP ^ k;
    /*
    printf("\nEP XOR k1 out: %d\n", XOROUT1);
    print_bin(XOROUT1, 8);
    */


    //SPLIT 8 #2
    Split(XOROUT, 8, S);
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

    /*
    printf("\nP4: %d\n", p4);
    print_bin(p4, 4);
    */

    //LFR
    *LFR = p4 ^ LR[0]; //result
    /*
    printf("\np4(LF) XOR k out: %d\n", *LFR);
    print_bin(*LFR, 4);

    printf("\n(R)Right: %d\n", LR[1]);
    print_bin(LR[1], 4);
    */

    LFR++;
    *LFR = LR[1]; //right

    //LFR array contains the value splitted in each cells

}

// function to calclulate REVERSED IP PERM
int IP_REV_perm(int message) {
    int temp = 0;
    int IP_GUIDE[8] = { 1,2,-2,1,3,0,-4,-1 }; //Shift positions according to pdf
    for (int i = 0; i < 8; i++) { //PERMUTATE
        if (IP_GUIDE[i] >= 0)
            temp += (message & (1 << i)) << IP_GUIDE[i];
        else
            temp += (message & (1 << i)) >> -IP_GUIDE[i];
    }
    return temp;
}

//Gets k1 and k2 from key
void init(int key, int *k1, int *k2) {
    int split[2];
    //P10
    key = P10_perm(key);

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
}
int encrypt(int message, int k1, int k2) {
    int output, LR[2];
    int LFR[2];

    //ip perm
    message = IP_perm(message);

    /*
    printf("\nIP: %d\n", message);
    print_bin(message, 8);
    */

    //SPLIT 8
    Split(message, 8, LR);

    //ENCRYPT 1
    //printf("\nENCRYPT #1\n");
    process(LR, k1, LFR); //arrays work like pointres


    //switch
    LR[0] = LFR[1];
    LR[1] = LFR[0];

    //ENCRYPT 2
    //printf("\nENCRYPT #2\n");
    process(LR, k2, LFR);

    //OUTPUT
    output = 0;
    output += (LFR[0] << 4) + LFR[1];

    //REVERSED PERMUTATION
    output = IP_REV_perm(output);

    return output;
}

int decrypt(int message, int k1, int k2) {
    int output, LR[2];
    int LFR[2];

    //ip perm
    message = IP_perm(message);

    /*
    printf("\nIP: %d\n", message);
    print_bin(message, 8);
    */

    //SPLIT 8
    Split(message, 8, LR);

    //DECRYPT 1
    //printf("\nDECRYPT #1\n");
    process(LR, k2, LFR); //arrays work like pointres

    //switch
    LR[0] = LFR[1];
    LR[1] = LFR[0];

    //DECRYPT 2
    //printf("\nDECRYPT #2\n");
    process(LR, k1, LFR);

    //OUTPUT
    output = 0;
    output += (LFR[0] << 4) + LFR[1];

    //REVERSED PERMUTATION
    output = IP_REV_perm(output);

    return output;
}

//bruteforce all keys to find the right ones
int hack(int message, int encrypted) {
    int output,k1,k2;
    int found = 0;
    unsigned long time,total;
    struct timeval start, end;

    total=0;

    //timings
    gettimeofday(&start,NULL); //start timer
    for (int i = 0; i < 1024; i++) {
        init(i,&k1,&k2);

        output=encrypt(message, k1, k2);

        if (output == encrypted) {
            gettimeofday(&end, NULL); //stop timer

            time=(end.tv_sec * 1000000 + end.tv_usec)-(start.tv_sec * 1000000 + start.tv_usec); //calculate Time
            found++;
            total+=time;// add total time
            printf("\n10-bit key Found (#%d) |  Time: %ld us\nDecimal: %d\nBinary:  ", found, time,i);
            print_bin(i, 10);

            gettimeofday(&start,NULL); //start again
        }
    }

    if (found==0)
        return found;
    else
        printf("\nAverage time per found key: %ld us\n",total/found);

    return found; //return the amount of found keys for main
}

int read_num(int bits){
    int a,i,bin_flag,dec_flag;

    char c[BUFFERSIZE]; //We assume that the length is not exceeded and that the numbers are correctly inputed
    do{
        fgets(c,BUFFERSIZE,stdin);
        a=0,bin_flag=0,dec_flag=1;
        //check if number is decimal
        for(i=0;c[i+1]!='\0';i++){
            if(!(c[i]>='0' && c[i]<='9')){
                dec_flag=0;
                break;
            }
        }

        i=2;//initialize counter
        //if not decimal check if number is binary or not valid
        if(!dec_flag){
            if(c[0]=='0'){ //check if number is binary by having the intendifier "0b"
                if(c[1]=='b' || c[1]=='B'){
                    if(c[2]=='0' || c[2]=='1'){ //check if it's followed by at least one 0 or 1
                        bin_flag=1;
                        while (c[i+1]!= '\0'){// Read a line char by char until the one next to it is NULL

                            //check if valid Bit
                            if(c[i]!='0' && c[i]!='1'){
                                bin_flag=0; //number is not valid binary (and not decimal since the dec_flag is triggered)
                                break;
                            }

                            a <<= 1;                 // shift the bit left
                            a += (c[i] - '0') & 1;  // convert the char to 0/1 and put it at the end of the binary
                            i++;

                        }
                    }
                }
            }
        }
        else if (sscanf(c, "%d", & a) != 1) { //read num as decimal with sscanf to check if valid decimal
                printf("Error: Decimal value not read. Try again: ");
                a=(1<<bits); //make check false for while to continue (because a is altered)
                continue;
            }

        //Print Error for length if at least one of the flags is true
        if(bin_flag||dec_flag){
            if (a >= (1 << bits)) { //check for length
               if (bin_flag)
                 printf("Binary is greater than %d bits, Try again: ", bits);
               else if (dec_flag)
                 printf("Decimal is greater than %d, Try again: ", (1 << bits) - 1);
             }
        }
        else if(!bin_flag&&!dec_flag){ //not bin or dec
            printf("Error: value is not a valid binary. Try again: ");
            a=(1<<bits); //make check false for while to continue (because a is altered)
            continue;
        }

    }while (a>=(1<<bits));

    printf("\n");

    return a;
}


