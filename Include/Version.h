#ifndef VERSION_H_
#define VERSION_H_

// Show a list contains version info
void Version_show();

// Get version info
char *Version_getCompileDate();
char *Version_getCompileTime();
char *Version_getC51Version();
char *Version_getMemModel();
char *Version_getVersionString();

#endif
