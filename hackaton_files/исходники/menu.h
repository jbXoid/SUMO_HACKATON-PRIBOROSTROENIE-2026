#ifndef MENU_h
#define MENU_h
//-------------------ПАРАМЕТРЫ--------------------------------------------------------------------------------
#define T_BUTTONS   6 //время выдержки для защиты от "дребезга" кнопок ms.
#define T1        100 //продолжительность ВКЛ светодиода
#define T2        160 //продолжительность ВЫКЛ светодиода

#define BUTTON_A 8 //Кнопка А
#define BUTTON_B 4 //Кнопка В

#define TACTIC_MODE_SLOW_STEP		0
#define TACTIC_MODE_FAST_STEP		1
#define TACTIC_MODE_ROTATION		2
//------------------------------------------------------------------------------------------------------------
extern uint8_t menu_state;

//typedef void (*

extern void InitFSM_MENU (void);
extern void ProcessFSM_MENU (void);

#endif
