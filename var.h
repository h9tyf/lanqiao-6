
#ifndef  __VAR_H__
#define  __VAR_H__

#include "STC/STC15F2K60S2.H"
#include "intrins.h"

typedef unsigned char u8;

//main
void LatchControl(u8 num, u8 value);
long time_to_num();

#define OFF 10
#define SHOW_A 11
extern u8 digital_tube[8];

enum SHOW{
	PARA = 1, CLOCK, SHOW_T
};

extern enum SHOW show_state;
extern long temperature[10];
extern u8 t_count;
extern u8 interval;

extern u8 hour;
extern u8 min;
extern u8 seconds;

extern long start_time;


//button
void check_button();
void respond_to_button();

extern u8 t_index;

//show
void change_show();

#endif