#include "MainScene.h"
#include "MainMenu.h"

extern int g_height;
extern int g_width;
extern IOSTYPE phoneType;
extern unsigned int currentLevel;
extern unsigned int totalPoints;
extern unsigned int totalWonPoints;

bool transitionInProgress = false;

GAMELEVEL globalCurrentLevel = {0};

CCScene* MainScene::scene()
{
	CCScene* ret = NULL;
	do
	{
		ret = CCScene::create();
		MainScene *mainLayer = MainScene::create();
		CC_BREAK_IF(!ret || !mainLayer);
		
		//	add layer
		ret->addChild(mainLayer);
        
	} while (false);

	return ret;
}

bool MainScene::init()
{
	//	init parent and textures
	//ccColor4B cornflowerBlue = ccc4(100, 149, 237, 255);
	ccColor4B colorWhite = ccc4(0, 0, 0, 255);
	CCLayerColor::initWithColor(colorWhite);
	initTextures();
    
	//	init snow
	CCParticleSnow *ps = CCParticleSnow::create();
	CCTexture2D *tx = CCTextureCache::sharedTextureCache()->addImage(IMG_SNOWFLAKE);
	ps->setTexture(tx);
    
    if (phoneType == IPAD3)
        ps->setScale(2.0f);
    
	this->addChild(ps);

	//	init matrix
	memset(this->matrix, 0, sizeof(this->matrix));
	memset(this->spareRow, 0, sizeof(this->spareRow));	

	//	init game content layer
	this->gameContent = CCLayer::create();
	this->gameContent->setPosition(ccp(0, 0));
	this->gameContent->setAnchorPoint(ccp(0, 0));
	this->addChild(this->gameContent, 1);
   
	this->wasInit = false;
	this->upButton = NULL;

	this->gameLevel = LevelLoader::sharedLoader()->getGameLevel(currentLevel);
	globalCurrentLevel = this->gameLevel;

	//CCAssert(this->gameLevel.valid, "GameLevel not valid. Probably end of the game.");	
	if (!this->gameLevel.valid)
	{
#ifdef CC_PLATFORM_WIN32
		MessageBox(NULL, "Probably end of the game (no more levels in xml file)", "Hello", MB_OK | MB_ICONWARNING);		
#endif
		exit(0);
	}

	//	set points and such
    this->totalTime = 0;
    this->pointsCount = 0;
	this->timerTip = 0;
	this->bonusVisibleCount = 0;
	
	this->timerCount = this->gameLevel.insertRowTime;
	this->pauseMessage = NULL;
	this->doneMessage = NULL;

	//	schedule ticker
	this->schedule(schedule_selector(MainScene::handleTimeUpdate), 1);
    
	return true;
}

void MainScene::postInit()
{
	this->initMatrix();
	this->initSpareRow();

	//	init sidebar
	this->initSidebar();	

	//	finally - init touches
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	//	callback count reset - this will enable touching
	this->disableTouches = false;
}

void MainScene::handleClickReset(CCObject* sender)
{
	CCLog("Reset click");

	CCDirector* sd = CCDirector::sharedDirector();
	CCScene* sc = MainScene::scene();
	//CCTransitionScene* t = CCTransitionFlipAngular::create(1, sc);
    CCTransitionScene* t = CCTransitionFadeBL::create(1, sc);
    
    sidebar->runAction(CCEaseBounceOut::create(CCMoveBy::create(1, ccp(200, 0))));

	sd->replaceScene(t);
}

void MainScene::handleClickUp(CCObject* sender)
{
	CCLog("Handle UP button");
    
	if (this->disableTouches || this->isMatrixFull())
		return;

	//	add points	
	CCLabelTTF *pts = CCLabelTTF::create("100 points!", GAME_FONT, 24);
	pts->setPosition(this->upButton->getPosition());
	this->sidebar->addChild(pts);
	
	pts->runAction(CCEaseOut::create(CCMoveBy::create(0.5, ccp(0, 50)), 1));
	pts->runAction(CCEaseOut::create(CCFadeTo::create(0.5, 0), 1));	
	this->pointsCount += 100;
	
	char tmp[50];
	sprintf(tmp, "%d points", this->pointsCount);
	this->pointsLabel->setString(tmp);
	this->updateProgress();

	//	now insert rows
	this->insertRowFromBottom();
}

