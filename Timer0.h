//TIMER0_ON
//TIMER0_OFF
//TIMER0_OVF_INT_ENABLE - Прерывание по переполнению рег.TCNT0
//TIMER0_OUTPUT_COMP_A_INT_ENABLE - Прерывание по совпадению с рег.OCR0A
//TIMER0_OUTPUT_COMP_B_INT_ENABLE - Прерывание по совпадению с рег.OCR0A
//T0_NORMAL_ALL
//T0_NORMAL_OC0A
//T0_NORMAL_OC0B
//T0_CTC_TOGGLE_OC0A
//T0_CTC_CLEAR_OC0A
//T0_CTC_SET_OC0A
//и т.д. См. ниже.
//---------------------------
//#ifdef CK0_8 и т.д.
//#define OC0A 6
//#define OC0B 5
//************************************************************************
//Доработать блок подсчета внешних импульсов (дописать настройку портов)
//************************************************************************
//Изменение предделителя выполнять через остановку таймера!!!!!!!!!!!!!!!!!

#ifndef _TIMER0_H
#define _TIMER0_H

#if defined (__AVR_ATmega168__)||(__AVR_ATmega168P__)

#ifdef CK0_1
#define TIMER0_ON {TCCR0B |= (0<<CS02|0<<CS01|1<<CS00);}
#endif

#ifdef CK0_8
#define TIMER0_ON {TCCR0B |= (0<<CS02|1<<CS01|0<<CS00);}
#endif

#ifdef CK0_64
#define TIMER0_ON {TCCR0B |= (0<<CS02|1<<CS01|1<<CS00);}
#endif

#ifdef CK0_256
#define TIMER0_ON {TCCR0B |= (1<<CS02|0<<CS01|0<<CS00);}
#endif

#ifdef CK0_1024
#define TIMER0_ON {TCCR0B |= (1<<CS02|0<<CS01|1<<CS00);}
#endif

#ifdef CK0_EXT_FALLING
#define TIMER0_ON {TCCR0B |= (1<<CS02|1<<CS01|0<<CS00);}
#endif

#ifdef CK0_EXT_RISING
#define TIMER0_ON {TCCR0B |= (1<<CS02|1<<CS01|1<<CS00);}
#endif

#define TIMER0_OFF {TCCR0B = 0; TCNT0 = 0;}

//---------Прерывания----------------------------------------------------

#define TIMER0_OVF_INT_ENABLE {TIMSK0 |= 1<<TOIE0;}

#define TIMER0_OUTPUT_COMP_B_INT_ENABLE {TIMSK0 |= 1<<OCIE0B;}

#define TIMER0_OUTPUT_COMP_A_INT_ENABLE {TIMSK0 |= 1<<OCIE0A;}

//--------Настройка ШИМ--------------------------------------------------
//Переключение между режимами только через T0_NORMAL_ALL
//Переключение в режиме только через T0_NORMAL_OC0A или T0_NORMAL_OC0B

#define OC0A 6
#define OC0B 5

#define T0_NORMAL_ALL {TCCR0A = 0; TCCR0B &=~(1<<WGM02);}

#define T0_NORMAL_OC0A {TCCR0A &=~(1<<COM0A1|1<<COM0A0);}
#define T0_NORMAL_OC0B {TCCR0A &=~(1<<COM0B1|1<<COM0B0);}

//                                   CTC

#define T0_CTC_TOGGLE_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (0<<COM0A1|1<<COM0A0|1<<WGM01);}
#define T0_CTC_CLEAR_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|0<<COM0A0|1<<WGM01);}
#define T0_CTC_SET_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|1<<COM0A0|1<<WGM01);}

#define T0_CTC_TOGGLE_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (0<<COM0B1|1<<COM0B0|1<<WGM01);}
#define T0_CTC_CLEAR_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|0<<COM0B0|1<<WGM01);}
#define T0_CTC_SET_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|1<<COM0B0|1<<WGM01);}

//                                   Fast_PWM

#define T0_Fast_TOGGLE_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (0<<COM0A1|1<<COM0A0|1<<WGM01|1<<WGM00); TCCR0B |= 1<<WGM02;}
#define T0_Fast_CLEAR_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|0<<COM0A0|1<<WGM01|1<<WGM00);}
#define T0_Fast_SET_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|1<<COM0A0|1<<WGM01|1<<WGM00);}

#define T0_Fast_CLEAR_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|0<<COM0B0|1<<WGM01|1<<WGM00);}
#define T0_Fast_SET_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|1<<COM0B0|1<<WGM01|1<<WGM00);}

//                                   Phase_Correct_PWM

#define T0_Phase_TOGGLE_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (0<<COM0A1|1<<COM0A0|1<<WGM00); TCCR0B |= 1<<WGM02;}
#define T0_Phase_CLEAR_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|0<<COM0A0|1<<WGM00);}
#define T0_Phase_SET_OC0A {DDRD |= 1<<OC0A; TCCR0A |= (1<<COM0A1|1<<COM0A0|1<<WGM00);}

#define T0_Phase_CLEAR_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|0<<COM0B0|1<<WGM00);}
#define T0_Phase_SET_OC0B {DDRD |= 1<<OC0B; TCCR0A |= (1<<COM0B1|1<<COM0B0|1<<WGM00);}

#endif //__AVR_ATmega168__








#endif //_TIMER0_H
