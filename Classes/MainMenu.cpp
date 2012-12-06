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
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_GREEN_BACK);
    CCTexture2D* logo = CCTextureCache::sharedTextureCache()->addImage(IMG_SPOOKY);
    
    CCSprite* back = CCSprite::createWithTexture(tex);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    
    back->setScale(2.0f);
    if (phoneType == IPAD3)
        back->setScale(4);
    
    this->addChild(back);
    
    g_width = CCDirector::sharedDirector()->getWinSize().width;
    g_height = CCDirector::sharedDirector()->getWinSize().height;
    
    int fontSize = (phoneType == IPAD3 || phoneType == IPHONE4) ? 52 : 26;
    CCMenuItemFont::setFontSize(fontSize);
    
    //  menu
    CCMenuItemSprite* l = CCMenuItemSprite::create(CCSprite::createWithTexture(logo), CCSprite::createWithTexture(logo));
    l->setPosition(ccp(g_width / 2, g_height /2 + 100));
    
    CCMenuItemFont *play = CCMenuItemFont::create("Play now", this, menu_selector(MainMenuScene::PlayButton));
    play->setPosition(ccp(g_width / 2, g_height /2 - 100));
    
    CCMenuItemFont* options = CCMenuItemFont::create("Options");
    options->setPosition(ccp(g_width / 2, g_height / 2 - 160));
    
    CCMenuItemFont* credits = CCMenuItemFont::create("Credits");
    credits->setPosition(ccp(g_width / 2, g_height / 2 - 220));
    
    CCMenu *menu = CCMenu::create(play, options, credits, l, NULL);
    menu->setPosition(ccp(0, 0));
    menu->setAnchorPoint(ccp(0, 0));
    
    this->addChild(menu);
    
    
    //  Move play button arround a little

    play->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCSkewBy::create(1, 5, 5)), CCEaseBackInOut::create(CCSkewBy::create(1, -5, -5)))));
    play->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCScaleTo::create(1, 1.2, 1.2)), CCEaseBackInOut::create(CCScaleTo::create(1, 1.0, 1.0)))));
    credits->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCSkewBy::create(1, 5, 5)), CCEaseBackInOut::create(CCSkewBy::create(1, -5, -5)))));
    credits->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCScaleTo::create(1, 1.2, 1.2)), CCEaseBackInOut::create(CCScaleTo::create(1, 1.0, 1.0)))));
    options->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCSkewBy::create(1, 5, 5)), CCEaseBackInOut::create(CCSkewBy::create(1, -5, -5)))));
    options->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCEaseBackInOut::create(CCScaleTo::create(1, 1.2, 1.2)), CCEaseBackInOut::create(CCScaleTo::create(1, 1.0, 1.0)))));
    
    
    //  move background too
    CCEaseBackInOut* b1 = CCEaseBackInOut::create(CCMoveBy::create(10, ccp(-30, -15)));
    CCEaseBackInOut* b2 = CCEaseBackInOut::create(CCMoveBy::create(10, ccp(+30, +15)));
    
    int ws = 2;
    int ws2 = 2.4;
    if (phoneType == IPAD3)
    {
        ws = 4;
        ws2 = 5.6;
    }
    
    CCEaseBackInOut* b3 = CCEaseBackInOut::create(CCScaleTo::create(10, ws, ws2));
    CCEaseBackInOut* b4 = CCEaseBackInOut::create(CCScaleTo::create(10, ws, ws));
    
    CCSequence* bs1 = CCSequence::createWithTwoActions(b1, b2);
    CCSequence* bs2 = CCSequence::createWithTwoActions(b3, b4);
    
    back->runAction(CCRepeatForever::create(bs1));
    back->runAction(CCRepeatForever::create(bs2));
    
    return true;
}

void MainMenuScene::PlayButton(CCObject* sender)
{
    CCLog("PlayButton");
    
    CCDirector::sharedDirector()->pushScene(CCTransitionFadeBL::create(1, MainScene::scene()));
    
}