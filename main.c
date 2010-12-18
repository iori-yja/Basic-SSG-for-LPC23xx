#include"LPC23xx.h"
#include"interrupt.h"
#include"drv.h"
/* �|�[�g�R���g���[���̃��W�X�^ */
#define GPIO_PINSEL1	((volatile unsigned int *)(0xE002C004))
#define GPIO_PINMODE1	((volatile unsigned int *)(0xE002C044))


/* ���荞�݃R���g���[��(VIC)�̃��W�X�^ */
#define TIMER0_INT_BIT  (0x00000010)
#define WRITTEN 10
int g_i=1;

void Delay(volatile unsigned long nCount);
/* IRQ ���荞�ݏ��� */
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
void ymzwrite0(int frec,int adr){
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
void ymzwrite1(int value,int adr){
		FIO4PIN=0x00;
		FIO4PIN=adr;
		FIO4PIN=(0x200+adr);
		FIO4PIN=(0x100+value);
		FIO4PIN=(0x300+value);
//		Delay(10);
}
//----------------------------------------------------------------------------

int main(void)
{
	SCS = SCS | 1;    //FGPIO Select
	set_pll();
	VICIntSelect = 0;	//�S�Ă̊��荞�݂�FIQ�ł͂Ȃ�IRQ�ɂ���
	T0PR  =  0x00000000;	/* �v���X�P�[������  */
	PCLKSEL1=0x00000004; /*TIMER0��CPU���x��*/
	T0MR0 =  72000000;	/*0.1ms=1C20*/
	T0MCR =  0x00000003;	/* Match����TC�N���A & ���荞�� */

	/* VIC�Ɋ��荞�݋����w������        */
	VICIntEnable = TIMER0_INT_BIT;

	RegisterVector(TIMER0_INT, Isr_TIMER0, PRI_LOWEST, CLASS_IRQ);

	IrqEnable();

	/* �^�C�}�X�^�[�g                     */
	T0TCR = 1;


//	Delay_ms(100);
	FIO4DIR = 0x3FF;
	FIO1DIR = 0x40000;
	ymzwrite1(0x3E,0);
	ymzwrite1(0x1F,0);
	ymzwrite1(0xF,0x8);
	ymzwrite1(0x1F,0xF);
	ymzwrite0(568,0);
	ymzwrite0(1136,2);
	ymzwrite0(0xF00,11);
	ymzwrite1(0x8,13);

	while(1){
		Delay_ms(1);
		FIO1PIN=0;
		Delay_ms(1000);
		FIO1PIN=0x40000;
		}
