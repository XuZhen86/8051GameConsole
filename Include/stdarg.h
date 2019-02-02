#ifndef STDARG_H_
#define STDARG_H_

typedef char *va_list;

#define va_start(ap,v) ap=(va_list)&v+sizeof(v)
#define va_arg(ap,t) (((t*)ap)++[0])
#define va_end(ap) (ap=(va_list)0)

#endif