void MainScene::handleClickMenu(CCObject* sender)
{
    CCLog("handleClickMenu");

    CCScene* sc = CCTransitionFadeBL::create(1, MainMenuScene::scene());
    CCDirector::sharedDirector()->replaceScene(sc);
}

void MainScene::handlePauseShow()
{
	this->pauseSchedulerAndActions();
	this->disableTouches = true;	
	this->sidebarMenu->setTouchEnabled(false);

	CCMenuItemFont *p1 = CCMenuItemFont::create("Continue", pauseMessage, pauseMessage->sel_hideMessageBox);
	p1->setColor(ccc3(0, 0, 0));
	
	CCMenu *m = CCMenu::create(p1, NULL);	
	pauseMessage->getMessageLayer()->addChild(m);
}

void MainScene::handlePauseHide()
{	
	this->disableTouches = false;
	this->sidebarMenu->setTouchEnabled(true);

	this->resumeSchedulerAndActions();
}

void MainScene::handleClickPause(CCObject* sender)
{
	CCLog("Pause click");	

	if (this->pauseMessage)
		delete this->pauseMessage;

	this->pauseMessage = CCGameMessage::create(this, callfunc_selector(MainScene::handlePauseShow), callfunc_selector(MainScene::handlePauseHide));
	pauseMessage->showMessageBox();
}

bool MainScene::initTextures()
{
    // add background
	CCSprite* back = CCSprite::create(IMG_BLUE_BACK);
	back->setAnchorPoint(ccp(0, 0));
    
    if ( phoneType == IPAD3)
        back->setScale(4.0f);
    else
        back->setScale(2.0f);
    
	this->addChild(back);   	
	return true;
}

void MainScene::initMatrix()
{
	//	clear memory
	memset(this->matrix, 0, sizeof(this->matrix));
    
    //  init scale
    this->rows = MAX_ROWS;
    this->columns = MAX_COLS;
    
    if (phoneType == IPAD3 || phoneType == IPAD)
	{
        this->rows = MAX_ROWS_IPAD3;
        this->columns = MAX_COLS_IPAD3;
    }
	else if (phoneType == WINDOWS)
	{
		this->rows = MAX_ROWS_WINDOWS;
		this->columns = MAX_COLS_WINDOWS;
	}

	//	init start position
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			int stop = this->rows - this->gameLevel.substractRows;
			if (i < stop)
			{
				CCGemSprite* gs = CCGemSprite::create();
				gs->setAnchorPoint(ccp(0, 0));
				this->matrix[i][j] = gs;
				this->gameContent->addChild(gs);
			}
		}
	}

	//	now show matrix
	this->redrawMatrix();	
}

void MainScene::initSpareRow()
{
	memset(this->spareRow, 0, sizeof(this->spareRow));

	//	init spare row	
	for (int i = 0; i < this->columns; i++)
	{
		//if (this->spareRow[i] != NULL)		
		//	this->spareRow[i]->removeFromParentAndCleanup(true);					

		CCGemSprite* gs = CCGemSprite::create();
		this->spareRow[i] = gs;		
		gs->setAnchorPoint(ccp(0, 0));
		gs->setOpacity(SPARE_ROW_FADE_START);
		this->gameContent->addChild(gs);
	}

	this->redrawSpareRow();
}

