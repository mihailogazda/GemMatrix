//
//  MainMenu.h
//  gameX
//
//  Created by Mihailo Gazda on 12/5/12.
//
//

#ifndef gameX_MainMenu_h
#define gameX_MainMenu_h

#include <cocos2d.h>
#include "GameResources.h"
#include "MainScene.h"

using namespace cocos2d;

class MainMenuScene : public CCLayerColor
{
private:

	void PlayButton(CCObject* sender);

public:

	static CCScene* scene();
	virtual bool init();

	CREATE_FUNC(MainMenuScene);
};


#endif