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

int main(void) {

    int input, key, output;
    
    input = 0b00010101;
    key = 0b1010000010; //642
          // 0b1000001100 524
    printf("OG: %d\n", key);

    key=P10_perm(key);


    if (key == -1) {
        return 1;
    }

    printf("\nPERM: %d\n", key);
    return 0;

}



//function to calclulate P10
int P10_perm(int key) { 
    if (key > 1023) {
        printf("value is greater than 10 bits");
        return -1;
    }

    int temp = 0;
    int P10_GUIDE[10] = { 3,7,9,5,8,0,6,1,2,4}; //Shift positions according to pdf
    for (int i = 0; i < 10; i++) {
        temp += (key & (0b1000000000 >> i)) / (0b1000000000 >> i) * pow(2, P10_GUIDE[i]);
    }
    return temp;

}