bool MainScene::initSidebar()
{
	char* fontName = GAME_FONT;
	unsigned int fontSize = 26;
    
    if (phoneType == IPAD3 || phoneType == IPHONE4)
        fontSize *= 2;

    int gw = 200;
	int xs = g_width;
	int ys = g_height - 30;

	int buttonStart = 120;
    
    if (phoneType == IPAD3 || phoneType == IPHONE4)
        gw *= 2;

	//	set size of game content (minus right bar)
	CCSize s = this->getContentSize();
	s.width -= gw;
	this->gameContent->setContentSize(s);
	
#ifdef _WINDOWS
		buttonStart = 70;
#endif
    
	ccColor4B colorWhite = ccc4(255, 255, 255, 100);
	ccColor4B colorWhatever = ccc4(247, 133, 26, 255);

	sidebar = CCLayerColor::create(colorWhite);
    sidebar->setPosition(ccp(xs, 0));
    sidebar->setAnchorPoint(ccp(0, 0));

	char tmp[100];
	sprintf(tmp, "Level %d", currentLevel);

    levelLabel = CCLabelTTF::create(tmp, fontName, fontSize - 6);
    levelLabel->setPosition(ccp(gw / 2, g_height - 20));
    sidebar->addChild(levelLabel);

	sprintf(tmp, "Total: %d", totalPoints);
	CCLabelTTF* pointsTotal = CCLabelTTF::create(tmp, fontName, fontSize - 6);
	pointsTotal->setPosition(ccp(gw / 2, g_height - 50));
	pointsTotal->setColor(ccc3(80, 80, 80));
	sidebar->addChild(pointsTotal);

	/*
	sprintf(tmp, "Collect %d", gameLevel.minScore);	
	CCLabelTTF *collect = CCLabelTTF::create(tmp, fontName, fontSize - 6);
	collect->setPosition(ccp(gw /2, g_height - 60));
	collect->setColor(ccc3(80, 80, 80));
	sidebar->addChild(collect);
	*/

	//	points won	
	sprintf(tmp, "0 points");
	pointsLabel = CCLabelTTF::create(tmp, fontName, fontSize);
	pointsLabel->setPosition(ccp(gw / 2, g_height - 90));
    sidebar->addChild(pointsLabel);
    
	pointsProgress = CCProgressTimer::create(CCSprite::create(IMG_PROGRESS_FORE));
	CCSprite* backProgress = CCSprite::create(IMG_PROGRESS);

	pointsProgress->setPosition(ccp(gw / 2, g_height - 115));
	backProgress->setPosition(ccp(gw / 2, g_height - 115));

	pointsProgress->setType(kCCProgressTimerTypeBar);
	pointsProgress->setMidpoint(ccp(0, 0));	
	pointsProgress->setPercentage(0);
	pointsProgress->setBarChangeRate(ccp(1, 0));
	
	sidebar->addChild(backProgress);
	sidebar->addChild(pointsProgress);

	short menuItemX = gw / 2;
	short menuItemStart = g_height - 300;

	//	game menu
	CCMenuItemFont::setFontName(fontName);
	CCMenuItemFont::setFontSize(fontSize);	

	CCMenuItemFont* pause =  CCMenuItemFont::create("Pause", this, menu_selector(MainScene::handleClickPause));
	pause->setPosition(ccp(menuItemX, menuItemStart));

	menuItemStart -= 30;
	CCMenuItemFont* reset = CCMenuItemFont::create("Restart", this, menu_selector(MainScene::handleClickReset));
	reset->setPosition(ccp(menuItemX, menuItemStart));
    
	menuItemStart -= 30;
    CCMenuItemFont* mainMenu = CCMenuItemFont::create("Menu", this, menu_selector(MainScene::handleClickMenu));
    mainMenu->setPosition(ccp(menuItemX, menuItemStart));
    
	this->sidebarMenu = CCMenu::create(pause, reset, mainMenu, NULL);
    this->sidebarMenu->setPosition(ccp(0, 0));
    this->sidebarMenu->setAnchorPoint(ccp(0, 0));
	sidebar->addChild(this->sidebarMenu);

	//	add clock
	CCSprite* clockBack = CCSprite::create(IMG_CLOCK);
	clockBack->setScale(0.2f);
	CCPoint clockPos = ccp(menuItemX, g_height - 190);
	clockBack->setPosition(clockPos);
	sidebar->addChild(clockBack);

	this->timeProgress = CCProgressTimer::create(CCSprite::create(IMG_CLOCK_FORE));
	this->timeProgress->setPosition(clockPos);
	this->timeProgress->setScale(0.2f);
	this->timeProgress->setType(kCCProgressTimerTypeRadial);
	this->timeProgress->setPercentage(0);
	sidebar->addChild(timeProgress, 1);    
    
    //  now button
	if (!gameLevel.upButtonHidden)
	{
		CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_BUTTON);
		upButton = CCMenuItemSprite::create(CCSprite::createWithTexture(tex), CCSprite::createWithTexture(tex), this, menu_selector(MainScene::handleClickUp));
    
		if (phoneType == IPAD3)
			upButton->setScale(1.4f);
		else
			upButton->setScale(0.7f);
    
		upButton->setPosition(ccp(gw / 2, buttonStart));
		this->sidebarMenu->addChild(upButton);
	}
    
	//	Timer for new row
	this->timeLabel = CCLabelTTF::create("0s", fontName, fontSize - 6);
	this->timeLabel->setPosition(ccp(gw / 2, buttonStart));
	sidebar->addChild(this->timeLabel, 1);

    this->addChild(sidebar);
    
    sidebar->runAction(CCEaseBounceOut::create(CCMoveBy::create(1, ccp(-gw, 0))));

	return true;
}

