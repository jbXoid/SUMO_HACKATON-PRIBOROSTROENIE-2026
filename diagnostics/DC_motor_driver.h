//Action parameter
//REMADE: enum will be better for this
/*
#define FORWARD			1
#define BACK			2
#define LEFT			3
#define RIGHT			4
#define STOP			5
#define TORNADO_LEFT 	6
#define TORNADO_RIGHT	7
*/

enum Action {
    FORWARD, BACK, LEFT, RIGHT, STOP, TORNADO_LEFT, TORNADO_RIGHT
};

extern void DC_Motors (enum Action action, uint8_t Speed);
