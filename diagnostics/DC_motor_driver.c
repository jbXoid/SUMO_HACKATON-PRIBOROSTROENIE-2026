#include "main.h"

void DC_Motors (Action action, uint8_t speed)
{
cli();
    
    OCR1A = speed;
    OCR1B = speed;

	switch(action) {

        case FORWARD:

            PORTB |= (1<<0);
            PORTD &= ~(1<<7);

            PORTD |= (1<<4);
            PORTD &= ~(1<<6);


            break;

        case BACK:

            PORTB &= ~(1<<0);
            PORTD |= (1<<7);

            PORTD &= ~(1<<4);
            PORTD |= (1<<6);

            break;

        case LEFT:

            PORTB &= ~(1<<0);
            PORTD &= ~(1<<7);

            PORTD |= (1<<4);
            PORTD &= ~(1<<6);

            break;
 
        case RIGHT:

            PORTB |= (1<<0);
            PORTD &= ~(1<<7);

            PORTD &= ~(1<<4);
            PORTD &= ~(1<<6);

            break;

        case TORNADO_LEFT:

            PORTB &= ~(1<<0);
            PORTD |= (1<<7);

            PORTD |= (1<<4);
            PORTD &= ~(1<<6);
            
            break;
        
        case TORNADO_RIGHT:

            PORTB |= (1<<0);
            PORTD &= ~(1<<7);

            PORTD &= ~(1<<4);
            PORTD |= (1<<6);
            
            break;


        case STOP:

            PORTB &= ~(1<<0);
            PORTD &= ~(1<<7);

            PORTD &= ~(1<<4);
            PORTD &= ~(1<<6);
            
            break;

    }

    OCR1A = speed;
    OCR1B = speed;
sei();	
}
//В рамках организации программы через автоматы FSM не представляется возможным создать функцию DC_Motors с возможностью паузы на момент реверса.
