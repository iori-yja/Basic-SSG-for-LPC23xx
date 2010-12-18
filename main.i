# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 1 "LPC23xx.h" 1
# 2 "main.c" 2
# 1 "interrupt.h" 1





void RegisterVector (int IntNum, void(*Isr)(void), int Priority, int IsFiq);
void ClearVector (void);
void FiqEnable (void);
void FiqDisable (void);
void IrqEnable (void);
void IrqDisable (void);
# 3 "main.c" 2
# 1 "drv.h" 1
void set_pll(void);
void stepperinit(void);
void vStepper0rise(void);
void vStepper0down(void);
void vStepper1rise(void);
void vStepper1down(void);
void lwheeltsk(unsigned int number);
void rwheeltsk(unsigned int number);
unsigned int dstnsa,dstnsb;


int status;
# 4 "main.c" 2
# 12 "main.c"
void Delay(volatile unsigned long nCount);


void Isr_TIMER0 (void)
{
static unsigned int times=0;
 times++;
# 34 "main.c"
 wheel(times);
 (*(volatile unsigned long *)(0xE0004000 + 0x00)) = 1;
}
void Delay(volatile unsigned long nCount)
{
 for(; nCount != 0; nCount--);
}


int main(void)
{
 (*(volatile unsigned long *)(0xE01FC000 + 0x1A0)) = (*(volatile unsigned long *)(0xE01FC000 + 0x1A0)) | 1;
 set_pll();
 (*(volatile unsigned long *)(0xFFFFF000 + 0x00C)) = 0;
 stepperinit();
 (*(volatile unsigned long *)(0x3FFFC000 + 0x00))= 0x00000000;
 (*(volatile unsigned long *)(0xE0004000 + 0x0C)) = 0x00000000;

 (*(volatile unsigned long *)(0xE0004000 + 0x18)) = 0x0000F200;
 (*(volatile unsigned long *)(0xE0004000 + 0x14)) = 0x00000003;
 (*(volatile unsigned long *)(0x3FFFC000 + 0x94))=0x2;


 (*(volatile unsigned long *)(0xFFFFF000 + 0x010)) = (0x00000010);

 RegisterVector(4, Isr_TIMER0, 15, 0);

 IrqEnable();


 (*(volatile unsigned long *)(0xE0004000 + 0x04)) = 1;

 while(1){



}
}
