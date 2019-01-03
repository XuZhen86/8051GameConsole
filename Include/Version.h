#ifndef VERSION_H_
#define VERSION_H_

void Version_show();

char *Version_getCompileDate();
char *Version_getCompileTime();
char *Version_getC51Version();
char *Version_getMemModel();
char *Version_getVersionString();

#endif
