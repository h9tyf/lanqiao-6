#include "var.h"

#define S4 0
#define S5 1
#define S6 2
#define S7 3

button_flag[4] = {0, 0, 0, 0};
button_state[4] = {1, 1, 1, 1};
button_check[4] = {1, 1, 1, 1};
button_count[4] = {0, 0, 0, 0};

void check_button()
{
	u8 i;
	button_check[S7] = P30;
	button_check[S6] = P31;
	button_check[S5] = P32;
	button_check[S4] = P33;
	
	for(i = 0; i < 4; i++){
		if(button_check[i] != button_state[i]){
			button_count[i]++;
		} else {
			button_count[i] = 0;
		}
	}
	
	for(i =  0; i < 4; i++){
		if(button_check[i] != button_state[i] && button_count[i] >= 5){
			button_state[i] = 1 - button_state[i];
			if(button_state[i] == 0){
				button_flag[i] = 1;
			}
		}
	}
}

u8 t_index = 0;

void respond_to_button()
{
	if(button_flag[S4] == 1 && show_state == PARA){
		button_flag[S4] = 0;
		if(interval == 1){
			interval = 5;
		} else if(interval == 5){
			interval = 30;
		} else if(interval == 30){
			interval = 60;
		} else {
			interval = 1;
		}
	} 
	if(button_flag[S5] == 1 && show_state == PARA){
		button_flag[S5] = 0;
		show_state = CLOCK;
		t_count = 0;
		start_time = time_to_num();
	}
	if(button_flag[S6] == 1 && show_state == SHOW_T){
		t_index = (t_index + 1) % 10;
		button_flag[S6] = 0;
	}
	if(button_flag[S7] == 1){
		if(show_state == SHOW_T){
			show_state = PARA;
		}
		button_flag[S7] = 0;
	}
	
}
