#ifndef __SCENE_NEXT__H__
#define __SCENE_NEXT__H__

#include "cocos2d.h"
#include "GameResources.h"
#include "utils.h"
#include "MainScene.h"
#include "MainMenu.h"

using namespace cocos2d;

class NextScene : public CCLayerColor
{
private:

	void handleContinue(CCObject* sender);
	void handleRestart(CCObject* sender);
	void handleMenu(CCObject* sender);

public:

	static CCScene* scene();
	virtual bool init();

	CREATE_FUNC(NextScene);
};

#endif