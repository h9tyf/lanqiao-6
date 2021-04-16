#include "var.h"
#include "onewire.h"
#include "ds1302.h"

long SysTick;

u8 digital_tube[8] = {1, 2, 3, 4, 5, 6, 7, 8};
u8 tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf};
							//0			1			2			3			4			5			6			7			8			9		OFF		-
enum SHOW show_state = PARA;

long temperature[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 t_count;
u8 interval = 1;

u8 hour;
u8 min;
u8 seconds;

long start_time;

void Timer0Init(void)		//2??@11.0592MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x9A;		//??????
	TH0 = 0xA9;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
}



void LatchControl(u8 num,u8 value)
{
	P0 = value;
	_nop_();
	P2 = (num << 5) | (P2 & 0x1f);
	_nop_();
	P2 &= 0x1f;
	_nop_();
}

u8 index;
void func() interrupt 1
{
	SysTick++;
	LatchControl(7, tab[OFF]);
	LatchControl(6, 1<<index);
	LatchControl(7, tab[digital_tube[index]]);
	index = (index + 1) % 8;
}

long time_to_num()
{
	long temp = 0;
	temp += hour * 60L * 60L;
	temp += min * 60L;
	temp += seconds;
	return temp;
}

void rd_temperature()
{
	char temp;
	u8 low, high;
	////

	
	if(show_state == CLOCK){
		if(time_to_num() == (t_count * interval + start_time) % (24L * 60L * 60L)){
				Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay_OneWire(200);
	
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);

	low = Read_DS18B20();
	high = Read_DS18B20();
	
	temp = high<<4;
  temp |= (low>>4);
			temperature[t_count] = temp;
			t_count++;
		}
		
	}
}

u8 bcd2res(u8 bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0f);
}

void rd_time()
{
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	Write_Ds1302_Byte(0xbf); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	seconds = bcd2res(Read_Ds1302_Byte()); /*��DS1302�ж���һ���ֽڵ�����*/	
	min = bcd2res(Read_Ds1302_Byte());
	hour = bcd2res(Read_Ds1302_Byte());	
	RST_CLR;	/*ֹͣDS1302����*/
}

void change_state()
{
	if(t_count == 10 && show_state == CLOCK){
		show_state = SHOW_T;
		t_index = 0;
	}	
	check_button();
	respond_to_button();
}

void main()
{
	Timer0Init();
	EA = 1;
	ET0 = 1;
	P3 |= 0x0f;
	LatchControl(4, 0xff);
	Ds1302_Single_Byte_Write(0x84, 0x23);
	Ds1302_Single_Byte_Write(0x82, 0x59);
	Ds1302_Single_Byte_Write(0x80, 0x50);
	while(1)
	{
		long tickBkp = SysTick;
		if(tickBkp % 200 == 0){
			rd_temperature();
		}
		
		
		if(tickBkp % 100 == 0){
			EA = 0;
			rd_time();
			EA = 1;
		}
		
		if(tickBkp % 10 == 0){
			EA = 0;
			change_state();
			change_show();
			EA = 1;
		}
		while(tickBkp == SysTick);
	}
}