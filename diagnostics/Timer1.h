//TIMER1_ON
//TIMER1_OFF

//и т.д. См. ниже.
//---------------------------
//#ifdef CK1_8 и т.д.
//#define OC1A 1
//#define OC1B 2
//************************************************************************
//Доработать блок подсчета внешних импульсов (дописать настройку портов)
//************************************************************************
//Изменение предделителя выполнять через остановку таймера!!!!!!!!!!!!!!!!!

#ifndef _TIMER1_H
#define _TIMER1_H

#if defined (__AVR_ATmega168__)||(__AVR_ATmega168P__)

#ifdef CK1_1
#define TIMER1_ON {TCCR1B |= (0<<CS12|0<<CS11|1<<CS10);}
#endif

#ifdef CK1_8
#define TIMER1_ON {TCCR1B |= (0<<CS12|1<<CS11|0<<CS10);}
#endif

#ifdef CK1_64
#define TIMER1_ON {TCCR1B |= (0<<CS12|1<<CS11|1<<CS10);}
#endif

#ifdef CK1_256
#define TIMER1_ON {TCCR1B |= (1<<CS12|0<<CS11|0<<CS10);}
#endif

#ifdef CK1_1024
#define TIMER1_ON {TCCR1B |= (1<<CS12|0<<CS11|1<<CS10);}
#endif

#ifdef CK1_EXT_FALLING
#define TIMER1_ON {TCCR1B |= (1<<CS12|1<<CS11|0<<CS10);}
#endif

#ifdef CK1_EXT_RISING
#define TIMER1_ON {TCCR1B |= (1<<CS12|1<<CS11|1<<CS10);}
#endif

#define TIMER1_OFF {TCCR1B = 0; TCNT1 = 0;}

//---------Прерывания----------------------------------------------------
#define TIMER1_INPUT_CAPTURE_ENABLE {TIMSK1 |= 1<<ICIE1;}

#define TIMER1_OVF_INT_ENABLE {TIMSK1 |= 1<<TOIE1;}

#define TIMER1_OUTPUT_COMP_B_INT_ENABLE {TIMSK1 |= 1<<OCIE1B;}

#define TIMER1_OUTPUT_COMP_A_INT_ENABLE {TIMSK1 |= 1<<OCIE1A;}

//--------Настройка ШИМ--------------------------------------------------
//Переключение между режимами только через T1_NORMAL_ALL
//Переключение в режиме только через T1_NORMAL_OC0A или T1_NORMAL_OC0B
//====================================================================
//Порядок включения ШИМ:
//1.Запуск таймера TIMER1_ON.
//2.Выбор режима ШИМ, например T1_CTC_CLEAR_OC1A на один выход OC1A.
//  Выход устанавливается автоматически в рег.DDR.
//3.Запись значения в регистр сравнения OCR1A.
//====================================================================
#define OC1A 1 //PB1
#define OC1B 2 //PB2

#define T1_NORMAL_ALL {TCCR1A = 0; TCCR1B &=~(1<<WGM13|1<<WGM12);}

#define T1_NORMAL_OC1A {TCCR1A &=~(1<<COM1A1|1<<COM1A0);}
#define T1_NORMAL_OC1B {TCCR1A &=~(1<<COM1B1|1<<COM1B0);}

//                                   Fast_PWM

#define T1_FastPWM_8bit_CLEAR_OC1A {DDRB |= 1<<OC1A; TCCR1A |= (1<<COM1A1|0<<COM1A0|0<<WGM11|1<<WGM10); TCCR1B |= (0<<WGM13|1<<WGM12);}
#define T1_FastPWM_8bit_CLEAR_OC1B {DDRB |= 1<<OC1B; TCCR1A |= (1<<COM1B1|0<<COM1B0|0<<WGM11|1<<WGM10); TCCR1B |= (0<<WGM13|1<<WGM12);}

#define T1_FastPWM_9bit_CLEAR_OC1A {DDRB |= 1<<OC1A; TCCR1A |= (1<<COM1A1|0<<COM1A0|1<<WGM11|0<<WGM10); TCCR1B |= (0<<WGM13|1<<WGM12);}
#define T1_FastPWM_9bit_CLEAR_OC1B {DDRB |= 1<<OC1B; TCCR1A |= (1<<COM1B1|0<<COM1B0|1<<WGM11|0<<WGM10); TCCR1B |= (0<<WGM13|1<<WGM12);}

#endif //__AVR_ATmega168__
//-----------------------------------------------------------------------
// Описание глобальных функций
//-----------------------------------------------------------------------

extern void init_timer1 (void);

#endif //_TIMER1_H
