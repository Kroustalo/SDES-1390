//Dimitrios Sideris AM: 1390 S-DES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
/*
• Είσοδος:   00010101
• Κλειδί:  0101101000
• Έξοδος:    11001111
*/




//const int P10_GUIDE[10]={4,2,1,6,0,8,5,9,7,3};
const int P10_GUIDE[10]={2,4,1,6,3,9,0,8,7,5};//P10 Permutation Array according to "S_DES Example.pdf"

//Prototypes
void Print_bin(int array[],int length); //print number on screen
void convert_table(int n,int *table,int length); //make x values segments to an array
void P10_perm(int *table); //function to calclulate P10
void P10_split(int key[10],int *split); // splits p10 in two parts

int main(void){

    int input[8],key[10],output[8];// Input, key encrypted output.
    int K[10],Split[2][5],K1[8],K2[8];//steps for function.

    int in_test= 0b00010101;
    int key_test=0b1010000010;
    convert_table(in_test,input,8);
    convert_table(key_test,key,10);

    Print_bin(key,10);
    P10_perm(key);
    P10_split(key,Split);




    //Print_bin(input,8);
    Print_bin(key,10);
    Print_bin(Split[1],5);
    Print_bin(Split[2],5);

    return 0;

}

//Print Bin value on screen
void Print_bin(int array[],int length){
    for(int i=0;i<length;i++)
        printf("%d",array[i]);

    printf("\n");
}

//make x values segments to an array
void convert_table(int n,int *table,int length){
    if(n>pow(2,length)){
        perror("Error Value Exceeds Length\n"); //value doe not fit on length
        exit(7);
    }

    for(int i=length-1;i>=0;i--){
        if(n==0)    //store zeros
            *(table+i)=0; //store inverse
        else{       //store data
            *(table+i)=n%2; //store inverse
             n=n/2;
        }
    }
}

//function to calclulate P10
void P10_perm(int table[]){

    int temp[10];
    for(int i=0;i<10;i++){
        temp[i]=*(table+P10_GUIDE[i]);
    }
    for(int i=0;i<10;i++){
        *(table+i)=temp[i];
    }
}

void P10_split(int key[10],int *split){ // splits p10 in two parts
    for(int i=0;i<10;i++){
        *(split+i)=key[i];      //first split
        *(split+5+i)=key[i+5];  //second split
    }
}