CCPoint MainScene::positionForElement(int row, int col, bool isSpareRow = false)
{
	CCLOG("positionForElement %dx%d", row, col);

	int xs = 30;
	int ys = 15;

	if (!isSpareRow)
		ys += GEM_HEIGHT + (GEM_SPACING * 2);

	int x = xs + (col * (GEM_WIDTH + GEM_SPACING)  );
	int y = ys + (row * (GEM_HEIGHT + GEM_SPACING) );	

	return ccp(x, y);
}

bool MainScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	return true;
}

void MainScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLog("Touches ended");
	CCPoint point = pTouch->getLocation();	
	verifyTouch(point);	
}

bool timeArtifactInProcessing = false;
void MainScene::verifyTouch(CCPoint point)
{
	CCLog("Touch recieved: x:%f y:%f", point.x, point.y);
	
	if (this->disableTouches){
		CCLog("Processing in progress. Skip this one.");
		return;
	}

	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			if (this->matrix[i][j] == NULL)
				continue;				

			CCRect r = this->matrix[i][j]->boundingBox();
			if (r.containsPoint(point))
			{
				CCLog("Found item @ %dx%d", i, j);

				if (this->matrix[i][j]->getTag() == TIME_ID)
					timeArtifactInProcessing = true;

				if (this->matrix[i][j]->getTag() == BOMB_ID)
				{
					this->processBomb(i, j);
				}
				else
				{
					this->foundItems.clear();
					this->processItem(i, j);
					this->postProcess();
				}
				return;
			}
		}
	}
}

void MainScene::processItem(int row, int col)
{	
	//	skipp existing elements	
	for (unsigned int i = 0; i < this->foundItems.size(); i++)
	{
		MATRIXPOS p2 = this->foundItems.at(i);
		if ( p2.col == col && p2.row == row )
			return;
	}
	
	CCGemSprite* s = this->matrix[row][col];
	int tag = s->getTag();
	
	MATRIXPOS p;
	p.col = col;
	p.row = row;
	this->foundItems.push_back(p);
	//s->setTag(-1);//already visited

	//	Check for connecting ones
	CCGemSprite* left = col - 1 >= 0 ? this->matrix[row][col - 1] : NULL;
	CCGemSprite* right = col + 1 < MAX_COLS ? this->matrix[row][col + 1] : NULL;
	CCGemSprite* top = row + 1 < MAX_ROWS ? this->matrix[row + 1][col] : NULL;
	CCGemSprite* bottom = row - 1 >= 0 ? this->matrix[row - 1][col] : NULL;

	//	
	if (left && (left->getTag() == tag || left->getTag() == TIME_ID) )
		this->processItem(row, col - 1);
	if (right && (right->getTag() == tag || right->getTag() == TIME_ID) )
		this->processItem(row, col + 1);
	if (top && (top->getTag() == tag || top->getTag() == TIME_ID) )
		this->processItem(row + 1, col);
	if (bottom && (bottom->getTag() == tag || bottom->getTag() == TIME_ID) )
		this->processItem(row - 1, col);
}

void MainScene::hideItem(unsigned int row, unsigned int col)
{
	CCGemSprite* sp = this->matrix[row][col];
	CC_ASSERT(sp);

	if (sp->getTag() == TIME_ID)
		processTimeArtifact();

	//	fade out
	//CCBlink* fo = CCBlink::create(0.6f, 2);		
	CCHide *hi = CCHide::create();
	CCCallFuncN *cf = CCCallFuncN::create(this, callfuncN_selector(MainScene::callbackItemHidden));
	CCAction* seq = CCSequence::create(/*fo,*/ hi, cf, NULL);
		
	//	Add points won per gem
	CCPoint pos = positionForElement(row, col);
	CCLabelTTF *lab  = CCLabelTTF::create("10", GAME_FONT, 16);
	pos.x += GEM_WIDTH / 2;
	pos.y += GEM_HEIGHT / 2;
	lab->setPosition(pos);
	this->addChild(lab);

	CCFadeOut* fo2 = CCFadeOut::create(0.8f);				
	CCMoveBy* mb2 = CCMoveBy::create(0.4f, ccp(0, 10));

	sp->runAction(seq);
	lab->runAction(mb2);
	lab->runAction(fo2);

	this->matrix[row][col] = NULL;
}

