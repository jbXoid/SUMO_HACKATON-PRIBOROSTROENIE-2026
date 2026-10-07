#include "main.h"

unsigned int GTimers[MAX_GTIMERS];
unsigned char GTStates[MAX_GTIMERS];

void Init_GTimer (void)
{
	unsigned char i;

	TIMER2_ON;
	TIMER2_OVF_INT_ENABLE;

	for(i=0;i<MAX_GTIMERS;i++)
		GTStates[i]=TIMER_STOPPED;
}

void Start_GTimer (unsigned char GTimerID)
{
cli();
	if(GTStates[GTimerID]==TIMER_STOPPED)
		GTimers[GTimerID]=0;

	GTStates[GTimerID]=TIMER_RUNNING;
sei();
}

void Stop_GTimer (unsigned char GTimerID)
{
	GTStates[GTimerID]=TIMER_STOPPED;
}

void Pause_GTimer (unsigned char GTimerID)
{
cli();
	if(GTStates[GTimerID]==TIMER_RUNNING)
		GTStates[GTimerID]=TIMER_PAUSED;
sei();
}

void Release_GTimer (unsigned char GTimerID)
{
cli();
	if(GTStates[GTimerID]==TIMER_PAUSED)
		GTStates[GTimerID]=TIMER_RUNNING;
sei();
}

unsigned int Get_GTimer (unsigned char GTimerID)
{
	return GTimers[GTimerID];
}

ISR(TIMER2_OVF_vect)
{
	unsigned char i;

	TCNT2 = 130; //Настраиваю счетный регистр. Счет каждый раз начинается от 130 до 255,
				 //125 отчета и предделитель на 128 дают прерывание каждую 1мс при 16 MHz.
	for(i=0;i<MAX_GTIMERS;i++)
		if(GTStates[i]==TIMER_RUNNING)
		GTimers[i]++;
}
