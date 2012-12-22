#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "cocos2d.h"
#include <time.h>
#include <vector>
#include <array>
#include <random>

#include "GameResources.h"


//	few usefull macros
#define INSERT_LABEL(name, txt, font, fontSize, x, y, color, layer) CCLabelTTF* name = CCLabelTTF::create(txt, font, fontSize); name->setPosition(ccp(x, y)); name->setColor(color); layer->addChild(name);

using namespace cocos2d;

enum IOSTYPE
{
    UNDEFINED = 0,
    IPHONE,
    IPHONE4,
    IPAD,
    IPAD3,
	WINDOWS,
	MAC,
	ANDROID,
	WINDOWS_MOBILE
};

//	for uniform distribution
static bool seedWasAssigned = false;
static bool timeSeedWasAssigned = false;
static bool bombSeedWasAssigned = false;

static std::vector<int> bombRandomList;
static std::vector<int> randomList;
static std::vector<int> timeRandomList;

class CCUtils 
{
public:

	static void randomizeVector(std::vector<int> &v, int oneIn)
	{
		for (int i = 0; i < oneIn; i++)
			v.push_back(i);

		std::random_shuffle(bombRandomList.begin(), bombRandomList.end());
	}

	//	gets random interval number with upper border
	static int randomInInterval(int min, int max)
    {
		if (!seedWasAssigned)
		{
			srand(time(0));
			seedWasAssigned = true;
		}
        return min + (rand() % (int)(max - min + 1));
	}

	static bool uniformProbabilityBombs(int oneIn = 120)
	{
		if (!bombSeedWasAssigned)
		{
			randomizeVector(bombRandomList, oneIn);
			bombSeedWasAssigned = true;
		}

		if (bombRandomList.size() == 0)
		{
			bombRandomList.clear();
			bombSeedWasAssigned = false;
		}
		else
		{
			int ret = bombRandomList.back();
			bombRandomList.pop_back();
			return ret == 5;
		}
		return false;
	}

	static bool uniformProbabilityTime(int oneIn = 120)
	{		
		if (!timeSeedWasAssigned)
		{
			randomizeVector(timeRandomList, oneIn);
			timeSeedWasAssigned = true;
		}

		if (timeRandomList.size() == 0)
		{
			timeRandomList.clear();
			timeSeedWasAssigned = false;
		}
		else
		{
			int ret = timeRandomList.back();
			timeRandomList.pop_back();
			return ret == 5;
		}

		return false;		
	}

	static void trim(char * s) 
	{
		char * p = s;
		int l = strlen(p);

		while(isspace(p[l - 1])) p[--l] = 0;
		while(* p && isspace(* p)) ++p, --l;

		memmove(s, p, l + 1);
	}

}; 

#endif //__CC_UTILS_H__