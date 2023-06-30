#include <stdio.h>
#include "ad9854.h"

int main(void)
{
	
    AD9854_Init();
    AD9854_SetSine(70000000, 4095);
    //AD9854_SetSine(70000000, 2047);
}
