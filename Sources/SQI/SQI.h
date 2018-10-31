#ifndef SQI_H_
#define SQI_H_

void sqi_initialize();

unsigned char sqi_send(unsigned char imm8);
unsigned char *sqi_sendSeq(unsigned char *src,unsigned int len);
unsigned char sqi_sendN(unsigned char imm8,unsigned int count);

unsigned char sqi_recv();
unsigned char *sqi_recvSeq(unsigned char *dst,unsigned int len);

#endif
