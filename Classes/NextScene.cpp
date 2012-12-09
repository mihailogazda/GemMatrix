#include "NextScene.h"

extern IOSTYPE phoneType;

CCScene* NextScene::scene()
{
	CCScene* ret = NULL;
	do
	{
		ret = CCScene::create();
		NextScene *layer = NextScene::create();
		CC_BREAK_IF(!ret || !layer);
		    
		//	add layer
		ret->addChild(layer);

	} while (false);

	return ret;
}

bool NextScene::init()
{
	//	init parent and textures
    ccColor4B colorWhite = ccc4(0, 0, 0, 255);
	CCLayerColor::initWithColor(colorWhite);
    
    //  Add background
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_GREENYELLOW);    
    
    CCSprite* back = CCSprite::createWithTexture(tex);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    
    back->setScale(2.0f);
    if (phoneType == IPAD3)
        back->setScale(4);
    
    this->addChild(back);

	return true;
}