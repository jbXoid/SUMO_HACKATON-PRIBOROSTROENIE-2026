#ifndef _USART_H
#define _USART_H

#define	T_USART	250 //Период передачи данных по USART [ms]

#if defined (__AVR_ATmega168__)||(__AVR_ATmega168P__)
//Настройка скорости передачи данных
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)
//Прерывания по приему разерешены, прием и передача включены, 8bits mode (UCSZn2,UCSZn1,UCSZn0)
#define USART_RXTX_IE_ON {UCSR0B |= (1<<RXCIE0|0<<TXCIE0|0<<UDRIE0|1<<RXEN0|1<<TXEN0|0<<UCSZ02|0<<RXB80|0<<TXB80);}
//Asynchronous USART (UMSEL01;UMSEL00)(UPM01;UPM00), проверка чётности отключена (UPMn1;UPM00), 1 stop bit (USBS0),
//8bits mode (UCSZ02,UCSZ01,UCSZ00)
#define USART_CONTROL_INIT {UCSR0C |= (0<<UMSEL01|0<<UMSEL00|0<<UPM01|0<<UPM00|0<<USBS0|1<<UCSZ01|1<<UCSZ00|0<<UCPOL0);}

#endif //__AVR_ATmega168__ __AVR_ATmega168P__

//-----------------------------------------------------------------------
// Описание глобальных переменных
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Описание глобальных функций
//-----------------------------------------------------------------------
extern void init_USART (void);
extern void ProcessFSM_USART_TX (void);
#endif //_USART_H
