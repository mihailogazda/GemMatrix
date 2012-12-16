#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "cocos2d.h"

#if _WINDOWS
#include <random>
#include <array>
#include <time.h>
#endif

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

static std::array<int, 3> g_a = {33, 33, 33};
static std::vector<int> g_p(g_a.begin(), g_a.end());
static std::tr1::discrete_distribution<int> randomDist();

#if _WINDOWS
static std::tr1::mt19937 eng;
static std::tr1::mt19937 engBombs;
static std::tr1::mt19937 engTime;
static std::tr1::uniform_int_distribution<int> uniformGems(0, 3);
static std::tr1::uniform_int_distribution<int> uniformGemsBombs(0, GEM_ROCK_PROBABILITY_MAX);
static std::tr1::uniform_int_distribution<int> uniformGemsTime(0, GEM_ROCK_PROBABILITY_MAX * 2);
#endif

class CCUtils 
{
public:

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
    
#if _WINDOWS
	static int uniformRandomGems()
	{
		if (!seedWasAssigned)
		{
			eng.seed(static_cast<unsigned int>(time(0)));
			seedWasAssigned = true;
		}
		return uniformGems(eng);
	}

	static int uniformProbabilityBombs()
	{
		if (!bombSeedWasAssigned)
		{
			engBombs.seed(time(0));
			bombSeedWasAssigned = true;
		}
		return uniformGemsBombs(engBombs) == 0;
	}

	static int uniformProbabilityTime()
	{
		if (!timeSeedWasAssigned)
		{
			engTime.seed(time(0));
			timeSeedWasAssigned = true;
		}
		return uniformGemsTime(engTime) == 0;
	}

#endif

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