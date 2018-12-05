#include"../Main/STC15W4K48S4.h"
#include"./Universal.h"

char* uctoa(unsigned char value,char *str){
    unsigned char len=0,i,temp;

    do{
        str[len++]=value%10+'0';
        value/=10;
    }while(value);

    for(i=0;i<len/2;i++){
        temp=str[len-i-1];
        str[len-i-1]=str[i];
        str[i]=temp;
    }

    str[len]=0;
    return str;
}

char* uitoa(unsigned int value,char *str){
    unsigned char len=0,i,temp;

    do{
        str[len++]=value%10+'0';
        value/=10;
    }while(value);

    for(i=0;i<len/2;i++){
        temp=str[len-i-1];
        str[len-i-1]=str[i];
        str[i]=temp;
    }

    str[len]=0;
    return str;
}

char* ultoa(unsigned long int value,char *str){
    unsigned char len=0,i,temp;

    do{
        str[len++]=value%10+'0';
        value/=10;
    }while(value);

    for(i=0;i<len/2;i++){
        temp=str[len-i-1];
        str[len-i-1]=str[i];
        str[i]=temp;
    }

    str[len]=0;
    return str;
}
