#ifndef INPUTDIALOG_H_
#define INPUTDIALOG_H_

unsigned char InputDialog_getUChar(unsigned char *title,unsigned char value,unsigned char min,unsigned char max,unsigned char step,void (*sigUCharValueChanged)(unsigned char));

#endif
