#include "main.h"
//-----------Global-------------------------------------------------------------------------------------------------
uint8_t PIN_C; //переменная в которой остануться биты (PC0,PC1)датчиков LH,RH. Контроль края ринга.
//------------------------------------------------------------------------------------------------------------------
static uint8_t fsm_Go_state; //переменная автомата ProcessFSM_Go
static uint8_t num_back; // Счётчик кол-ва входов в режим ухода от края

void InitFSM_Go (void)
{
	fsm_Go_state = 0;
	num_back = 0;
	PIN_C = 3;//Для того, чтобы небыло случайного ухода в состоние "объезд линии" при старте. Ринг чёрный, линия белая.
}	

void ProcessFSM_Go (void)
{
cli();
	Start_GTimer (Sensor_Timer); //выдержка времени Sensor_Timer для защиты от "дребезга" показаний датчиков
	if (Get_GTimer(Sensor_Timer)>T_SENSOR)
	{
		PIN_C=PINC;
		PIN_C=PIN_C&3; //Обнуляю разряды 2-7. Смотрим датчики линии.

		Stop_GTimer(Sensor_Timer);
	}
				
	switch (fsm_Go_state)
	{
	case 0:
		if (GetMessage(MSG_Go_R))
		{
			fsm_Go_state=3;
		}
		else if (GetMessage(MSG_Go_L))
		{
			fsm_Go_state=2;
		}
		else if (GetMessage(MSG_Go_F))
		{
			fsm_Go_state=1;
			LED_ON;
		}
		else if (GetMessage(MSG_Go_Stop))
		{
			fsm_Go_state=6;
		}
	break;

	case 1: //Выход на цель
//Продумать алгоритм действий на случай продолжения атаки более 30с
        
        DC_motors(FORWARD, SPEED_F, SPEED_F);


		//----------------Проверка событий-------------------------------------------------------------------------
		if ((ADCH_L >= K_FRONT_DIST) || (ADCH_R >= K_FRONT_DIST)) //Цель на КОВШЕ
		{
			Start_GTimer (RRLL_Timer); //Подсчёт длительности фронтального контакта.
			if(Get_GTimer(RRLL_Timer) > T_FRONT_CONTACT)
			{
				fsm_Go_state = 7;
				Stop_GTimer(RRLL_Timer);
			}
		}

		else if((ADCH_L > K_ADCH_L) && (ADCH_R <= K_ADCH_R)) //Цель находится ЛЕВЕЕ
		{
			LED_OFF;
			fsm_Go_state = 2;
			Stop_GTimer(RRLL_Timer);
		}

		else if((ADCH_L <= K_ADCH_L) && (ADCH_R > K_ADCH_R)) //Цель находится ПРАВЕЕ
		{
			LED_OFF;
			fsm_Go_state = 3;
			Stop_GTimer(RRLL_Timer);
		}

		else if(ADCH_LL > K_ADCH_LL) //Цель находится СИЛЬНО СЛЕВА
		{
			LED_OFF;
			fsm_Go_state = 4;
			Stop_GTimer(RRLL_Timer);
		}

		else if(ADCH_RR > K_ADCH_RR) //Цель находится СИЛЬНО СПРАВА
		{
			LED_OFF;
			fsm_Go_state = 5;
			Stop_GTimer(RRLL_Timer);
		}

		if(PIN_C < 3)//Контроль края ринга
		{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			LED_OFF;
			fsm_Go_state = 6;
			Stop_GTimer(RRLL_Timer);
		}
		
	break;

	case 2: //Левый поворот к цели

        DC_motors(LEFT, SPEED_F - FORWARD_L, SPEED_F);        

		//----------------Проверка событий-------------------------------------------------------------------------

		if((ADCH_L>K_ADCH_L)&&(ADCH_R>K_ADCH_R)) //Цель ПРЯМО по курсу
		{
			fsm_Go_state=1;
			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
			LED_ON;
		}
		else if ((ADCH_R >= K_FRONT_DIST) || (ADCH_L >= K_FRONT_DIST)) //Цель на ковше, но смещена ВЛЕВО
		{
			Start_GTimer (RRLL_Timer); //Подсчёт длительности фронтального контакта.
			if(Get_GTimer(RRLL_Timer) > T_FRONT_CONTACT)
			{
				fsm_Go_state = 7;
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				LED_ON;
			}
		}
		else if((ADCH_L < K_ADCH_L) && (ADCH_R > K_ADCH_R)) //Цель ПРАВЕЕ
		{
			fsm_Go_state = 3;
			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
		}
				
		else if((ADCH_L < K_ADCH_L) && (ADCH_R < K_ADCH_R))//Цель ПОТЕРЕНА по фронту
		{
			fsm_Go_state = 4;
			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
		}
		else
		{
			Start_GTimer (Motion_Timer);
			if((Get_GTimer(Motion_Timer) > T_ROTATION) && ((Get_GTimer(RRLL_Timer) >= T_FRONT_CONTACT) || (Get_GTimer(RRLL_Timer) == 0)))
			{
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				fsm_Go_state = 0;
				SendMessage(MSG_TORNADO_L);
			}
		}

		if(PIN_C < 3)//Контроль края ринга
		{
            
            DC_motors(STOP,0,0);

			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
			fsm_Go_state = 6;
		}

	break;

	case 3:	//Правый поворот к цели

        DC_motors(RIGHT, SPEED_F, SPEED_F-FORWARD_R);

		//----------------Проверка событий-------------------------------------------------------------------------

		if((ADCH_L>K_ADCH_L)&&(ADCH_R>K_ADCH_R)) //Цель ПРЯМО по курсу
			{
			fsm_Go_state=1;
			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
			LED_ON;
			}
			else if ((ADCH_R>=K_FRONT_DIST) || (ADCH_L>=K_FRONT_DIST)) //Цель на ковше, но смещена ВПРАВО
				{
				Start_GTimer (RRLL_Timer); //Подсчёт длительности фронтального контакта.
				if(Get_GTimer(RRLL_Timer)>T_FRONT_CONTACT)
					{
					fsm_Go_state=7;
					Stop_GTimer(Motion_Timer);
					Stop_GTimer(RRLL_Timer);
					LED_ON;
					}
				}
			else if((ADCH_L>K_ADCH_L)&&(ADCH_R<K_ADCH_R)) //Цель ЛЕВЕЕ
				{
				fsm_Go_state=2;
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				}
			else if((ADCH_L<K_ADCH_L) && (ADCH_R<K_ADCH_R))//Цель ПОТЕРЕНА по фронту
				{
				fsm_Go_state=5;
				Stop_GTimer(Motion_Timer);
				Stop_GTimer(RRLL_Timer);
				}
			else
				{
				Start_GTimer (Motion_Timer);
				if((Get_GTimer(Motion_Timer)>T_ROTATION) && ((Get_GTimer(RRLL_Timer)>=T_FRONT_CONTACT) || (Get_GTimer(RRLL_Timer)==0)))
					{
					Stop_GTimer(Motion_Timer);
					Stop_GTimer(RRLL_Timer);
					fsm_Go_state=0;
					SendMessage(MSG_TORNADO_R);
					}
				}

		if(PIN_C<3)//Контроль края ринга
			{
                
            DC_motors(STOP,0,0);

			Stop_GTimer(Motion_Timer);
			Stop_GTimer(RRLL_Timer);
			fsm_Go_state=6;
			}

	break;

	case 4: //Цель находится СИЛЬНО СЛЕВА. Резкий доворот к цели.
            
        DC_motors(TORNADO_LEFT, SPEED_GO_ROTATION, SPEED_GO_ROTATION);

		//----------------Проверка событий-------------------------------------------------------------------------
		if(ADCH_LL >= K_RR_LL)
		{
			fsm_Go_state = 0;
			SendMessage(MSG_TORNADO_L);
		}
		else if((ADCH_L > K_ADCH_L) && (ADCH_R < K_ADCH_R)) //Цель ЛЕВЕЕ
		{
			fsm_Go_state = 2;
		}
		else if((ADCH_L > K_ADCH_L) && (ADCH_R > K_ADCH_R)) //Цель ПРЯМО по курсу
		{
			fsm_Go_state = 1;
			LED_ON;
		}
		else if((ADCH_L < K_ADCH_L) && (ADCH_R > K_ADCH_R)) //Цель ПРАВЕЕ
		{
			fsm_Go_state = 3;
		}

		if(PIN_C<3)//Контроль края ринга
		{
            
			fsm_Go_state=6;
		}		

	break;

	case 5: //Цель находится СИЛЬНО СПРАВА. Резкий доворот к цели.
            
        DC_motors(TORNADO_RIGHT, SPEED_GO_ROTATION, SPEED_GO_ROTATION);

		//----------------Проверка событий-------------------------------------------------------------------------
		if(ADCH_RR>=K_RR_LL)
			{
			fsm_Go_state=0;
			SendMessage(MSG_TORNADO_R);
			}
			else if((ADCH_L<K_ADCH_L)&&(ADCH_R>K_ADCH_R)) //Цель ПРАВЕЕ
				{
				fsm_Go_state=3;
				}
			else if((ADCH_L>K_ADCH_L)&&(ADCH_R>K_ADCH_R)) //Цель ПРЯМО по курсу
				{
				fsm_Go_state=1;
				LED_ON;
				}
			else if((ADCH_L>K_ADCH_L)&&(ADCH_R<K_ADCH_R)) //Цель ЛЕВЕЕ
				{
				fsm_Go_state=2;
				}

		if(PIN_C<3)//Контроль края ринга
			{
			PORTB &= ~(1<<IN1); 				//Делаем STOP на оба двигателя
			PORTD &= ~(1<<IN2|1<<IN3|1<<IN4);

			fsm_Go_state=6;
			}

	break;

	case 6: //Уход от края ринга
		if(PIN_C==1)	//Линия слева. Уходим назад и вправо.
			{
				Start_GTimer (Motion_Timer);			//Делаем задержку для плавной остановки двигателей
				if(Get_GTimer(Motion_Timer)>T_REVERS)	//и дальнейшего их пуска
					{
					PORTB |= 1<<IN1;
					PORTD |= (0<<IN2|1<<IN3|0<<IN4);

					OCR1A=SPEED_B-BACK_R;
					OCR1B=SPEED_B;	//регистр ШИМ на левый двигатель

					Stop_GTimer(Motion_Timer);	
					}
			}
			else if(PIN_C==2) //Линия справа. Уходим назад и влево.
				{
					Start_GTimer (Motion_Timer);			//Делаем задержку для плавной остановки двигателей
					if(Get_GTimer(Motion_Timer)>T_REVERS)	//и дальнейшего их пуска
						{
						PORTB |= 1<<IN1;
						PORTD |= (0<<IN2|1<<IN3|0<<IN4);

						OCR1A=SPEED_B;
						OCR1B=SPEED_B-BACK_L;	//регистр ШИМ на левый двигатель

						Stop_GTimer(Motion_Timer);	
						}	
				}
			else if(PIN_C==0) //Линия спереди. Уходим назад.
				{
					Start_GTimer (Motion_Timer);			//Делаем задержку для плавной остановки двигателей
					if(Get_GTimer(Motion_Timer)>T_REVERS)	//и дальнейшего их пуска
						{
                        
                        DC_motors(BACK,SPEED_B1,SPEED_B1);

						Stop_GTimer(Motion_Timer);	
						}	
				}

		Start_GTimer (Go_Timer);
		if((Get_GTimer(Go_Timer) > T_B) && (PIN_C > 2))
			{
			//Надо будет делать переход в режим ПЕРИМЕТР-----------------------------------------------------------------------------!!!
			//Пока временно поставлю переход в ТОРНАДО ПРАВЫЙ
			fsm_Go_state=0;
			SendMessage(MSG_TORNADO_R);
			Stop_GTimer(Go_Timer);	
			Stop_GTimer(Motion_Timer);
			}

		else if ((Get_GTimer(Go_Timer) > T_B) && (PIN_C < 3)) 
			{
            
            DC_motors(STOP,0,0);

			fsm_Go_state = 0;
			Stop_GTimer(Go_Timer);
			Stop_GTimer(Motion_Timer);
			menu_state = 1;						//Активация меню
			start_state = 1;					//Активация FSM Start
			}

	break;

	case 7: //Боевой контакт
            DC_motors(FORWARD, SPEED_FAST, SPEED_FAST);

		//----------------Проверка событий-------------------------------------------------------------------------
			
			if ((ADCH_L<K_ADCH_L) && (ADCH_R<K_ADCH_R)) //Потеря цели по фронту
				{
                
                    DC_motors(STOP,0,0);
                    
				LED_OFF;
				fsm_Go_state=0;
				SendMessage(MSG_TORNADO_R);
				}

			else if((PIN_C<3) && (ADCH_L<K_FRONT_DIST) && (ADCH_R<K_FRONT_DIST))//Контроль края ринга
			{
            
                DC_motors(STOP,0,0);


			LED_OFF;
			fsm_Go_state=6;
			}
	break;
	}	

sei();
}
//Написать легкую параметризацию смены черного ринга на белый
