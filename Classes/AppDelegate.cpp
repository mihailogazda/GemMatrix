
#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "MainMenu.h"
#include "DiedScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

int g_width = 0;
int g_height = 0;

IOSTYPE phoneType = UNDEFINED;
unsigned int currentLevel = 1;
unsigned int retina = 0;
unsigned int totalPoints = 0;
unsigned int totalWonPoints = 0;

//	windows only
bool overidedByCommandLine = false;


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());


    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
	//	capture projector size
    g_width = pDirector->getWinSize().width;
    g_height = pDirector->getWinSize().height;

	
#ifdef __MACH__    

    if (g_width == 2048)
        phoneType = IPAD3;
    else if (g_width == 960)
        phoneType = IPHONE4;
    else if (g_width == 1024)
        phoneType = IPAD;
    else if (g_width == 640)
        phoneType = IPHONE;

	//	enable 2x pixel 
	if (phoneType == IPAD3 || phoneType == IPHONE4)
		retina = true;
		
#endif	

#ifdef _WINDOWS
	phoneType = WINDOWS;
#endif
	    
    CCLog("Window size: %dx%d", g_width, g_height);
		
	LevelLoader *l = LevelLoader::sharedLoader();
	CCLog("Level count: %d", l->getLevelsCount());

	//GAMELEVEL lev = l->getGameLevel(currentLevel);

	//	load textures
	CCSpriteFrameCache *sfc = CCSpriteFrameCache::sharedSpriteFrameCache();	
	sfc->addSpriteFramesWithFile(IMG_MAIN_TILESET_PLIST);
	
	//	load current level and points
	totalPoints = CCUserDefault::sharedUserDefault()->getIntegerForKey(SETTING_TOTAL_POINTS, 0);
	if (!overidedByCommandLine)	
		currentLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(SETTING_LEVEL_CURRENT, 1);		

    // run
	CCScene *pScene = MainMenuScene::scene();
    pDirector->runWithScene(pScene);


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
