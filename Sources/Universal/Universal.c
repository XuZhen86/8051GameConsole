#include"Sources/Universal/Universal.h"

sbit systemBusyLed=P2^5;
bit delay(unsigned char i,unsigned char j,unsigned char k){
    systemBusyLed=0;
    do{
        do{
            while(k--);
        }while(j--);
    }while(i--);
    systemBusyLed=1;
    return 1;
}

bit delayBusy(unsigned char i,unsigned char j,unsigned char k){
    do{
        do{
            while(k--);
        }while(j--);
    }while(i--);
    return 1;
}

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
