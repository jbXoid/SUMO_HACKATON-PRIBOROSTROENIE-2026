#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#define MAX_MESSAGES 12

#define MSG_Go_Stop         0
#define MSG_Go_F       		1
#define MSG_Go_R       		2
#define MSG_Go_L       		3
#define MSG_START_L    		4
#define MSG_START_R    		5
#define MSG_TORNADO_R  		6
#define MSG_TORNADO_L  		7
#define MSG_START_SLOW		8
#define MSG_START_FAST		9
#define MSG_Perimeter_Start 10


extern uint8_t CodeMessages[MAX_MESSAGES];

extern void InitMessages(void);
extern void SendMessage(uint8_t msg);
extern uint8_t GetMessage(uint8_t msg);
extern uint8_t GetCodeMessages(uint8_t msg);
extern void ProcessMessages(void);





#endif //_MESSAGES_H_
