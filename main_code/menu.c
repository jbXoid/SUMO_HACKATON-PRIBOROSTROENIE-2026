#include "main.h"
//-----------Global-------------------------------------------------------------------------------------------------
uint8_t menu_state=1;
//------------------------------------------------------------------------------------------------------------------
static uint8_t fsm_MENU_state; ////переменная автомата ProcessFSM_MENU
static uint8_t tactics_step;//переключатель режимов
static uint8_t tactics_mode;//режим входа в ринг при старте
static uint8_t a_case; //переключает case после нажатия кнопки и мигания диодом
static uint8_t Pin_D; //переменная в которой остануться биты (PD2,PD3)кнопок.

static uint8_t i;
static uint8_t n; //Счётчик количества миганий диода
static uint8_t led_step; //Переменная задающая количество миганий диода

void Start (uint8_t *, uint8_t );

void InitFSM_MENU (void)
{
	fsm_MENU_state = 0;
	tactics_step = 0;
	tactics_mode = TACTIC_MODE_FAST_STEP;
	a_case = 2;
	led_step = 2;
	i = 1;
}

void ProcessFSM_MENU (void)
{
cli();
	Start_GTimer (Buttons_Timer); //выдержка времени Sensor_Timer для защиты от "дребезга" кнопок
	if (Get_GTimer(Buttons_Timer) > T_BUTTONS)
		{
		Pin_D = PIND;	
		Pin_D = Pin_D & 12;//Обнуляю разряды 0,1,4-7. Смотрим кнопки с PD2,PD3.

		Stop_GTimer(Buttons_Timer);
		}

	switch (fsm_MENU_state)
	{
	case 0://Мигание синим диодом 2 раза

		if (i == 1)
			{
			LED_ON;
			Start_GTimer (Menu_Timer);
			if (Get_GTimer(Menu_Timer) > T1)
				{
				Stop_GTimer(Menu_Timer);
				i = 0;
				n++;
				}
			}
		else
			{
			LED_OFF;
			Start_GTimer (Menu_Timer);
			if (Get_GTimer(Menu_Timer) > T2)
				{
				Stop_GTimer(Menu_Timer);
				i = 1;
				if (n == led_step)
					{
					n = 0;
					switch (a_case)
						{
						case 1:
							fsm_MENU_state = 1;//Переход в кейс старта
						break;

						case 2:
							fsm_MENU_state = 2;//Переход в кейс выбора боевой тактики							
						break;
						}
					
					}
				}
			}		
	break;

	case 1: //Подменю старта
			Start(&tactics_mode, Pin_D);
	break;

	case 2: //Подменю выбора боевых тактик

		if(Pin_D == BUTTON_A) //Нажата кнопка А
				{
				fsm_MENU_state = 0;
				a_case = 2;//Задаю переход (после миганий) в подменю выбора боевых тактик.
				switch (tactics_step)
					{
					case 0:
						led_step = 1;//Задаю одно мигание диодом
						tactics_mode = TACTIC_MODE_FAST_STEP;
						tactics_step = 1;
					break;

					case 1:
						led_step = 2;
						tactics_mode = TACTIC_MODE_SLOW_STEP;
						tactics_step = 2;
					break;

					case 2:
						led_step = 3;
						tactics_mode = TACTIC_MODE_ROTATION;
						tactics_step = 0;
					break;
					}
				Pin_D = 0;
				}
		else if(Pin_D == BUTTON_B) //Нажата кнопка В - подтверждение выбранной тактики
				{
				fsm_MENU_state = 0;
				a_case = 1;//Задаю переход 
				led_step = 2;//Подтверждение ввода тактики - два мигания диодом
				Pin_D = 0;
				}		
	break;
	}
sei();
}

/**************************************************************************************************
* Function Name	: Start
* Description	: Вариант старта при нажатии кнопки А
* Point			: mode - Указатель на переменную варианта выбранной тактики
* Input			: Pin - Параметр состояния кнопок А и В
* Output		: --
* Return		: --
**************************************************************************************************/
void Start (uint8_t *mode, uint8_t Pin)
{
	if ((Pin == BUTTON_A) ||  (Pin == BUTTON_B))
	{
		if(*mode == TACTIC_MODE_SLOW_STEP)
			{
			SendMessage(MSG_START_SLOW);
			}
		else if(*mode == TACTIC_MODE_FAST_STEP)
			{
			SendMessage(MSG_START_FAST);
			}
		else if(*mode == TACTIC_MODE_ROTATION)
			{
			if (Pin == BUTTON_A)
				SendMessage(MSG_START_R);
			else SendMessage(MSG_START_L);
			}
		menu_state = 0;// После этой команды главный цикл main больше не попадёт в меню
		fsm_MENU_state = 0;
		a_case = 2;
		*mode = TACTIC_MODE_FAST_STEP;
	}
}