void MainScene::postProcess()
{
	int size = this->foundItems.size();
	if (size < 3)
	{
		CCLOG("NOT ENOUGH");

		//	level checks for invalid touches
		if (gameLevel.checksInvalid && !timeArtifactInProcessing)
		{
			this->pointsCount = max(this->pointsCount - 100, 0); // always positive

			char tmp[50];
			sprintf(tmp, "%d points", this->pointsCount);
			this->pointsLabel->setString(tmp);
		}

		timeArtifactInProcessing = false;
		return;
	}

	timeArtifactInProcessing = false;

	this->callbackCount = size;
	this->disableTouches = true;

	//	set points
	this->checkForBonus();
	this->timerTip = 0;

	//	check for bonus

	//	hide all
	for (int i = 0; i < size; i++)
	{
		MATRIXPOS p = this->foundItems.at(i);
		CCLog("Post processing item: %d x %d", p.row, p.col);			

		//	finally hide
		this->hideItem(p.row, p.col);

		/*
			//http://www.cocos2d-iphone.org/forum/topic/19523
			CCParticleExplosion *ex = CCParticleExplosion::create();		
			ex->setPosition(pos);
			CCTexture2D *tx = CCTextureCache::sharedTextureCache()->addImage(IMG_SNOWFLAKE);
			ex->setTexture(tx);
			this->addChild(ex);
		*/
	}
}

void MainScene::completeShowBonus()
{
	CCLog("Complete show bonus");
	--this->bonusVisibleCount;
}

void MainScene::showBonusMessage(char* message)
{
	CCLabelTTF* bonusLabel = CCLabelTTF::create(message, GAME_FONT, 28);
	CCAssert(bonusLabel, "Bonus label is empty");	

	int spustac =  this->bonusVisibleCount * 80;

	bonusLabel->setPosition(ccp(gameContent->getContentSize().width / 2, g_height / 2 + 50 - spustac));
	bonusLabel->setOpacity(0);
	gameContent->addChild(bonusLabel, 2);

	bonusLabel->runAction(CCSequence::create(
		CCFadeTo::create(0.2f, 255),
		CCScaleTo::create(0.5, 2),			
		CCCallFunc::create(this, callfunc_selector(MainScene::completeShowBonus)),
		CCFadeTo::create(1.5, 0),		
		NULL));

	this->bonusVisibleCount++;
}

void MainScene::checkForBonus()
{
	unsigned int size = this->foundItems.size();
	
	//	now bonus
	unsigned int bonusOver = this->gameLevel.bonusOver;
	int bonus = size - bonusOver;
	if (bonus > 0)
	{
		int bonusWon = BONUS_FACTOR * GEM_POINS * bonus;
		this->pointsCount += bonusWon;

		char bonusText[100] = "";
		sprintf(bonusText, "%d BONUS POINTS!", bonusWon);
		
		/*
		if (bonus <= 3)
			sprintf(bonusText, "GREAT!");
		else if (bonus <= 5)
			sprintf(bonusText, "AMAZING!");
		else if (bonus <= 10)
			sprintf(bonusText, "INCREDIBLE!");
		else if (bonus <= 15)
			sprintf(bonusText, "GOD LIKE!");
		*/
		
		this->showBonusMessage(bonusText);
	}

	//	set points
	this->pointsCount += size * GEM_POINS;
	char points[100] = "";
	sprintf(points, "%d points", this->pointsCount);
	this->pointsLabel->setString(points);
	this->updateProgress();
}

void MainScene::callbackItemHidden(CCNode* node)
{
	CCLog("Done hiding item callback");
	//CCSprite *s = (CCSprite*) node;

	if (--callbackCount == 0)
	{
		CCLog("All items processed. Reorganize.");		
		this->reorganizeMatrix();
		this->disableTouches = false;
	}
}

