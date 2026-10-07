//-----------------------------------------------------------------------------------------------------//
// Ввход в режим "Торнадо" выполняется через приём сообщений MSG_TORNADO_R или MSG_TORNADO_L           //
// При обнаружении цели переходим в режим "Go" через отправку сообщений MSG_Go_XXX                     //
// По истечении времени TIME_TORNADO переходим в режим "Perimeter", отправляем сообщение MSG_Perimeter //
//-----------------------------------------------------------------------------------------------------//
#include "main.h"

static uint8_t fsm_TORNADO_state; //Переменная состояний конечного автомата

void ProcessFSM_TORNADO (void)
{
static uint8_t i_revers; //Показывает нужна ли смена вращения одного или несольких двигателей
cli();

switch (fsm_TORNADO_state)
	{
	case 0:

		if(GetMessage(MSG_TORNADO_R)) //Если пришло сообщение вызова режима ПРАВОГО вращения
			{
			fsm_TORNADO_state=1;
			i_revers=1;
			LED_OFF;
			}
		else if(GetMessage(MSG_TORNADO_L)) //Если пришло сообщение вызова режима ЛЕВОГО вращения
			{
			fsm_TORNADO_state=2;
			i_revers=1;
			LED_OFF;
			}
	break;

	case 1: //Правый ТОРНАДО

		Start_GTimer (Tornado_Timer);

		if (i_revers==1)
			{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			Start_GTimer (Motion_Timer);			//Делаем задержку для плавной остановки двигателей
			if(Get_GTimer(Motion_Timer)>T_REVERS)	//и дальнейшего их пуска
				{
				PORTB |= 1<<IN1;					//Правое вращение вокруг центра масс
				PORTD |= (0<<IN2|0<<IN3|1<<IN4);

				OCR1A = SPEED_TORNADO;
				OCR1B = SPEED_TORNADO; //регистр ШИМ на левый двигатель

				Stop_GTimer(Motion_Timer);
				i_revers=0;	
				}
			}
		else 
			{
				PORTB |= 1<<IN1;					//Правое вращение вокруг центра масс
				PORTD |= (0<<IN2|0<<IN3|1<<IN4);

				OCR1A=SPEED_TORNADO;
				OCR1B=SPEED_TORNADO; //регистр ШИМ на левый двигатель
			}
//----------------Проверка событий-------------------------------------------------------------------------
		if(ADCH_RR>=K_RR_LL) //Цель находится СПРАВА на расстоянии < 30см
			{
			Start_GTimer (RRLL_Timer); //Подсчёт длительности обнаружения цели. Для выявления бокового контакта с соперником.
			if(Get_GTimer(RRLL_Timer)>T_RRLL)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
				PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

				fsm_TORNADO_state=3; //Переход в кейс ухода от атаки в правый бок
				}
			}

			else if(ADCH_R>K_TORNADO_R)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				Stop_GTimer(Tornado_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_R);
				}
/*
			else if((ADCH_R>K_TORNADO_R)&&(ADCH_L>K_TORNADO_L))
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_F);
				}
*/		
			else if(ADCH_L>K_TORNADO_L)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_L);
				}
	
		if((PIN_C<3)&&(fsm_TORNADO_state|=0)) //Контроль края ринга
			{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			Stop_GTimer(Motion_Timer);
			Stop_GTimer(Tornado_Timer);
			Stop_GTimer(RRLL_Timer);

			fsm_TORNADO_state=0;
			SendMessage(MSG_Go_Stop);
			}

		if(Get_GTimer(Tornado_Timer)>T_TORNADO)
			{
			Stop_GTimer(Tornado_Timer);
			Stop_GTimer(RRLL_Timer);
			Stop_GTimer(Motion_Timer);

			//Оставлю до завершения реализации режима периметр
			//fsm_TORNADO_state=0;
			//SendMessage(MSG_Perimeter_Start);

			//------ВРЕМЕННО------------------------------------
			fsm_TORNADO_state=2;
			i_revers=1;
			//--------------------------------------------------
			}

	break;


	case 2: //Левый ТОРНАДО

		Start_GTimer (Tornado_Timer);

		if (i_revers==1)
			{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			Start_GTimer (Motion_Timer);			//Делаем задержку для плавной остановки двигателей
			if(Get_GTimer(Motion_Timer)>T_REVERS)	//и дальнейшего их пуска
				{
				PORTD |= (1<<IN2|1<<IN3|0<<IN4);	//Левое вращение вокруг центра масс

				OCR1A=SPEED_TORNADO;
				OCR1B=SPEED_TORNADO; //регистр ШИМ на левый двигатель

				Stop_GTimer(Motion_Timer);
				i_revers=0;	
				}
			}
		else 
			{
			PORTB &= ~(1<<IN1);
			PORTD |= (1<<IN2|1<<IN3|0<<IN4);	//Левое вращение вокруг центра масс

			OCR1A=SPEED_TORNADO;
			OCR1B=SPEED_TORNADO; //регистр ШИМ на левый двигатель
			}
