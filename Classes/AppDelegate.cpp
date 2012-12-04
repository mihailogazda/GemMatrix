
#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "DiedScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

#ifdef __MACH__
int g_width = 0;
int g_height = 0;
#endif

IOSTYPE phoneType = UNDEFINED;

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
    
#ifdef __MACH__
    g_width = pDirector->getWinSize().width;
    g_height = pDirector->getWinSize().height;
    
    
    if (g_width == 2048)
        phoneType = IPAD3;
    else if (g_width == 960)
        phoneType = IPHONE4;
    else if (g_width == 1024)
        phoneType = IPAD;
    else if (g_width == 640)
        phoneType = IPHONE;
    
    
    CCLog("Window size: %dx%d", g_width, g_height);
#endif
    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
	CCScene *pScene = MainScene::scene();
	//CCScene *pDied = DiedScene::scene();

    // run
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