void MainScene::reorganizeMatrix()
{
	CCGemSprite* newMatrix[MAX_ROWS][MAX_COLS] = {0};
	unsigned int lastColumn = 0;

	for (int i = 0; i < this->columns; i++)
	{
		int lastPositionNotNull = 0;
		for (int j = 0; j < this->rows; j++)
		{
			CCGemSprite* item = this->matrix[j][i];

			if (item != NULL)
			{
				this->matrix[j][i] = NULL;
				this->matrix[lastPositionNotNull][i] = item;
				lastPositionNotNull++;
			}
		}

		//	this removes empty rows
		if (lastPositionNotNull != 0)
		{
			for (int j = 0; j < this->rows; j++)			
				newMatrix[j][lastColumn] = this->matrix[j][i];
			lastColumn++;
		}
	}

	//	copy new matrix over existing one
	memcpy(this->matrix, newMatrix, sizeof(this->matrix));	
	
	//	now center align
	CCGemSprite* newMatrix2[MAX_ROWS][MAX_COLS] = {0};

	unsigned int lastTaken = 0;
	unsigned int start = ceil( (float)(this->columns - lastColumn) / 2 );
	
	for (int i = start; i < this->columns; i++)
	{
		for (int j = 0; j < this->rows; j++)
			newMatrix2[j][i] = this->matrix[j][lastTaken];
		lastTaken++;
	}

	memcpy(this->matrix, newMatrix2, sizeof(this->matrix));

	//	redraw
	this->redrawMatrix();	
}

void MainScene::redrawMatrix()
{
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			if (this->matrix[i][j] != NULL)
			{
				CCPoint pos = positionForElement(i, j);
				this->matrix[i][j]->stopAllActions();
				this->matrix[i][j]->runAction( CCEaseBounceOut::create( CCMoveTo::create(0.5f, pos) ) );
			}
		}
	}
}

void MainScene::redrawSpareRow()
{
	for (int i = 0; i < this->columns; i++)
	{
		CCPoint pos = positionForElement(0, i, true);
		this->spareRow[i]->runAction( CCEaseBounceOut::create( CCMoveTo::create(0.5f, pos) ) );
	}
}

void MainScene::handleTimeUpdate(float delta)
{
	//CCLog("Handle Time Update");
    
    if (!this->wasInit)
    {
        this->postInit();
        this->wasInit = true;
    }

	this->totalTime++;
	this->timerTip++;

	float max = this->gameLevel.timeout;
	float perc =  (this->totalTime / max) * 100 ;
	//CCLog("Percentage is %f (%d of %d)", perc, this->totalTime, max);
	this->timeProgress->setPercentage(perc);

	//	check for timeout
	if (this->totalTime == this->gameLevel.timeout)
	{
		//this->handleTimeout();
		//this->handleDone();
		this->doneMessage = CCGameMessage::create(this, callfunc_selector(MainScene::handleDone), callfunc_selector(MainScene::handleDoneExit));			
		this->doneMessage->showMessageBox();
		return;
	}

	if (this->timerTip >= this->gameLevel.tipPause)
	{
		this->startTipDiscovery();
		this->timerTip = 0;
	}

	//	check for new element insertion
	this->timerCount--;

	char tmp[100] = {0};
	sprintf(tmp, "%ds", this->timerCount);
	this->timeLabel->setString(tmp);

	int secTillReset = this->gameLevel.insertRowTime;
	float fade = SPARE_ROW_FADE_START + (255 - this->timerCount * (SPARE_ROW_FADE_END / secTillReset));
	if (fade > SPARE_ROW_FADE_END)
		fade = SPARE_ROW_FADE_END;

	for (int i = 0; i < this->columns; i++)
		this->spareRow[i]->runAction( CCFadeTo::create(1, fade) );

	if (this->timerCount == 0)
	{
		this->disableTouches = true;
        this->insertRowFromBottom();    
		this->disableTouches = false;
	}
}

