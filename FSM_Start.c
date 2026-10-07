#include "main.h"
//-----------Global-------------------------------------------------------------------------------------------------
uint8_t start_state=1;
//------------------------------------------------------------------------------------------------------------------
static uint8_t fsm_START_state;

void ProcessFSM_START (void)
{
static uint8_t a; //переключатель (левая или правая кнопка)

cli();
	switch (fsm_START_state)
	{
	case 0:

		if(GetMessage(MSG_START_R)) //Если пришло сообщение о нажатии правой кнопки
			{
			fsm_START_state = 7;
			a = 0;
			}
			else if(GetMessage(MSG_START_L)) //Если пришло сообщение о нажатии левой кнопки
				{
				fsm_START_state = 7;
				a = 1;
				}
			else if(GetMessage(MSG_START_SLOW)) //Если пришло сообщение о нажатии левой кнопки
				{
				fsm_START_state = 7;
				a = 2;
				}
			else if(GetMessage(MSG_START_FAST)) //Если пришло сообщение о нажатии левой кнопки
				{
				fsm_START_state = 7;
				a = 3;
				}
	break;

	case 1://Первый этап ПРАВОГО манёвра

		PORTB &= ~(1<<IN1);
		PORTD |= (1<<IN2|0<<IN3|1<<IN4);

		OCR1A=0;
		OCR1B=SPEED_START_1; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения левого доворота к краю ринга-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer)>T_START_R_1) 
				{
				fsm_START_state = 3;
				Stop_GTimer(Start_Timer);	
				}

	break;

	case 2://Первый этап ЛЕВОГО манёвра

		PORTB &= ~(1<<IN1);
		PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

		PORTD |= (1<<IN2|0<<IN3|1<<IN4);

		OCR1A = SPEED_START_1;
		OCR1B = 0; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения левого доворота к краю ринга-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer) > T_START_L_1) 
				{
				fsm_START_state = 4;
				Stop_GTimer(Start_Timer);	
				}


	break;

	case 3://Второй этап ПРАВОГО манёвра. Правая дуга. Проход по краю ринга.

		OCR1A = SPEED_START_2;
		OCR1B = SPEED_START_2-START_L; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения левой дуги-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer) > T_START_R_2) 
				{
				fsm_START_state = 5;
				Stop_GTimer(Start_Timer);	
				}

	break;

	case 4://Второй этап ЛЕВОГО манёвра. Левая дуга. Проход по краю ринга.

		OCR1A = SPEED_START_2 - START_R;
		OCR1B = SPEED_START_2; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения правой дуги-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer) > T_START_L_2) 
				{
				fsm_START_state = 6;
				Stop_GTimer(Start_Timer);	
				}
	break;

	case 5://Третий этап ПРАВОГО манёвра. Доворот в центр ринга.

		OCR1A = SPEED_START_3;
		OCR1B = 0; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения левой дуги-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer) > T_START_R_3) 
				{
				//SendMessage(MSG_TORNADO_L);//-----------------ВРЕМЕННО------------------------------------------!!
				SendMessage(MSG_Go_F);
				Stop_GTimer(Start_Timer);
				fsm_START_state = 0;
				start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
				}

	break;

	case 6://Третий этап ЛЕЕВОГО манёвра. Доворот в центр ринга.

		OCR1A = 0;
		OCR1B = SPEED_START_3; //регистр ШИМ на левый двигатель

		//----------------Отсчёт времени движения левой дуги-------------------------------------
		Start_GTimer (Start_Timer);
			if(Get_GTimer(Start_Timer) > T_START_L_3) 
				{
				//SendMessage(MSG_TORNADO_R);//-----------------ВРЕМЕННО------------------------------------------!!
				SendMessage(MSG_Go_F);
				Stop_GTimer(Start_Timer);
				fsm_START_state = 0;
				start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start	
				}

	break;

	case 7: //Реализация задержки 5000 мс

		Start_GTimer (Start_Timer);
		if(Get_GTimer(Start_Timer) >= T_INIT) 
				{
				Stop_GTimer(Start_Timer);
				LED_OFF;
				switch (a)
					{
					case 0://MSG_START_R
						fsm_START_state = 1;
					break;

					case 1://MSG_START_L
						fsm_START_state = 2;
					break;

					case 2://MSG_START_SLOW
						fsm_START_state = 9;
					break;

					case 3://MSG_START_FAST
						fsm_START_state = 8;
					break;
					}				
				}
		switch (Get_GTimer(Start_Timer))
		{
		case 800:
			LED_ON;
		break;

		case 1000:
			LED_OFF;
		break;

		case 1800:
			LED_ON;
		break;

		case 2000:
			LED_OFF;
		break;

		case 2800:
			LED_ON;
		break;

		case 3000:
			LED_OFF;
		break;

		case 3800:
			LED_ON;
		break;

		case 4000:
			LED_OFF;
		break;

		case 4800:
			LED_ON;
		break;
		}
	break;

	case 8://START FAST

		PORTB &= ~(1<<IN1);
		PORTD |= (1<<IN2|0<<IN3|1<<IN4);

		OCR1A = SPEED_START_FAST - K_LINE_R;
		OCR1B = SPEED_START_FAST - K_LINE_L; //регистр ШИМ на левый двигатель
		Start_GTimer(Start_Timer);

		//----------------Проверка событий-------------------------------------------------------------------------
		if (Get_GTimer(Start_Timer) > T_START_FAST_IGNORE)
			{
				if((ADCH_R > K_ADCH_START_R) && (ADCH_L < K_ADCH_START_L))
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_Go_R);//Переход в FSM_Go
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if((ADCH_R > K_ADCH_START_R) && (ADCH_L > K_ADCH_START_L))
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_Go_F);//Переход в FSM_Go
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}
		
				else if((ADCH_L > K_ADCH_START_L) && (ADCH_R < K_ADCH_START_R))
					{
					Stop_GTimer(Start_Timer);;
					SendMessage(MSG_Go_L);
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if(ADCH_RR >= K_ADCH_START_RR)
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_TORNADO_R);//Переход в ПРАВЫЙ ТОРНАДО
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if(ADCH_LL >= K_ADCH_START_LL)
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_TORNADO_L);//Переход в ПРАВЫЙ ТОРНАДО
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}
			}
		if(Get_GTimer(Start_Timer) > T_START_FAST)
				{
				Stop_GTimer(Start_Timer);
				SendMessage(MSG_TORNADO_R);//Переход в ПРАВЫЙ ТОРНАДО
				fsm_START_state = 0;
				start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
				}

	break;

	case 9: //START SLOW
		PORTB &= ~(1<<IN1);
		PORTD |= (1<<IN2|0<<IN3|1<<IN4);

		OCR1A = SPEED_START_SLOW - K_LINE_R;
		OCR1B = SPEED_START_SLOW - K_LINE_L; //регистр ШИМ на левый двигатель
		Start_GTimer(Start_Timer);

		//----------------Проверка событий-------------------------------------------------------------------------
		if (Get_GTimer(Start_Timer) > T_START_SLOW_IGNORE)
			{
				if((ADCH_R > K_ADCH_START_R) && (ADCH_L < K_ADCH_START_L))
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_Go_R);//Переход в FSM_Go
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if((ADCH_R > K_ADCH_START_R) && (ADCH_L > K_ADCH_START_L))
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_Go_F);//Переход в FSM_Go
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}
		
				else if((ADCH_L > K_ADCH_START_L) && (ADCH_R < K_ADCH_START_R))
					{
					Stop_GTimer(Start_Timer);;
					SendMessage(MSG_Go_L);
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if(ADCH_RR >= K_ADCH_START_RR)
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_TORNADO_R);//Переход в ПРАВЫЙ ТОРНАДО
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}

				else if(ADCH_LL >= K_ADCH_START_LL)
					{
					Stop_GTimer(Start_Timer);
					SendMessage(MSG_TORNADO_L);//Переход в ПРАВЫЙ ТОРНАДО
					fsm_START_state = 0;
					start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
					sei();
					return;
					}
			}

		if(Get_GTimer(Start_Timer) > T_START_SLOW)
				{
				Stop_GTimer(Start_Timer);
				SendMessage(MSG_TORNADO_R);//Переход в ПРАВЫЙ ТОРНАДО
				fsm_START_state = 0;
				start_state = 0;// После этой команды главный цикл main больше не попадёт в FSM_Start
				}
	break;
	}
sei();
}
