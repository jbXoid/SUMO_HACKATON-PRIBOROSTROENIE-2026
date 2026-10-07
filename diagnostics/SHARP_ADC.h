#ifndef SHARP_ADC_h
#define SHARP_ADC_h
//-------------------Параметры------------------------------------------------
#define T_ADCH		1 // Интервал между выборками из АЦП ms.
#define N_SAMPLE	8 // Количестов сэмплов для расчёта среднего значения
#define DIVISOR     3 // Поразрядный сдвиг в право bin: 1000 - Деление на 8
//----------------------------------------------------------------------------

extern uint8_t ADCH_R; // Усреднённое значение АЦП с правого бокового датчика SHARP
extern uint8_t ADCH_L; // Усреднённое значение АЦП с левого бокового датчика SHARP
extern uint8_t ADCH_RR; // Усреднённое значение АЦП с правого переднего датчика SHARP
extern uint8_t ADCH_LL; // Усреднённое значение АЦП с левого переднего датчика SHARP

extern void InitFSM_SHARP_ADC (void);
extern void ProcessFSM_SHARP_ADC (void);

#endif