void MainScene::handleDone()
{
	CCLog("Game over");

	this->pauseSchedulerAndActions();
	this->disableTouches = true;	
	this->sidebarMenu->setTouchEnabled(false);

	CCPoint p = this->doneMessage->position;

	int x =  20, y =  20;
	CCLayer* lay = CCLayer::create();
	ccColor3B colorBlack = ccc3(0, 0, 0);
	
	CCLabelTTF *gameOver = CCLabelTTF::create("Game Over", GAME_FONT, 24);
	gameOver->setColor(colorBlack);
	lay->addChild(gameOver);
	gameOver->getTexture()->setAliasTexParameters();	

	CCMenuItemFont* cont = CCMenuItemFont::create("Continue", this, menu_selector(MainScene::handleClickContinue));
	cont->setColor(colorBlack);	
	CCMenuItemFont *restart = CCMenuItemFont::create("Restart", this, menu_selector(MainScene::handleClickReset));
	restart->setColor(colorBlack);
	CCMenuItemFont *main = CCMenuItemFont::create("Main menu", this, menu_selector(MainScene::handleClickMenu));
	main->setColor(colorBlack);

	CCMenu *menu = NULL;
	if (this->pointsCount >= this->gameLevel.minScore)
		menu = CCMenu::create(cont, restart, main, NULL);
	else
		menu = CCMenu::create(restart, main, NULL);

	menu->setPosition(ccp(0, -80));
	menu->alignItemsVertically();

	lay->addChild(menu);

	lay->setPosition(p);
	this->doneMessage->getMessageLayer()->addChild(lay);
}

void MainScene::handleClickContinue(CCObject* sender)
{
	//totalWonPoints = this->pointsCount;
	totalPoints += this->pointsCount;
	currentLevel++;

	CCUserDefault::sharedUserDefault()->setIntegerForKey(SETTING_TOTAL_POINTS, totalPoints);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(SETTING_LEVEL_CURRENT, currentLevel);

	this->handleClickReset(NULL);
}

void MainScene::handleDoneExit()
{
	this->pauseSchedulerAndActions();
	this->disableTouches = false;	
	this->sidebarMenu->setTouchEnabled(true);
}

bool MainScene::isMatrixFull()
{
	for (int i = 0; i < this->columns; i++)
    {
        if (this->matrix[this->rows - 1][i] != NULL)
        {
			return true;
		}
	}
	return false;
}

void MainScene::insertRowFromBottom()
{
    //	first check if game is ended
    //	basically if last row is filled then we end
	if (this->isMatrixFull())
	{
		CCLog("EXIT SCENE");

		this->doneMessage = CCGameMessage::create(this, callfunc_selector(MainScene::handleDone), callfunc_selector(MainScene::handleDoneExit));			
		this->doneMessage->showMessageBox();						

		return;
    }

    //	now insert from bottom
    CCGemSprite* tmp[MAX_ROWS][MAX_COLS] = {0};

    for (int i = 0; i < this->columns; i++)
    {
        this->spareRow[i]->stopAllActions();
        this->spareRow[i]->setOpacity(255);
        tmp[0][i] = this->spareRow[i];
    }

    int maxColumnHeight = 0;
    for (int i = 1; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            tmp[i][j] = this->matrix[i-1][j];

            //	row height check
            if (tmp[i][j] != NULL)
            {
                if (i > maxColumnHeight)
                    maxColumnHeight = i;
            }
        }
    }

    memcpy(this->matrix, tmp, sizeof(this->matrix));
    	
    this->redrawMatrix();
    
    this->initSpareRow();
	this->timerCount = gameLevel.insertRowTime;
}

void MainScene::updateProgress()
{
	this->pointsProgress->setPercentage( (float) this->pointsCount / (float) this->gameLevel.minScore * 100);
}

void MainScene::startTipDiscovery()
{
	CCLog("Start tip discovery");

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			if (this->matrix[i][j] != NULL)
			{
				this->foundItems.clear();
				this->processItem(i, j);
				if (this->foundItems.size() >= 3)
				{
					CCLog("Found tip");

					for (unsigned int m = 0; m < this->foundItems.size(); m++)
					{
						MATRIXPOS po = this->foundItems[m];
						CCGemSprite* s = this->matrix[po.row][po.col];
						int scale = s->getScaleX();

						CCEaseInOut *e1 = CCEaseInOut::create(CCScaleTo::create(0.3f, GEM_SCALE + 0.05, GEM_SCALE + 0.05), 1);						
						CCEaseInOut *e2 = CCEaseInOut::create(CCScaleTo::create(0.3f, GEM_SCALE, GEM_SCALE), 1);
						CCRepeat *rep = CCRepeat::create(CCSequence::createWithTwoActions(e1, e2), 3);
						
						s->runAction(rep);
					}

					return;
				}
			}
		}
	}
}