//----------------Проверка событий-------------------------------------------------------------------------
		if(ADCH_LL>=K_RR_LL) //Цель находится СЛЕВА на расстоянии < 30см
			{
			Start_GTimer (RRLL_Timer); //Подсчёт длительности обнаружения цели. Для выявления бокового контакта с соперником.
			if(Get_GTimer(RRLL_Timer)>T_RRLL)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
				PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

				fsm_TORNADO_state=4; //Переход в кейс ухода от атаки в левый бок
				}
			}

			else if(ADCH_L>K_TORNADO_L)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				Stop_GTimer(Tornado_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_L);
				}
/*
			else if((ADCH_R>K_TORNADO_R)&&(ADCH_L>K_TORNADO_L))
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_F);
				}
*/		
			else if(ADCH_R>K_TORNADO_R)
				{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(Tornado_Timer);
				Stop_GTimer(RRLL_Timer);

				fsm_TORNADO_state=0;
				SendMessage(MSG_Go_R);
				}
	
		if((PIN_C<3)&&(fsm_TORNADO_state|=0)) //Контроль края ринга
			{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			Stop_GTimer(Motion_Timer);
			Stop_GTimer(Tornado_Timer);
			Stop_GTimer(RRLL_Timer);

			fsm_TORNADO_state=0;
			SendMessage(MSG_Go_Stop);
			}

		if(Get_GTimer(Tornado_Timer)>T_TORNADO)
			{
			Stop_GTimer(Tornado_Timer);
			Stop_GTimer(RRLL_Timer);
			Stop_GTimer(Motion_Timer);

			//Оставлю до завершения реализации режима периметр
			//fsm_TORNADO_state=0;
			//SendMessage(MSG_Perimeter_Start);

			//------ВРЕМЕННО------------------------------------
			fsm_TORNADO_state=1;
			i_revers=1;
			//--------------------------------------------------
			}

	break;

	case 3: //Начало манёвра ухода от атаки в ПРАВЫЙ бок. Движение задним ходом прямо.

		PORTB |= 1<<IN1;
		PORTD |= (0<<IN2|1<<IN3|0<<IN4);

		OCR1A=SPEED_BACK_1;
		OCR1B=SPEED_BACK_1;	//регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения задним ходом прямо-------------------------------------
		Start_GTimer (Motion_Timer);
			if(Get_GTimer(Motion_Timer)>T_TORNADO_BACK) 
				{
				fsm_TORNADO_state=5;
				Stop_GTimer(Motion_Timer);	
				}

	break;

	case 4: //Начало манёвра ухода от атаки в ЛЕВЫЙ бок

		PORTB |= 1<<IN1;
		PORTD |= (0<<IN2|1<<IN3|0<<IN4);

		OCR1A=SPEED_BACK_1;
		OCR1B=SPEED_BACK_1;	//регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения задним ходом прямо-------------------------------------
		Start_GTimer (Motion_Timer);
			if(Get_GTimer(Motion_Timer)>T_TORNADO_BACK) 
				{
				fsm_TORNADO_state=6;
				Stop_GTimer(Motion_Timer);	
				}
	break;

	case 5: //Второй этап манёвра ухода от атаки в ПРАВЫЙ бок. Движение задним ходом вправо.

		OCR1A=SPEED_BACK_2-K_BACK_LR;
		OCR1B=SPEED_BACK_2;	//регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения задним ходом прямо-------------------------------------
		Start_GTimer (Motion_Timer);
			if(Get_GTimer(Motion_Timer)>T_TORNADO_BACK_LR) 
				{
				fsm_TORNADO_state=1;
				Stop_GTimer(Motion_Timer);
				
				PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
				PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);	
				}

	break;

	case 6: //Второй этап манёвра ухода от атаки в ЛЕВЫЙ бок. Движение задним ходом влево.

		OCR1A=SPEED_BACK_2;
		OCR1B=SPEED_BACK_2-K_BACK_LR;	//регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения задним ходом прямо-------------------------------------
		Start_GTimer (Motion_Timer);
			if(Get_GTimer(Motion_Timer)>T_TORNADO_BACK_LR) 
				{
				fsm_TORNADO_state=2;
				Stop_GTimer(Motion_Timer);
				
				PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
				PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);	
				}
					
	break;
	}

sei();
}
//Реализовать обработку отправленных сообщений. Осталось PERIMETER
