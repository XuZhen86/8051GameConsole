#ifndef SNAKESTATIC_H_
#define SNAKESTATIC_H_

static bit selectLevel();
static void play();
static void pause();

static void loadData();
static void storeData();
static void generateDefaultData(IapFile *file);

static void initializeGame();
static void restartTicks();
static void renewFood();
static unsigned int setMap(unsigned char x,unsigned char y,unsigned int val);
static unsigned int getMap(unsigned char x,unsigned char y);

static void drawScreen();
static void drawMap();
static void drawScore();

static void dumpMap();

#endif
