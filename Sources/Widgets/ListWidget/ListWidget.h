#ifndef LISTWIDGET_H_
#define LISTWIDGET_H_

unsigned char listWidget_selectFromList(unsigned char code *title,unsigned char code *items[],unsigned char itemCount,void (*sigCurrentItemChanged)(unsigned char),bit saveBuffer);

static unsigned char *strcpySpaceExtend(unsigned char *destination,unsigned char *source);

#endif
