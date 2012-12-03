#ifndef __DIED_SCENE_H__
#define __DIED_SCENE_H__

#include <cocos2d.h>
using namespace cocos2d;

class DiedScene : public CCLayerColor
{
private:

	void backButton(CCObject* sender);

public:

	unsigned int numPoints;

	static CCScene* scene();
	virtual bool init();

	CREATE_FUNC(DiedScene);
};

#endif