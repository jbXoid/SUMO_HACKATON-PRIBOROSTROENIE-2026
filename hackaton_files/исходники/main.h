#ifndef _TEST_H
#define _TEST_H

#define FK35      //Сборка проекта с параметрами для роботов FK63/FK35/JS400/CHINA300
#define F_CPU 16000000   //Частота ядра
#define CK1_8            //Выбор предделителя таймера_1 на 8. Отвечает за частоту ШИМ управления моторами.
//------------------USART------------------------------------------------------------------
#define USART_ON//Закоментировать, если не используется USART (BAUD 9600/8/1)
//------------------LED--------------------------------------------------------------------
#define LED 5  // Светодиод на плате Arduino Pro Mini. Включен при подтяжке PB5 к 5 В.
#define LED_ON {DDRB |= 1<<LED; PORTB |= 1<<LED;}
#define LED_OFF {DDRB |= 1<<LED; PORTB &= ~(1<<LED);}
//-------------------Buttom----------------------------------------------------------------
#define BUTTOM_R 2
#define BUTTOM_L 3
//--------------------Подключение драйвера двигателей DC-----------------------------------
#define ENA 1 //PB1 ШИМ1
#define IN1 0 //PB0	управление1
#define IN2 7 //PD7 управление1
#define IN3 4 //PD4 управление2
#define IN4 6 //PD6 управление2
#define ENB 2 //PB2 ШИМ2
//-------------------Include---------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>

#include "messages.h"

#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "ADC.h"

#include "FSM_Go.h"
#include "FSM_Perimeter.h"
#include "FSM_Tornado.h"
#include "SHARP_ADC.h"
#include "menu.h"
#include "FSM_Start.h"
#include "EEPROM.h"
#include "USART.h"

#endif //_TEST_H
