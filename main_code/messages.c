#include "main.h"

uint8_t Messages[MAX_MESSAGES];
uint8_t CodeMessages[MAX_MESSAGES];

void InitMessages(void)
{
	uint8_t i;
	for(i=0;i<MAX_MESSAGES;i++)
		Messages[i]=0;
}

void SendMessage(uint8_t msg)
{
cli();
	if(Messages[msg]==0) 
		Messages[msg]=1;
sei();
}

uint8_t GetMessage(uint8_t msg)
{
cli();
	if(Messages[msg]==2)
	{
	Messages[msg]=0;
	sei();
	return 1;
	}
	sei();
	return 0;
}

uint8_t GetCodeMessages(uint8_t msg)
{
	return CodeMessages[msg];
}

void ProcessMessages(void)
{
cli();
	uint8_t i;
	for(i=0;i<MAX_MESSAGES;i++)
	{
		if(Messages[i]==2) Messages[i]=0;
		if(Messages[i]==1) Messages[i]=2;
	}
sei();
}	
