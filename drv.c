#include"LPC23xx.h"
#include"interrupt.h"
#include"drv.h"
#include"stdio.h"
void set_pll()
{
	/* 1. もしPLLを使用しているなら、ディスコネクトし、フィード・シーケンスを踏む */
    if ( PLLSTAT & (1 << 25) )
    {
    	PLLCON = 1;				/* Keep PLL Enable disconnected */
    	PLLFEED = 0xAA;
    	PLLFEED = 0x55;
    }

	/* 2. PLLをディセーブルし、フィード・シーケンスを踏む。 */
	PLLCON = 0;		/* Disable PLL */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 3. CPUクロック・ディバイダの設定を、PLLなしでも遅くなりすぎないように設定する。 */
	CCLKCFG = 0;	/* CCLK = PCLK / 1 */

	/* 4. PLLのクロックソースを変更する。 */
	CLKSRCSEL = 0;	/* Select 4MHz internal oscillator*/

	/* 5. PLLCFGへの書き込みを行い、フィード・シーケンスを踏んで有効化する。
	 * PLLCFGは、PLLがディセーブルのときにだけ変更できる。 */
	PLLCFG = 35;	/* Divide by 1, Multiply by 72, PCLK = 4*72 = 288MHz */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 6. PLLをイネーブルにし、フィード・シーケンスを踏む. */
	PLLCON = 1;		/* Enable PLL */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 7. CPUクロック・ディバイダの値を、PLL動作用に変更する。
	 * これは必ず、PLLをクロック源として接続する前に行う。 */
	CCLKCFG = 3;	/* CCLK = PCLK/4 = 288/4 = 72MHz */

	/* 8. PLLSTATレジスタのPLOCK bitを調べて、PLLがロックするまで待つ。
	 * あるいはPLOCK割り込みを使用してもいい。あるいは、PLLの入力クロックが小さい(32kHzなど)なら、
	 * 単純にウェイト・ループを回してもいい。*/
	while ( !(PLLSTAT & (1<<26)))
		;

	/* 9. PLLをシステムに接続し、フィードシーケンスを実行する */
	PLLCON = 3;		/* Select PLL as CPU clock source */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

}
/*******************

* vLedDrv_init			
  
 *******************/
void stepperinit(void)
{
	dstnsa=0;
	dstnsb=0;
	FIO1DIR =0x00040000;	/* P1[18] を出力端子にする       */
	FIO1MASK=0x00000000;
	FIO4DIR =0x0000000F;
	FIO4MASK=0x00000000;
	FIO4PIN =0x00000002;
	FIO4PIN =0x00040000;
}

// or in header file
//#define vLed_LedOn()		FIO1PIN = 0x00000000; /* P1[18] を'0'に（LEDを点ける） */
//#define vLed_LedOff()		FIO1PIN = 0x00040000; /* P1[18] を'1'に（LEDを消す）   */

/*******************

* stepper0				
  
 *******************/
void vStepper0rise(void)
{
dstnsa++;
//	FIO1PIN = 0x00000000; 
	FIO4SET = 0x00000001;
}
void vStepper0down(void)
{
//	FIO1PIN = 0x00040000; 	/* P1[18] を'1'に（LEDを消す）   */
	FIO4CLR = 0x00000001;
}
/*******************

* stepper1

 *******************/
void vStepper1rise(void)
{
dstnsb++;
	FIO1PIN = 0x00000000; 	
	FIO4SET = 0x00000004;
}
void vStepper1down(void)
{
	FIO1PIN = 0x00040000; 	/* P1[18] を'1'に（LEDを消す）   */
	FIO4CLR = 0x00000004;
}

