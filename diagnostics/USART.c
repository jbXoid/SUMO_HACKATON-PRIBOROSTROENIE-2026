#include "main.h"

uint8_t usart_rx_udr;

void init_USART(void)
{
	UBRR0L = LO(MYUBRR);
	UBRR0H = HI(MYUBRR);

	USART_CONTROL_INIT;
	USART_RXTX_IE_ON;
	LED_ON;
}

ISR(USART_RX_vect)
{
usart_rx_udr=UDR0;
}

void ProcessFSM_USART_TX (void)
{
cli();
	Start_GTimer (USART_Timer); //
	if (Get_GTimer(USART_Timer)>T_USART)
		{		
		Stop_GTimer(USART_Timer);

		switch(usart_rx_udr)
			{
			case '1':
				if (UCSR0A&(1<<UDRE0)) 
				UDR0=ADCH_LL;		
			break;
	
			case '2':
				if (UCSR0A&(1<<UDRE0))
				UDR0=ADCH_L;
			break;

			case '3':
				if (UCSR0A&(1<<UDRE0))
				UDR0=ADCH_R;
			break;

			case '4':
				if (UCSR0A&(1<<UDRE0))
				UDR0=ADCH_RR;
			break;

			default: break;
			}
		}
sei();
}


// Sending text by UART
void send_text_uart( unsigned char* text, uint16_t text_size ) {
    
    for( uint16_t char_pos = 0 ; char_pos < text_size ; char_pos ++ ) {

        while ( !( UCSR0A & (1<<UDRE0)) ); // Waiting for empty transmit buffer
        UDR0 = text[char_pos]; // Sending next char

    }

}
