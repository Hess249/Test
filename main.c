#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

//==================== 硬件定义 ====================
sbit Buzzer = P2^5;
sbit KEY1  = P3^1;   // 按键1：一闪一闪亮晶晶
sbit KEY2  = P3^0;   // 按键2：天空之城

//==================== 播放参数 ====================
#define SPEED    500

//==================== 音符索引 ====================
#define P      0
#define L1     1
#define L1_    2
#define L2     3
#define L2_    4
#define L3     5
#define L4     6
#define L4_    7
#define L5     8
#define L5_    9
#define L6     10
#define L6_    11
#define L7     12
#define M1     13
#define M1_    14
#define M2     15
#define M2_    16
#define M3     17
#define M4     18
#define M4_    19
#define M5     20
#define M5_    21
#define M6     22
#define M6_    23
#define M7     24
#define H1     25
#define H1_    26
#define H2     27
#define H2_    28
#define H3     29
#define H4     30
#define H4_    31
#define H5     32
#define H5_    33
#define H6     34
#define H6_    35
#define H7     36

//==================== 频率表（12MHz专用） ====================
unsigned int FreqTable[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

//==================== 乐谱1：一闪一闪亮晶晶 ====================
unsigned char code Music_Star[]={
	M1,4,	M1,4,	M5,4,	M5,4,
	M6,4,	M6,4,	M5,8,
	
	M4,4,	M4,4,	M3,4,	M3,4,
	M2,4,	M2,4,	M1,8,
	
	M5,4,	M5,4,	M4,4,	M4,4,
	M3,4,	M3,4,	M2,8,
	
	M5,4,	M5,4,	M4,4,	M4,4,
	M3,4,	M3,4,	M2,8,
	
	M1,4,	M1,4,	M5,4,	M5,4,
	M6,4,	M6,4,	M5,8,
	
	M4,4,	M4,4,	M3,4,	M3,4,
	M2,4,	M2,4,	M1,8,
	
	0xFF
};

//==================== 乐谱2：天空之城 ====================
unsigned char code Music_Sky[]={
	P,4,P,4,P,4,M6,2,M7,2,
	H1,6,M7,2,H1,4,H3,4,
	M7,12,M3,2,M3,2,
	M6,6,M5,2,M6,4,H1,4,
	M5,12,M3,4,
	M4,6,M3,2,M4,4,H1,4,
	M3,8,P,2,H1,2,H1,2,H1,2,
	M7,6,M4_,2,M4_,4,M7,4,
	M7,8,P,4,M6,2,M7,2,
	H1,6,M7,2,H1,4,H3,4,
	M7,12,M3,2,M3,2,
	M6,6,M5,2,M6,4,H1,4,
	M5,12,M2,2,M3,2,
	M4,4,H1,2,M7,4,H1,6,
	H2,2,H2,2,H3,2,H1,10,
	H1,2,M7,2,M6,2,M6,2,M7,4,M5_,4,
	M6,12,H1,2,H2,2,
	H3,6,H2,2,H3,4,H5,4,
	H2,12,M5,2,M5,2,
	H1,6,M7,2,H1,4,H3,4,
	H3,16,
	M6,2,M7,2,H1,4,M7,4,H2,4,H2,2,
	H1,10,M5,6,
	H4,4,H3,4,H3,4,H1,4,
	H3,12,H3,4,
	H6,8,H5,4,H5,4,
	H3,2,H2,2,H1,8,P,2,H1,2,
	H2,4,H1,2,H2,2,H2,4,H5,4,
	H3,12,H3,4,
	H6,8,H5,8,
	H3,2,H2,2,H1,8,P,2,H1,2,
	H2,4,H1,2,H2,6,M7,4,
	M6,12,P,4,
	
	0xFF
};

//==================== 全局变量 ====================
unsigned char FreqSelect, MusicSelect;

//==================== 按键扫描（防抖） ====================
unsigned char KeyScan(void)
{
	if(KEY1 == 0)
	{
		Delay(20);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			return 1;
		}
	}
	if(KEY2 == 0)
	{
		Delay(20);
		if(KEY2 == 0)
		{
			while(KEY2 == 0);
			return 2;
		}
	}
	return 0;
}

//==================== 播放函数 ====================
void PlayMusic(unsigned char code *music)
{
	TR0 = 0;
	MusicSelect = 0;
	
	while(1)
	{
		if(music[MusicSelect] == 0xFF)
		{
			TR0 = 0;
			break;
		}
		
		FreqSelect = music[MusicSelect];
		MusicSelect++;
		TR0 = 1;
		Delay(SPEED/4 * music[MusicSelect]);
		MusicSelect++;
		TR0 = 0;
		Delay(5);
	}
}

//==================== 主函数 ====================
void main()
{
	Timer0Init();
	TR0 = 0;
	while(1)
	{
		unsigned char key = KeyScan();
		if(key == 1)
		{
			PlayMusic(Music_Star);
		}
		if(key == 2)
		{
			PlayMusic(Music_Sky);
		}
	}
}

//==================== 定时器0中断 ====================
void Timer0_Routine() interrupt 1
{
	if(FreqTable[FreqSelect])
	{
		TL0 = FreqTable[FreqSelect] % 256;
		TH0 = FreqTable[FreqSelect] / 256;
		Buzzer = !Buzzer;
	}
}