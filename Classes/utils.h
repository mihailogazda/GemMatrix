#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "cocos2d.h"

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

class CCUtils 
{
public:

	//	gets random interval number with upper border
	static int randomInInterval(int min, int max)
    {
        return min + (rand() % (int)(max - min + 1));
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