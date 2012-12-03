#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "cocos2d.h"

using namespace cocos2d;

class CCUtils 
{
public:

	//	gets random interval number with upper border
	static int randomInInterval(int min, int max)
    {
        return min + (rand() % (int)(max - min + 1));
	}

};

#endif //__CC_UTILS_H__