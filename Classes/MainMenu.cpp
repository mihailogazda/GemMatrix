//
//  MainMenu.cpp
//  gameX
//
//  Created by Mihailo Gazda on 12/5/12.
//
//

#include "MainMenu.h"


extern int g_width;
extern int g_height;

CCScene* MainMenuScene::scene()
{
	CCScene* ret = NULL;
	do
	{
		ret = CCScene::create();
		MainMenuScene *layer = MainMenuScene::create();
		CC_BREAK_IF(!ret || !layer);
		    
		//	add layer
		ret->addChild(layer);

	} while (false);

	return ret;
}

bool MainMenuScene::init()
{
	//	init parent and textures
    ccColor4B colorWhite = ccc4(0, 0, 0, 255);
	CCLayerColor::initWithColor(colorWhite);
    
    //  Add background
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_RED_BACK);
    CCTexture2D* logo = CCTextureCache::sharedTextureCache()->addImage(IMG_SPOOKY);
    
    CCSprite* back = CCSprite::createWithTexture(tex);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    
    back->setScale(2.0f);
    this->addChild(back);
    
    g_width = CCDirector::sharedDirector()->getWinSize().width;
    g_height = CCDirector::sharedDirector()->getWinSize().height;
    
    
    //  menu
    CCMenuItemSprite* l = CCMenuItemSprite::create(CCSprite::createWithTexture(logo), CCSprite::createWithTexture(logo));
    l->setPosition(ccp(g_width / 2, g_height /2 + 100));
    
    CCMenuItemFont *play = CCMenuItemFont::create("Play now", this, menu_selector(MainMenuScene::PlayButton));
    play->setPosition(ccp(g_width / 2, g_height /2 - 100));
    
    CCMenu *menu = CCMenu::create(play, l, NULL);
    menu->setPosition(ccp(0, 0));
    menu->setAnchorPoint(ccp(0, 0));
    
    this->addChild(menu);
    
    return true;
}

void MainMenuScene::PlayButton(CCObject* sender)
{
    CCLog("PlayButton");
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(1, MainScene::scene()));
    
}