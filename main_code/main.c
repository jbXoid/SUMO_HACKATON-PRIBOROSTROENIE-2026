#include "main.h"

void init_port(void);

// REMADE: made tabs to make code more readable
int main (void)	
{
    cli();

    InitMessages();
    Init_GTimer();

    init_timer1();
    init_port();

    #ifdef USART_ON
    init_USART();
    #endif //USART_ON

    sei();

    InitFSM_MENU ();
    InitFSM_Go();
    InitFSM_SHARP_ADC();

    while (1)
    {
        if(start_state == 1)
        {
            ProcessFSM_START();
        }
                
        ProcessFSM_TORNADO();
        //ProcessFSM_PERIMETER();
        ProcessFSM_SHARP_ADC();
        ProcessFSM_Go();
        ProcessMessages();

        #ifdef USART_ON
        ProcessFSM_USART_TX();
        #endif //USART_ON

        if(menu_state == 1)
        {
            ProcessFSM_MENU();
        }
    }

}

void init_port(void)
{
	DDRB |= (1<<ENA|1<<ENB|1<<IN1);	//Управление двигателями
	DDRD |= (1<<IN2|1<<IN3|1<<IN4|0<<BUTTOM_L|0<<BUTTOM_R); //Управление двигателями - порты на выход; кнопки PD3,PD2 - порты на вход.
	PORTD |= (1<<BUTTOM_L|1<<BUTTOM_R); //Кнопки PD3,PD2 - порты в режиме PullUp с подтяжкой к 1.

	DDRC |= (0<<DDC5|0<<DDC4|0<<DDC3|0<<DDC2|0<<DDC1|0<<DDC0); //Входы (инфракрасные датчики и датчик расстояния) в режиме Hi-Z

	ADC_INIT_R; //инициализация АЦП. Читаем сигнал с порта PC5 (правый датчик). Данные берем с ADCH. Опорное-5V(255). Сигнал до 3V(153).
	ADC_ON; //запуск АЦП.
}
