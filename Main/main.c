#include<Main/STC15W4K48S4.h>
#include<Main/Universal.h>
#include<PWM/PWM.h>
#include<SPI/SPI.h>
#include<LCD12864/LCD12864.h>
#include<ADC/ADC.h>
#include<Main/InterruptRoutine.h>
#include<Joystick/Joystick.h>
#include<I23LC512/I23LC512.h>

#include<stdio.h>

unsigned char counter=0;

void main(){
    unsigned char buffer[16];
    unsigned int address;
    unsigned char i=0,j=0;

    P0M0=0x00;
    P0M1=0x00;
    P1M0=0x00;
    P1M1=0x00;
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P4M0=0x00;
    P4M1=0x00;

    delay(0,0,0);
    interruptInitialize();
    spiInitialize(1,1);
    i23lc512Initialize();
    lcd12864SpiInitialize();
    joystickInitialize();

    for(address=0;address<0xffff;address++){
        delay(5,6,203);

        sprintf(buffer,"addr=0x%04x",address);
        lcd12864StringSet(0,0,buffer,1);

        for(i=0;i<255;i++){
            i23lc512ByteWrite(address,i);
            j=i23lc512ByteRead(address);
            if(i!=j){
                sprintf(buffer,"addr=0x%04x j=%d",address,j);
                lcd12864StringSet(1,0,buffer,1);
            }
        }
    }

    while(delay(1,1,1)){}
}

