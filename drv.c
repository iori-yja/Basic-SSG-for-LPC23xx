#include"LPC23xx.h"
#include"interrupt.h"
#include"drv.h"
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

