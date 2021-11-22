#ifndef PIC_H_
#define PIC_H_

#include "system.h"

void initPIC();
void setPIC1Mask(BYTE mask);
void setPIC2Mask(BYTE mask);
void acknowledgePIC1();
void acknowledgePIC2();

#endif /*PIC_H_*/
