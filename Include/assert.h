#ifndef ASSERT_H_
#define ASSERT_H_

#include<stdio.h>
#include<stdlib.h>

#ifdef DEBUG_OUTPUT
    #define assert(expr) \
        do{ \
            if(expr){;} \
            else{ \
                printf("Assert failed: " #expr " (file %s line %d)\n",__FILE__,(int)__LINE__); \
                abort(); \
            } \
        }while (0)
#else
    #define assert(expr) ((void)0)
#endif

#endif
