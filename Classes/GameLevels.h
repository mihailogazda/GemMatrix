#ifndef __GAME_LEVELS_H__
#define __GAME_LEVELS_H__


typedef struct 
{
	unsigned int timeout;
	unsigned int insertRowTime;
	unsigned int minScore;
} GAMELEVEL;


static const GAMELEVEL GAME_LEVELS[] = 
{	
	{60,	10,		4000},	//1
	{60,	9,		6000},	//2
	{60,	8,		8000},	//3
	{60,	7,		1000},	//4
	NULL
};

#endif