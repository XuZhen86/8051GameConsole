#ifndef VERSIONSTATIC_H_
#define VERSIONSTATIC_H_

#define xStringize(s) stringize(s)
#define stringize(s) #s

static const char code *TITLE="Version";
static const unsigned char code ITEM_COUNT=5;
static const char code *ITEMS[]={
    "Ver  "VERSION_STRING,
    "Date "__DATE2__,
    "Time "__TIME__,
    "C51  "xStringize(__C51__),
    "Mem  "xStringize(__MODEL__)
};

#endif
