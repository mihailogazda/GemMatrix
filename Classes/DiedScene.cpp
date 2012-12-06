#include "DiedScene.h"
#include "MainScene.h"

extern int g_width;
extern int g_height;

CCScene* DiedScene::scene()
{
	CCScene* ret = NULL;
	do
	{
		ret = CCScene::create();
		DiedScene *layer = DiedScene::create();
		CC_BREAK_IF(!ret || !layer);
		
		layer->numPoints = 0;

		//	add layer
		ret->addChild(layer);

	} while (false);

	return ret;
}

bool DiedScene::init()
{
	//	init parent and textures
	ccColor4B cornflowerBlue = ccc4(100, 149, 237, 255);
	ccColor4B colorWhite = ccc4(0, 0, 0, 255);
	CCLayerColor::initWithColor(colorWhite);

	int xs = g_width / 2;
	int ys = g_height / 2;
    
    //  Add background
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_ORANGE_BACK);
    
    CCSprite* back = CCSprite::createWithTexture(tex);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    
    back->setScale(2.0f);
    this->addChild(back);    

    int fontSize = (phoneType == IPAD3 || phoneType == IPHONE4) ? 72 : 36;
	CCLabelTTF *loser = CCLabelTTF::create("You loose!", "Impact", fontSize);
	loser->setPosition(ccp(xs, ys));	
	this->addChild(loser);
	
    fontSize = (phoneType == IPAD3 || phoneType == IPHONE4) ? 52 : 26;
    CCMenuItemFont::setFontSize(fontSize);
    
	CCMenuItemFont *backb = CCMenuItemFont::create("Back to the game", this, menu_selector(DiedScene::backButton));
	CCMenu *menu = CCMenu::create(backb, NULL);

	ys -= 80;
	if (phoneType == IPAD3)
		ys -= 40;

	menu->setPosition(ccp(xs, ys));
				
	this->addChild(menu);

	return true;
}

void DiedScene::backButton(CCObject* sender)
{
	CCLog("Back to main menu button");

	CCScene* sc = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene( CCTransitionFadeBL::create(1, sc) );

}