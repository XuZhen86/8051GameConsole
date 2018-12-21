#ifndef VERSION_H_
#define VERSION_H_

void Version_show();
unsigned char Version_getCompileDate();
unsigned char Version_getCompileTime();
unsigned char Version_getC51Version();
unsigned char Version_getMemModel();
char *Version_getVersionString();

#endif
