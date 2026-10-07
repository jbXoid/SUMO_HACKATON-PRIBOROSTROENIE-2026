//TIMER2_ON
//TIMER2_OFF
//TIMER2_OVF_INT_ENABLE - Прерывание по переполнению рег.TCNT0
//TIMER2_OUTPUT_COMP_A_INT_ENABLE - Прерывание по совпадению с рег.OCR0A
//TIMER2_OUTPUT_COMP_B_INT_ENABLE - Прерывание по совпадению с рег.OCR0A
//T2_NORMAL_ALL
//T2_NORMAL_OC2A
//T2_NORMAL_OC2B
//T2_CTC_TOGGLE_OC2A
//T2_CTC_CLEAR_OC2A
//T2_CTC_SET_OC2A
//и т.д. См. ниже.
//---------------------------
//#ifdef CK2_8 и т.д.
//#define OC2A 3
//#define OC2B 3
//************************************************************************
//Доработать блок подсчета внешних импульсов (дописать настройку портов)
//************************************************************************
//Изменение предделителя выполнять через остановку таймера!!!!!!!!!!!!!!!!!

#ifndef _TIMER2_H
#define _TIMER2_H

#define CK2_128 //Задаю предделитель на 128.

#if defined (__AVR_ATmega168__)||(__AVR_ATmega168P__)

#ifdef CK2_1
#define TIMER2_ON {TCCR2B |= (0<<CS22|0<<CS21|1<<CS20);}
#endif

#ifdef CK2_8
#define TIMER2_ON {TCCR2B |= (0<<CS22|1<<CS21|0<<CS20);}
#endif

#ifdef CK2_32
#define TIMER2_ON {TCCR2B |= (0<<CS22|1<<CS21|1<<CS20);}
#endif

#ifdef CK2_64
#define TIMER2_ON {TCCR2B |= (1<<CS22|0<<CS21|0<<CS20);}
#endif

#ifdef CK2_128
#define TIMER2_ON {TCCR2B |= (1<<CS22|0<<CS21|1<<CS20);}
#endif

#ifdef CK2_256
#define TIMER2_ON {TCCR2B |= (1<<CS22|1<<CS21|0<<CS20);}
#endif

#ifdef CK2_1024
#define TIMER2_ON {TCCR2B |= (1<<CS22|1<<CS21|1<<CS20);}
#endif

#define TIMER2_OFF {TCCR2B = 0; TCNT2 = 0;}

//---------Прерывания----------------------------------------------------

#define TIMER2_OVF_INT_ENABLE {TIMSK2 |= 1<<TOIE2;}

#define TIMER2_OUTPUT_COMP_B_INT_ENABLE {TIMSK2 |= 1<<OCIE2B;}

#define TIMER2_OUTPUT_COMP_A_INT_ENABLE {TIMSK2 |= 1<<OCIE2A;}

//--------Настройка ШИМ--------------------------------------------------
//Переключение между режимами только через T2_NORMAL_ALL
//Переключение в режиме только через T2_NORMAL_OC0A или T2_NORMAL_OC0B
//====================================================================
//Порядок включения ШИМ:
//1.Запуск таймера TIMER2_ON.
//2.Выбор режима ШИМ, например T2_CTC_CLEAR_OC2A на один выход OC2A.
//  Выход устанавливается автоматически в рег.DDR.
//3.Запись значения в регистр сравнения OCR2A.
//====================================================================
#define OC2A 3 //PB3
#define OC2B 3 //PD3

#define T2_NORMAL_ALL {TCCR2A = 0; TCCR2B &=~(1<<WGM22);}

#define T2_NORMAL_OC2A {TCCR2A &=~(1<<COM2A1|1<<COM2A0);}
#define T2_NORMAL_OC2B {TCCR2A &=~(1<<COM2B1|1<<COM2B0);}

//                                   CTC

#define T2_CTC_TOGGLE_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (0<<COM2A1|1<<COM2A0|1<<WGM21);}
#define T2_CTC_CLEAR_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|0<<COM2A0|1<<WGM21);}
#define T2_CTC_SET_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|1<<COM2A0|1<<WGM21);}

#define T2_CTC_TOGGLE_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (0<<COM2B1|1<<COM2B0|1<<WGM21);}
#define T2_CTC_CLEAR_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|0<<COM2B0|1<<WGM21);}
#define T2_CTC_SET_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|1<<COM2B0|1<<WGM21);}

//                                   Fast_PWM

#define T2_Fast_TOGGLE_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (0<<COM2A1|1<<COM2A0|1<<WGM21|1<<WGM20); TCCR2B |= 1<<WGM22;}
#define T2_Fast_CLEAR_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|0<<COM2A0|1<<WGM21|1<<WGM20);}
#define T2_Fast_SET_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|1<<COM2A0|1<<WGM21|1<<WGM20);}

#define T2_Fast_CLEAR_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|0<<COM2B0|1<<WGM21|1<<WGM20);}
#define T2_Fast_SET_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|1<<COM2B0|1<<WGM21|1<<WGM20);}

//                                   Phase_Correct_PWM

#define T2_Phase_TOGGLE_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (0<<COM2A1|1<<COM2A0|1<<WGM20); TCCR2B |= 1<<WGM22;}
#define T2_Phase_CLEAR_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|0<<COM2A0|1<<WGM20);}
#define T2_Phase_SET_OC2A {DDRB |= 1<<OC2A; TCCR2A |= (1<<COM2A1|1<<COM2A0|1<<WGM20);}

#define T2_Phase_CLEAR_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|0<<COM2B0|1<<WGM20);}
#define T2_Phase_SET_OC2B {DDRD |= 1<<OC2B; TCCR2A |= (1<<COM2B1|1<<COM2B0|1<<WGM20);}

#endif //__AVR_ATmega168__

//------------------------------------------------------------------------------------------------------------------
#define MAX_GTIMERS 10

#define ms  1
#define sec 1000
#define min 60*sec
#define hour 60*min
#define day 24*hour

#define Go_Timer 		0
#define ADCH_Timer  	1
#define Sensor_Timer 	2
#define Motion_Timer 	3
#define Buttons_Timer 	4
#define Menu_Timer 		5
#define Start_Timer 	6
#define Tornado_Timer 	7
#define RRLL_Timer 		8
#define USART_Timer		9

#define TIMER_STOPPED 0
#define TIMER_RUNNING 1
#define TIMER_PAUSED  2


extern void Init_GTimer (void);
extern void Start_GTimer (unsigned char GTimerID);
extern void Stop_GTimer (unsigned char GTimerID);
extern void Pause_GTimer (unsigned char GTimerID);
extern void Release_GTimer (unsigned char GTimerID);
extern unsigned int Get_GTimer (unsigned char GTimerID);

//------------------------------------------------------------------------------------------------------------------
#endif //_TIMER2_H
