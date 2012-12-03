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

	CCLabelTTF *loser = CCLabelTTF::create("You loose!", "Impact", 36);
	loser->setPosition(ccp(xs, ys));	
	this->addChild(loser);	
	
	CCMenuItemFont::setFontSize(20);
	CCMenuItemFont *back = CCMenuItemFont::create("Back to the game", this, menu_selector(DiedScene::backButton));		
	CCMenu *menu = CCMenu::create(back, NULL);

	ys -= 80;
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