#include"../LPC23xx.h"

void motorDriveMainStream(int acc){
static long Dist;
static int V, acs;//verocity,acs
static int direc;

acs = acc;
while(1){
FIO4SET0=0x1;
Delay(72000);
FIO4CLR0=0x1;
}
}
