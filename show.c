#include "var.h"

void change_show()
{
	if(show_state == PARA){
		u8 i;
		for(i = 0; i < 5; i++){
			digital_tube[i] = OFF;
		}
		digital_tube[5] = SHOW_A;
		digital_tube[6] = interval / 10;
		digital_tube[7] = interval % 10;
	} else if(show_state == CLOCK){
		digital_tube[0] = hour / 10;
		digital_tube[1] = hour % 10;
		
		digital_tube[3] = min / 10;
		digital_tube[4] = min % 10;
		
		digital_tube[6] = seconds / 10;
		digital_tube[7] = seconds % 10;
		if(seconds % 2 == 0){
			digital_tube[2] = SHOW_A;
			digital_tube[5] = SHOW_A;
		} else {
			digital_tube[2] = OFF;
			digital_tube[5] = OFF;
		}
	} else {
		digital_tube[0] = SHOW_A;
		digital_tube[1] = t_index / 10;
		digital_tube[2] = t_index % 10;
		digital_tube[3] = OFF;
		digital_tube[4] = OFF;
		digital_tube[5] = SHOW_A;
		digital_tube[6] = (temperature[t_index] / 10) % 10;
		digital_tube[7] = temperature[t_index] % 10;
	}
	
	
	LatchControl(4, ~(1<<(t_count % 8)));
}