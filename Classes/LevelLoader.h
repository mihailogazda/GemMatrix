#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "cocos2d.h"


static LevelLoader* p_sharedLoader = NULL;

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