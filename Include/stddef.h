#ifndef STDDEF_H_
#define STDDEF_H_

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef _SIZE_T
    #define _SIZE_T
    typedef unsigned int size_t;
#endif

#ifndef _WCHAR_T_DEFINED_
    #define _WCHAR_T_DEFINED_
    typedef char wchar_t;
#endif

#define offsetof(s,m) ((size_t)&(((s code *)0)->m))

#endif
