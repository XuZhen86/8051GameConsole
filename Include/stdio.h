#ifndef STDIO_H_
#define STDIO_H_

#ifndef EOF
    #define EOF -1
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef _SIZE_T
    #define _SIZE_T
    typedef unsigned int size_t;
#endif

extern char _getkey(void);
extern char getchar(void);
extern char ungetchar(char);
extern char putchar(char);
extern int printf(const char *,...);
extern int sprintf(char *,const char *,...);
extern int vprintf(const char *,char *);
extern int vsprintf(char *,const char *,char *);
extern char *gets(char *,int n);
extern int scanf(const char *,...);
extern int sscanf(char *,const char *,...);
extern int puts(const char *);

#endif

