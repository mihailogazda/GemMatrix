#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameResources.h"
#include "GemSprite.h"
#include "LevelLoader.h"
#include <list>

using namespace cocos2d;

typedef struct  
{
	int row;
	int col;
} MATRIXPOS;


class MainScene : public CCLayerColor
{
private:
    
	int pointsCount;
	int timerCount;
    int totalTime;
	
	GAMELEVEL gameLevel;

	CCProgressTimer *timeProgress;

	CCLabelTTF* pointsLabel;
	CCLabelTTF* timeLabel;
    CCLabelTTF* levelLabel;

	CCMenuItemSprite* upButton; 
	CCMenu* pauseMenu;
	CCMenu* sidebarMenu;

	CCLayer* gameContent;
    CCLayerColor* sidebar;
    
	bool initTextures();

	//	game matrix we are going to use    
    CCGemSprite* matrix[MAX_ROWS][MAX_COLS];
	CCGemSprite* spareRow[MAX_COLS];
    
    short rows;
    short columns;
    
	void initMatrix();
	void initSpareRow();

	void sortMatrix();
	void updateMatrix();

	CCPoint positionForElement(int row, int col, bool isSpareRow);

	void handleClickReset(CCObject* sender);
    void handleClickMenu(CCObject* sender);
    
	bool initSidebar();

	void handleTimeUpdate(float delta);
    void handleClickUp(CCObject* sender);

	bool isPaused;
	CCLayerColor* pauseLayer;
	void handleClickPause(CCObject* sender);

	void handleTimeout();

public:

    bool wasInit;
	virtual bool init();
    void postInit();    

	static CCScene* scene();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void verifyTouch(CCPoint point);

	std::vector<MATRIXPOS> foundItems;
	void processItem(int row, int col);
	void postProcess();
	void checkForBonus();
	
	bool disableTouches;
	int callbackCount;
	void callbackItemHidden(CCNode* node);

	void redrawMatrix();
	void redrawSpareRow();

	void reorganizeMatrix();
    void insertRowFromBottom();

	//	dont know what this is
	CREATE_FUNC(MainScene);
};


#endif