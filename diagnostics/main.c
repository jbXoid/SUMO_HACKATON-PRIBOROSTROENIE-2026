#include "main.h"

void init_port(void);

typedef enum {
    NONE, SHARP_TEST, MOTOR_TEST, LINE_SENSOR_TEST
} Mode; 

int main (void)	
{
    
    init_port();
    init_USART();
    Init_GTimer();
    init_timer1();

    Mode diag_mode = NONE;

    const char begin_text[] = "DIAGNOSTICS. PRESS A TO SWITCH THROUGH MODES: SHARP_TEST, MOTOR_TEST, LINE_SENSOR\r\n";
    send_text_uart( (unsigned char *)begin_text, sizeof(begin_text) - 1 );

    uint8_t Prev_Pin_D = PIND;
    uint8_t Pin_D = PIND;


    bool motor_direction = 0;
    bool motor_speed_rising = 0;

    while(1) {

        // Checking buttons
        Pin_D = PIND;
        if ( ( Prev_Pin_D & BUTTON_A ) && !( Pin_D & BUTTON_A ) ) {
            
            diag_mode = (Mode)((diag_mode + 1) % 4);

            char press_text[23];
            int len = snprintf(press_text, sizeof(press_text),
                                "SWITCHING MODE TO %d\r\n",
                                (int)diag_mode
                    );

            send_text_uart( (unsigned char *)press_text, (uint16_t)len );

            Start_GTimer(10);


        }
        Prev_Pin_D = Pin_D;

        // Modes
        switch ( diag_mode ) {
            
            default: {
                break;
            }

            case SHARP_TEST: 
            
                if ( Get_GTimer(10) >= 500 ) {
                        
                    // Processing SHARP
                    ProcessFSM_SHARP_ADC();

                    char debug_text[100];
                    int len = snprintf(debug_text, sizeof(debug_text),
                                       "LEFT: %03u\r\n"
                                       "FORWARD LEFT: %03u\r\n"
                                       "FORWARD RIGHT: %03u\r\n"
                                       "RIGHT: %03u\r\n"
                                       "\r\n",
                                       (unsigned int)ADCH_LL,
                                       (unsigned int)ADCH_L,
                                       (unsigned int)ADCH_R,
                                       (unsigned int)ADCH_RR);

                    if (len > 0 && (size_t)len < sizeof debug_text) {
                        send_text_uart((unsigned char *)debug_text, (uint16_t)len);
                    }
                   
                    Stop_GTimer(10);
                    Start_GTimer(10);
                }

                break;

            case MOTOR_TEST:
                
                if (Get_GTimer(10) >= 2 ) {



                    if( motor_speed_rising ) {

                        if ( OCR1A == 150 && OCR1B == 150) {
                            
                            motor_speed_rising = 0;

                        }

                    }

                    else {

                        if( OCR1A == 0 && OCR1B == 0 ) {

                            motor_speed_rising = 1;
                            motor_direction = !motor_direction;

                        }

                    }

                    
                    switch( motor_speed_rising ) {

                        case 0:

                            OCR1A --;
                            OCR1B --;

                            break;

                        case 1:

                            OCR1A ++;
                            OCR1B ++;

                            break;

                    }


                    switch( motor_direction ) {

                        case 0:

                            PORTD |= (1<<7);
                            PORTB &= ~(1<<0);

                            PORTD |= (1<<6);
                            PORTD &= ~(1<<4);

                            break;

                        case 1: 
                            
                            PORTD &= ~(1<<7);
                            PORTB |= (1<<0);

                            PORTD |= (1<<4);
                            PORTD &= ~(1<<6);

                            break;

                    }            


                    Stop_GTimer(10);
                    Start_GTimer(10);

                }
                break;

            case LINE_SENSOR_TEST:

                if ( Get_GTimer(10) >= 500 ) {
                    
                    char debug_text[100];

                    int len = snprintf( debug_text, sizeof(debug_text),
                            "FIRST SENSOR: %d\r\n"
                            "SECOND SENSOR: %d\r\n"
                            "\r\n",
                            (PINC & (PINC & _BV(PC0)) != 0),
                            (PINC & (PINC & _BV(PC1)) != 0)
                            );

                    if (len > 0 && (size_t)len < sizeof debug_text) {
                        send_text_uart((unsigned char *)debug_text, (uint16_t)len);
                    }
                   
                    Stop_GTimer(10);
                    Start_GTimer(10);
                }

        }

    }
}

void init_port(void)
{
	DDRB |= (1<<ENA|1<<ENB|1<<IN1);	//Управление двигателями
	DDRD |= (1<<IN2|1<<IN3|1<<IN4|0<<BUTTOM_L|0<<BUTTOM_R); //Управление двигателями - порты на выход; кнопки PD3,PD2 - порты на вход.
	PORTD |= (1<<BUTTOM_L|1<<BUTTOM_R); //Кнопки PD3,PD2 - порты в режиме PullUp с подтяжкой к 1.

	DDRC |= (0<<DDC5|0<<DDC4|0<<DDC3|0<<DDC2|0<<DDC1|0<<DDC0); //Входы (инфракрасные датчики и датчик расстояния) в режиме Hi-Z

	ADC_INIT_R; //инициализация АЦП. Читаем сигнал с порта PC5 (правый датчик). Данные берем с ADCH. Опорное-5V(255). Сигнал до 3V(153).
	ADC_ON; //запуск АЦП.
}
