#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "cocos2d.h"

typedef struct 
{
	unsigned int timeout;
	unsigned int insertRowTime;
	unsigned int minScore;
} GAMELEVEL, *LPGAMELEVEL;

//	singleton class
class LevelLoader;
static LevelLoader* p_sharedLoader = NULL;

//
//	Enables level loading from resource XML file
//
class LevelLoader 
{
private:

	LevelLoader()
	{
	}

	~LevelLoader()
	{
	}

public:

	LevelLoader* sharedLoader()
	{
		if (!p_sharedLoader)
		{
			p_sharedLoader = new LevelLoader();
			return p_sharedLoader;
		}
		return p_sharedLoader;
	}


};


#endif