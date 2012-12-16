//
//  MainMenu.cpp
//  gameX
//
//  Created by Mihailo Gazda on 12/5/12.
//
//

#include "MainMenu.h"

#pragma warning (push)
#pragma warning (disable: 4305)

extern int g_width;
extern int g_height;
extern unsigned int currentLevel;

int selectedLevel = 0;

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
	this->messageShowing = false;

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
    
    
	//logo
    CCMenuItemSprite* l = CCMenuItemSprite::create(CCSprite::createWithTexture(logo), CCSprite::createWithTexture(logo));
    l->setPosition(ccp(g_width / 2, g_height /2 + 100));
	this->addChild(l);
    
	//  menu
	CCMenuItemFont::setFontName("Impact");
    CCMenuItemFont *play = CCMenuItemFont::create("Play now", this, menu_selector(MainMenuScene::PlayButton));
    play->setPosition(ccp(g_width / 2, g_height /2 - 100));
    
    CCMenuItemFont* options = CCMenuItemFont::create("Options");
    options->setPosition(ccp(g_width / 2, g_height / 2 - 160));
    
    CCMenuItemFont* credits = CCMenuItemFont::create("Credits");
    credits->setPosition(ccp(g_width / 2, g_height / 2 - 220));
    
    CCMenu *menu = CCMenu::create(play, options, credits, NULL);
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
    
	if (this->messageShowing)
		return;    

	this->messageShowing = true;

	CCLayerColor* darkBack = CCLayerColor::create(ccc4(0, 0, 0, 255 * 0.6));
	this->addChild(darkBack);

	messageLayer = CCLayer::create();

	CCSprite* levelList = CCSprite::createWithSpriteFrameName("levelList.png");
	levelList->setPosition(ccp(g_width / 2, g_height / 2));
	messageLayer->addChild(levelList);
	messageLayer->setScale(0);

	this->addChild(messageLayer);


	CCCallFunc *cf1 = CCCallFunc::create(this, callfunc_selector(MainMenuScene::handleBoxShowed));
	CCSequence *seq = CCSequence::createWithTwoActions(CCEaseBounceOut::create(CCScaleTo::create(0.5, 1, 1)), cf1);
	messageLayer->runAction(seq);
}

void MainMenuScene::handleBoxShowed()
{
	CCLog("Handle box showed");

	char tmp[100];
	sprintf(tmp, "Level: %d", currentLevel);

	int x = g_width / 2;
	int y = g_height - 110;
	ccColor3B black = ccc3(0, 0, 0);
	char *font = "Impact";
	short fontSize = 28;
	selectedLevel = currentLevel;
	
	CCLabelTTF *level = CCLabelTTF::create(tmp, "Impact", fontSize);
	level->setPosition(ccp(x, y));
	level->setColor(black);
	messageLayer->addChild(level, 1);

	GAMELEVEL gl = LevelLoader::sharedLoader()->getGameLevel(currentLevel);
	if (gl.valid)
	{
		fontSize = 20;
		short spaceing = 60;

		y -= spaceing + 20;
		sprintf(tmp, "Points to win: %d", gl.minScore);
		CCLabelTTF *l1 = CCLabelTTF::create(tmp, font, fontSize);				
		l1->setPosition(ccp(x, y));
		l1->setColor(black);
		messageLayer->addChild(l1);

		y -= spaceing;
		sprintf(tmp, "Time to win it: %d seconds", gl.timeout);
		CCLabelTTF *l2 = CCLabelTTF::create(tmp, font, fontSize);
		l2->setPosition(ccp(x, y));
		l2->setColor(black);
		messageLayer->addChild(l2);

		y -= spaceing;
		sprintf(tmp, "New row comes: every %d seconds", gl.insertRowTime);
		CCLabelTTF *l3 = CCLabelTTF::create(tmp, font, fontSize);
		l3->setPosition(ccp(x, y));
		l3->setColor(black);
		messageLayer->addChild(l3);		

		y -= spaceing * 2;
		fontSize = 36;
		sprintf(tmp, "Play");		
		
		CCMenuItemFont::setFontSize(fontSize);
		CCMenuItemFont* play = CCMenuItemFont::create(tmp, this, menu_selector(MainMenuScene::handleLevelSelected));
		play->setColor(ccc3(200, 0, 100));		

		CCMenu* menu = CCMenu::create(play, NULL);
		menu->setPosition(ccp(x, y));
		messageLayer->addChild(menu);

	}
}

void MainMenuScene::handleLevelSelected(CCObject* sender)
{
	CCLog("handleLevelSelected");

	currentLevel = selectedLevel;
	CCDirector::sharedDirector()->pushScene(CCTransitionFadeBL::create(1, MainScene::scene()));
}

#pragma warning (pop)