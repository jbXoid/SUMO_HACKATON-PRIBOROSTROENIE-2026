void DC_Motors (uint8_t Action, uint8_t Speed_L, uint8_t Speed_R)
{
cli();
	static uint8_t status;	
	static uint8_t stat_rev_old;
	uint8_t stat_rev_new=0;
	
	if (status == Action)//Ещё надо сравнить изменения скоростей. И если скорости изменились, а действие прежнее, то функция продолжает работу.
		return;
	
	if ((Action == 1) || ((Action == 3) || (Action == 4))
		stat_rev_new=1;
	else if ((Action == 2) || ((Action == 8) || (Action == 9))
		stat_rev_new=2;
	else if (Action == 6)
		stat_rev_new=3;
	else if (Action == 7)
		stat_rev_new=4;
	
	if (stat_rev_old != stat_rev_new)
	{
		//Как реализовать задержку в функции??? Только средствами RTOS.
	stat_rev_old = stat_rev_new;
	status = Action;
		
sei();	
}
//В рамках организации программы через автоматы FSM не представляется возможным создать функцию DC_Motors с возможностью паузы на момент реверса.