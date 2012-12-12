#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "cocos2d.h"
#include <random>
#include <array>
#include "GameResources.h"

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
//static std::array<int, 3> g_a = {33, 33, 33};
//static std::vector<int> g_p(g_a.begin(), g_a.end());
//static std::discrete_distribution<int> randomDist();

static std::mt19937 eng;
static std::uniform_int_distribution<int> uniformGems(0, 2);
static std::uniform_int_distribution<int> uniformGemsRocks(0, GEM_ROCK_PROBABILITY_MAX);

class CCUtils 
{
public:

	//	gets random interval number with upper border
	static int randomInInterval(int min, int max)
    {		
        return min + (rand() % (int)(max - min + 1));
	}

	static int uniformRandomGems()
	{
		return uniformGems(eng);
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