#ifndef PWM_h_
#define PWM_h_

void pwn_initialize(bit clkSrc,unsigned char clkDiv);
void pwm_enable();
void pwm_xsfrFlush();
void pwm_3_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
void pwm_3_timerValueSet(unsigned int t1InitV,unsigned int t2InitV);
void pwm_4_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
void pwm_4_timerValueSet(unsigned int t1InitV,unsigned int t2InitV);
void pwm_5_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
void pwm_5_timerValueSet(unsigned int t1InitV,unsigned int t2InitV);
void pwm_start();

static void interruptRoutine();

#endif
