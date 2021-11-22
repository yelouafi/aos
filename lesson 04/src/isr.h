#ifndef ISR_H_
#define ISR_H_

void IDTSetup();
void IDTsetHandler(int vector, DWORD handler, int dpl);
void isrDispatch(regs r, int vector, int errorCode);
void registerHandler(BYTE vector, Interrupt *interrupt);
void unregisterHandler(BYTE vector);


#endif /*ISR_H_*/
