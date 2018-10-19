#ifndef PWM_h_
#define PWM_h_

void pwm_initialize(unsigned char clkDiv);

void pwm3_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect);
void pwm3_timerValueSet(unsigned int imm16T1,unsigned int imm16T2);

void pwm_4_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect);
void pwm4_timerValueSet(unsigned int imm16T1,unsigned int imm16T2);

void pwm_5_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect);
void pwm5_timerValueSet(unsigned int imm16T1,unsigned int imm16T2);

#endif