void MainScene::processBomb(unsigned int row, unsigned int col)
{
	CCLog("PRocess bomb %dx%d", row, col);

	this->hideItem(row, col);
	unsigned int counter = 0;

	if (this->matrix[row + 1][col])		//	top
	{
		if (this->matrix[row + 1][col]->getTag() == BOMB_ID)
			this->processBomb(row + 1, col);
		else
		{
			this->hideItem(row + 1, col);		
			counter++;
		}
	}
	if (this->matrix[row][col + 1])		//	right
	{
		if (this->matrix[row][col + 1]->getTag() == BOMB_ID)
			this->processBomb(row, col + 1);
		else
		{
			this->hideItem(row, col + 1);
			counter++;
		}
	}
	if (row > 0 && this->matrix[row - 1][col])		//	bottom
	{
		if (this->matrix[row -1 ][col]->getTag() == BOMB_ID)
			this->processBomb(row - 1, col);
		else
		{
			this->hideItem(row - 1, col);	
			counter++;
		}
	}
	if (col > 0 && this->matrix[row][col - 1])		//	left
	{
		if (this->matrix[row][col - 1]->getTag() == BOMB_ID)
			this->processBomb(row, col - 1);
		else
		{
			this->hideItem(row, col - 1);
			counter++;
		}
	}

	/*
		 *		  1*2
		* *  -->  ***
		 *		  3*4
	*/
	
	//	1
	if (col > 0 && this->matrix[row + 1][col - 1])
	{
		if (this->matrix[row + 1][col - 1]->getTag() == BOMB_ID)
			this->processBomb(row + 1, col - 1);
		else
		{
			this->hideItem(row + 1, col - 1);
			counter++;
		}
	}
	//	2
	if (this->matrix[row + 1][col + 1])
	{
		if (this->matrix[row + 1][col + 1]->getTag() == BOMB_ID)
			this->processBomb(row + 1, col + 1);
		else
		{
			this->hideItem(row + 1, col + 1);
			counter++;
		}
	}
	//	3
	if (row > 0 && col > 0 && this->matrix[row - 1][col - 1])
	{
		if (this->matrix[row - 1][col - 1]->getTag() == BOMB_ID)
			this->processBomb(row - 1, col - 1);
		else
		{
			this->hideItem(row - 1, col - 1);
			counter++;
		}
	}
	//	4
	if (row > 0 && this->matrix[row - 1][col + 1])
	{
		if (this->matrix[row - 1][col + 1]->getTag() == BOMB_ID)
			this->processBomb(row - 1, col + 1);
		else
		{
			this->hideItem(row - 1, col + 1);
			counter++;
		}
	}	

	this->timerTip = 0;

	int won = counter * GEM_POINS * BOMB_FACTOR;
	this->pointsCount += won;
	this->updateProgress();
	
	char tmp[50];
	sprintf(tmp, "%d points", this->pointsCount);
	this->pointsLabel->setString(tmp);

	sprintf(tmp, "%d bonus points!", won);
	this->showBonusMessage(tmp);

	this->reorganizeMatrix();
	//this->redrawMatrix();
	
	//	shake it baby
	this->makeShakeEffect();
}

void MainScene::makeShakeEffect()
{
	CCLog("Make shake effect");

	shakeTimer = 0;
	this->schedule(schedule_selector(MainScene::shakeTick), 1 / 60);
}

void MainScene::shakeTick(float delta)
{
	CCLog("ShakeTICK");
	shakeTimer += delta;

	float x = 0, y = 0, factor = 8;

	x = this->gameContent->getPositionX();
	y = this->gameContent->getPositionY();

	if (CCUtils::randomInInterval(0, 1) == 0)
	{
		x -= CCUtils::randomInInterval(1, factor);
		y -= CCUtils::randomInInterval(1, factor);
	}
	else
	{
		x += CCUtils::randomInInterval(1, factor);
		y += CCUtils::randomInInterval(1, factor);
	}
		
	this->setPosition(x, y);

	if (shakeTimer >= BOMB_SHAKE)
	{
		CCLog("Unschedule shake");
		this->unschedule(schedule_selector(MainScene::shakeTick));
		this->setPosition(0, 0);
	}
}

void MainScene::processTimeArtifact()
{
	CCLog("ProcessTimeArtifact()");
	
	showBonusMessage("15s EXTRA TIME!");
	this->totalTime -= 15;
	if (this->totalTime < 0)
		this->totalTime = 0;
}