#include "main.h"

uint8_t ADCH_R;
uint8_t ADCH_L;
uint8_t ADCH_RR;
uint8_t ADCH_LL;

uint8_t Buff[N_SAMPLE] = {0}; // Массив для хранения мгновенных значений АЦП
uint8_t *pointBuff = Buff;

void InitFSM_SHARP_ADC (void)
{
	ADCH_L = 0;
	ADCH_R = 0;
	ADCH_LL = 0;
	ADCH_RR = 0;
}	

void ProcessFSM_SHARP_ADC (void)
{
static uint8_t mov_buff = 0; // Счётчик перемещений по Buff
static uint8_t k = 0; // переменная подсостояний выбора канала АЦП (PC2,PC3,PC4,PC5)
static uint16_t Sum = 0;

cli();
	Start_GTimer (ADCH_Timer); //Фильтр АЦП. Делаем несколько выборок с временным интервалом и считаем среднее значение (деление на 5 сдвигом вправо на два разряда).
	if (Get_GTimer(ADCH_Timer) > T_ADCH)
	{
			// Размещение значений АЦП в буфер Buff каждые T_ADCH [ms]
/*			if (mov_buff < N_SAMPLE)
			{
				*pointBuff = ADCH;
				pointBuff++;			
				Stop_GTimer(ADCH_Timer);	
				mov_buff++;
			}
			else // Если буфер заполнился
			{
				pointBuff--;
				for (int i = 0; i < N_SAMPLE; i++)
				{
					Sum += *pointBuff;
					pointBuff--;
				}
				Sum >>= DIVISOR; // Деление только на степень двойки. Результат max ~153
				pointBuff = Buff;
*/
		Sum = ADCH;
				switch (k)
				{
					case 0:
						ADCH_R = Sum; // Нужно ли сделать преобразование типа?
						Sum = 0;
						k = 1;
						mov_buff = 0;

						ADC_INIT_RESET;
						ADC_INIT_L;
						Stop_GTimer(ADCH_Timer);
					break;
						
					case 1:
						ADCH_L = Sum; // Нужно ли сделать преобразование типа?
						Sum = 0;
						k = 2;
						mov_buff = 0;

						ADC_INIT_RESET;
						ADC_INIT_LL;
						Stop_GTimer(ADCH_Timer);
					break;

					case 2:
						ADCH_LL = Sum; // Нужно ли сделать преобразование типа?
						Sum = 0;
						k = 3;
						mov_buff = 0;


						ADC_INIT_RESET;
						ADC_INIT_RR;
						Stop_GTimer(ADCH_Timer);
					break;

					case 3:
						ADCH_RR = Sum; // Нужно ли сделать преобразование типа?
						Sum = 0;
						k = 0;
						mov_buff = 0;

						ADC_INIT_RESET;
						ADC_INIT_R;
						Stop_GTimer(ADCH_Timer);
					break;
				}
//			}
	}
sei();
}

