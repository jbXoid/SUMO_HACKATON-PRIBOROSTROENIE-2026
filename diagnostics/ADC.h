#ifndef _ADC
#define _ADC

#if defined (__AVR_ATmega168__)||(__AVR_ATmega168P__)

//AVCC,ADC4/ADC5,выравнивание по левой границе
#define ADC_INIT_R {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|1<<MUX2|0<<MUX1|1<<MUX0);} //PC5 - правый датчик расстояния
#define ADC_INIT_L {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|1<<MUX2|0<<MUX1|0<<MUX0);} //PC4 - левый датчик расстояния
#define ADC_INIT_RR {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|0<<MUX2|1<<MUX1|1<<MUX0);} //PC3 - правый датчик расстояния
#define ADC_INIT_LL {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|0<<MUX2|1<<MUX1|0<<MUX0);} //PC2 - правый датчик расстояния
#define ADC_INIT_RESET {ADMUX &= ~ (1<<REFS1|1<<REFS0|1<<ADLAR|1<<MUX3|1<<MUX2|1<<MUX1|1<<MUX0);}

#define ADC_INIT_R {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|1<<MUX2|0<<MUX1|1<<MUX0);} //PC5 - їїїїїї їїїїїї їїїїїїїїїї
#define ADC_INIT_L {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|1<<MUX2|0<<MUX1|0<<MUX0);} //PC4 - їїїїї їїїїїї їїїїїїїїїї
#define ADC_INIT_RR {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|0<<MUX2|1<<MUX1|1<<MUX0);} //PC3 - їїїїїї їїїїїї їїїїїїїїїї
#define ADC_INIT_LL {ADMUX |= (0<<REFS1|1<<REFS0|1<<ADLAR|0<<MUX3|0<<MUX2|1<<MUX1|0<<MUX0);} //PC2 - їїїїїї їїїїїї їїїїїїїїїї
#define ADC_INIT_RESET {ADMUX &= ~ (1<<REFS1|1<<REFS0|1<<ADLAR|1<<MUX3|1<<MUX2|1<<MUX1|1<<MUX0);}



//предделитель на 128, прерывания запрещены, непрерывное преобразование
#define ADC_ON {ADCSRA |= (1<<ADEN|1<<ADSC|1<<ADATE|0<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0);}


#endif //__AVR_ATmega168__ __AVR_ATmega168P__
//-----------------------------------------------------------------------
// Описание глобальных функций
//-----------------------------------------------------------------------

extern void init_ADC (void);

#endif //_ADC
