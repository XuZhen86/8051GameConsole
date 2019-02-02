#ifndef STDLIB_H_
#define STDLIB_H_

#ifndef _SIZE_T
    #define _SIZE_T
    typedef unsigned int size_t;
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef _WCHAR_T_DEFINED_
    #define _WCHAR_T_DEFINED_
    typedef char wchar_t;
#endif

#ifndef RAND_MAX
    #define RAND_MAX (32767u)
#endif

extern char cabs(char val);
extern int abs(int val);
extern long labs(long val);

extern float atof(const char *s1);
extern long atol(const char *s1);
extern int atoi(const char *s1);
extern int rand(void);
extern void srand(unsigned int seed);

extern float strtod(const char *nptr,char **endptr);
extern long strtol(const char *nptr,char **endptr,unsigned char base);
extern unsigned long strtoul(const char *nptr,char **endptr,unsigned char base);

extern void *malloc(unsigned int size);
extern void *calloc(unsigned int num,unsigned int size);
extern void *realloc(void *ptr,unsigned int size);
extern void free(void *ptr);

// extern void abort(void);

#endif // __STDLIB_H__
