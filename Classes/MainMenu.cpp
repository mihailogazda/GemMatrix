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
    
    
    
    
    return true;
}