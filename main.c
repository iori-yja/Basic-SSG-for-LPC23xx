#include"LPC23xx.h"
#include"interrupt.h"
#include"drv.h"
/* ポートコントローラのレジスタ */
#define GPIO_PINSEL1	((volatile unsigned int *)(0xE002C004))
#define GPIO_PINMODE1	((volatile unsigned int *)(0xE002C044))

/* 割り込みコントローラ(VIC)のレジスタ */
#define TIMER0_INT_BIT  (0x00000010)
#define WRITTEN 10
int g_i=1;

void Delay(volatile unsigned long nCount);
/* IRQ 割り込み処理 */
/*__irq __arm void IRQ_Handler()*/
/*******************************************************/

void Isr_TIMER0 (void)
{
	g_i++;
//	FIO1PIN^=0x00040000;
	T0IR = 1;
}

void Delay_ms(int mCount)
{
	g_i=0;
	while(g_i!=mCount);
}
void Delay(volatile unsigned long nCount)
{
	for(; nCount != 0; nCount--);
}
void ymzwritelong(int frec,int adr){
int highbyte;
		FIO4PIN=0x00;
		FIO4PIN=adr;
		FIO4PIN=0x200+adr;
		FIO4PIN=0x100+(frec&0xFF);
		FIO4PIN=0x300+(frec&0xFF);
		FIO4PIN=adr+1;
		FIO4PIN=0x201+adr;
		highbyte=(frec>>8)&0xFF;
		FIO4PIN=0x100+highbyte;
		FIO4PIN=0x300+(frec>>8);
		
}
void ymzwritebyte(int value,int adr){
		FIO4PIN=0x00;
		FIO4PIN=adr;
		FIO4PIN=(0x200+adr);
		FIO4PIN=(0x100+value);
		FIO4PIN=(0x300+value);
}
//----------------------------------------------------------------------------

int main(void)
{
	SCS = SCS | 1;    //FGPIO Select
	set_pll();
	VICIntSelect = 0;	//全ての割り込みをFIQではなくIRQにする
	T0PR  =  0x00000000;	/* プリスケール無し  */
	PCLKSEL1=0x00000004; /*TIMER0をCPU速度で*/
	T0MR0 =  72000000;	/*0.1ms=1C20*/
	T0MCR =  0x00000003;	/* Match時にTCクリア & 割り込み */
	/* VICに割り込み許可を指示する        */
	VICIntEnable = TIMER0_INT_BIT;
	RegisterVector(TIMER0_INT, Isr_TIMER0, PRI_LOWEST, CLASS_IRQ);
	IrqEnable();
	/* タイマスタート                     */
	T0TCR = 1;


	FIO4DIR = 0x3FF;
	FIO1DIR = 0x40000;
	ymzwritebyte(0x3E,0);
	ymzwritebyte(0x1F,0);
	ymzwritebyte(0xF,0x8);
	ymzwritebyte(0x1F,0xF);
	ymzwritelong(568,0);
	ymzwritelong(1136,2);
	ymzwritelong(0xF00,11);
	ymzwritebyte(0x8,13);

	while(1){
		Delay_ms(1);
		FIO1PIN=0;
		Delay_ms(1000);
		FIO1PIN=0x40000;
	}
}

