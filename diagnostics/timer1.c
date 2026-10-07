#include "main.h"

void init_timer1 (void)
{
	TIMER1_ON;
	T1_FastPWM_8bit_CLEAR_OC1A;
	T1_FastPWM_8bit_CLEAR_OC1B;
}