/**************************************************************************************************************************************


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

UNDER HERE IS THE HANDLER OF STEPPERs AND SERVOs. UPWISE IS THEIR DRIVER, AND PLLCONFIGURATION.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



***************************************************************************************************************************************/
/*************************

* STEPPER HANDLER
っ説明。割り込み関数から呼び出し,times変数がnumberに化ける。number300毎に速度で5000をわっちゃう。
*************************/
//CN1-p1=clk.p2=cw/ccw.p3=clk.p4=cw/ccw
void lwheeltsk(unsigned int number){
static int radvlcty=50; /*50[1/100Hz]*/
static int blinky,odd;
static int pasttimes=0;
if(number){
	if(blinky==number){
		if(odd){vStepper1down();
			odd=0;}
		else{vStepper1rise();
			odd=1;}
	pasttimes=number;
}}
if(status<3){
if(number%100==0){
	if(dstnsa<6112){
		blinky=(5000/radvlcty);
		radvlcty+=1;
			}
	else if(dstnsb<(6112+MAWARE)){
		blinky=100;
		}
	else if(dstnsb<(6112+MAWARE+5900)){
		blinky=(5000/radvlcty);
		radvlcty+=1;
		}
	else if(dstnsb<(6112+MAWARE+5900+MAWARE)){
		blinky=100;
		FIO4SET=0x0008;
		}
	else if((dstnsb<(6112+MAWARE+5900+MAWARE+200))&&(status==0))FIO4CLR=0x0008;
	else if(dstnsb<(6112+MAWARE+5900+MAWARE+700))FIO4SET=0x0008;
//	else if(!status&0x01)status++,pasttimes=0,blinky=10000000;
	}}
else if(status==4)blinky=100;
else if(status==5){
	if(dstnsb<1600)FIO4CLR=0x0008;
	else status=6,dstnsa=0,dstnsb=0,FIO4SET=0x0008;
	}
}
void rwheeltsk(unsigned int number){
static int radvlcty=50; /*50[1/100Hz]*/
static int blinky,odd;
static int pasttimes=0;
if(number){
	if(blinky==(number-pasttimes)){
		if(odd){vStepper1down();
			odd=0;}
		else{vStepper1rise();
			odd=1;}
	pasttimes=number;		
}}
if(status<3){
if(number%100==0){
	if(dstnsa<6112){
		blinky=(5000/radvlcty);
		radvlcty+=1;
			}
	else if(dstnsa<(6112+MAWARE)){
		blinky=100;
		FIO4CLR=0x0002;
		}
	else if(dstnsa<6112+MAWARE+5900){
		FIO4SET=0x0002;
		blinky=(5000/radvlcty);
		radvlcty+=1;
		}
	else if((dstnsa<(6112+MAWARE+5900+MAWARE+200))&&(status==0)){
		blinky=100;
		}
	else if(dstnsa<(6112+MAWARE+5900+MAWARE+700)) FIO4CLR=0x0002;
//	else if(!status&0x02)status+=2,pasttimes=0,blinky=10000000;
	else status=4,dstnsa=0,dstnsb=0;	
	}}
//else if((status==3)&&(number>=50))status=4;
else if(status==4){
	if(dstnsa<800){
		blinky=100;
		FIO4SET=0x0002;
		}
	else status=5,dstnsa=0,dstnsb=0,FIO4CLR=0x0002;
	}
else if(status==6){
	if(dstnsa<800)FIO4SET=0x0002;
	else status=7,dstnsa=0,dstnsb=0,FIO4CLR=0x0002;
	}
else if(status==7){if(dstnsa>=300)status=4;}

}
void wheel(int a){
if(dstnsa<=612||dstnsb<=612){
	}
else if(dstnsa<=(612+MAWARE)||dstnsb<=(612+MAWARE)){
	FIO4SET=0x8;
	}
else if(dstnsa<=(6112+MAWARE+5900)||dstnsb<=(6112+MAWARE+5900)){
	FIO4CLR=0x8;
	}
else if(dstnsa<=(6112+MAWARE+5900+MAWARE)||dstnsb<=(6112+MAWARE+5900+MAWARE)){
	FIO4SET=0x8;
	}
else if(dstnsa<=(6112+MAWARE+5900+MAWARE+700)||dstnsb<=(6112+MAWARE+5900+MAWARE+700)){
	FIO4CLR=0x02;
	}
else {
	FIO4SET=0x02;
	FIO4CLR=0x08;
	}
if(a%2){vStepper1down();
	vStepper0down();
}
else{vStepper1rise();
	vStepper0rise();
}
}
//--------------------------
//status table.	status変数について
//0  <-  To Ball zone(not yet against wall)
//1  <-  To Ball zone(Against Wall!)
//2  <-  To Ball zone(N/A)
//3  <-  Already Reached to the Ball zone.
//4~7<-  Searching some balls....
//8  <-  A Ball Found.
//9  <-  Captured A Ball.
//10 <-  To The Goal Area.
//11 <-  To Goal Area(Left wheel reached)
//12 <-  To Goal Area(right wheel reached)
//13 <-  Already reached to Goal Area.
