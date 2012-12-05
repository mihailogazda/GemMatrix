#include "MainScene.h"
#include "DiedScene.h"
#include "GameLevels.h"

extern int g_height;
extern int g_width;
extern IOSTYPE phoneType;
extern unsigned int currentLevel;

bool transitionInProgress = false;

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
	this->addChild(ps);

	//	init matrix
	memset(this->matrix, 0, sizeof(this->matrix));
	memset(this->spareRow, 0, sizeof(this->spareRow));
    
    this->totalTime = 0;
    this->pointsCount = 0;
    this->timerCount = 0;

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

void MainScene::onEnter()
{
    CCLayer::onEnter();
    CCLog("OnEnter()");
}

bool MainScene::initSidebar()
{
	char* fontName = "Impact";
	unsigned int fontSize = 26;
    
    if (phoneType == IPAD3 || phoneType == IPHONE4)
        fontSize *= 2;

    int gw = 200;
	int xs = g_width;
	int ys = g_height - 30;
    
    sidebar = CCLayerColor::create(ccc4(255, 255, 255, 100));
    sidebar->setPosition(ccp(xs, 0));
    sidebar->setAnchorPoint(ccp(0, 0));

    
    levelLabel = CCLabelTTF::create("Level 1", fontName, fontSize);
    levelLabel->setPosition(ccp(gw / 2, g_height - 50));
    sidebar->addChild(levelLabel);

	//	points won
	this->pointsCount = 0;
	pointsLabel = CCLabelTTF::create("0 Points", fontName, fontSize);
	pointsLabel->setPosition(ccp(gw / 2, g_height - 100));
    sidebar->addChild(pointsLabel);
    
	this->timerCount = 0;
	this->timeLabel = CCLabelTTF::create("0s", fontName, fontSize - 6);
	this->timeLabel->setPosition(ccp(gw / 2, 80));
	sidebar->addChild(this->timeLabel, 1);

	//	game menu
	CCMenuItemFont::setFontName(fontName);
	CCMenuItemFont::setFontSize(fontSize);	
	CCMenuItemFont* reset = CCMenuItemFont::create("Restart", this, menu_selector(MainScene::handleClickReset));
    reset->setPosition(ccp(gw / 2, g_height - 200));
    
	CCMenu *menu = CCMenu::create(reset, NULL);
    menu->setPosition(ccp(0, 0));
    menu->setAnchorPoint(ccp(0, 0));
	sidebar->addChild(menu);
    
    
    //  now button
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(IMG_BUTTON);
    
    CCMenuItemSprite *button = CCMenuItemSprite::create(CCSprite::createWithTexture(tex), CCSprite::createWithTexture(tex), this, menu_selector(MainScene::handleUpButton));
    
    button->setScale(0.7f);
    button->setPosition(ccp(gw / 2, 80));
    menu->addChild(button);
    
    this->addChild(sidebar);
    
    sidebar->runAction(CCEaseBounceOut::create(CCMoveBy::create(1, ccp(-gw, 0))));

	return true;
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

void MainScene::handleUpButton(CCObject* sender)
{
    CCLog("Handle UP button");
    this->insertRowFromBottom();
}

bool MainScene::initTextures()
{
	CCSpriteFrameCache *sfc = CCSpriteFrameCache::sharedSpriteFrameCache();	
	sfc->addSpriteFramesWithFile(IMG_MAIN_TILESET_PLIST);

    // add background
	CCSprite* sp = CCSprite::create(IMG_RED_BACK);
	sp->setAnchorPoint(ccp(0, 0));
	sp->setScale(0.7f);
    
    //if (phoneType == IPAD || phoneType == IPAD3)
    sp->setScale(2.0f);
    
	this->addChild(sp);
	
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
			int stop = this->rows - 3;
			if (i < stop)
			{
				CCGemSprite* gs = CCGemSprite::create();
				gs->setAnchorPoint(ccp(0, 0));			
				this->matrix[i][j] = gs;
				this->addChild(gs);
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
		this->addChild(gs);
	}

	this->redrawSpareRow();
}

CCPoint MainScene::positionForElement(int row, int col, bool isSpareRow = false)
{		
	CCLOG("positionForElement %dx%d", row, col);

	int xs = 30;
	int ys = 30;

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
				this->foundItems.clear();
				this->processItem(i, j);
				this->postProcess();
				return;
			}
		}
	}
}

void MainScene::processItem(int row, int col)
{	
	//	skipp existing elementss	
	for (unsigned int i = 0; i < this->foundItems.size(); i++)
	{
		MATRIXPOS p2 = this->foundItems.at(i);
		if ( p2.col == col && p2.row == row)
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
	if (left && left->getTag() == tag)
		this->processItem(row, col - 1);
	if (right && right->getTag() == tag)
		this->processItem(row, col + 1);
	if (top && top->getTag() == tag)
		this->processItem(row + 1, col);
	if (bottom && bottom->getTag() == tag)
		this->processItem(row - 1, col);
}

void MainScene::postProcess()
{
	int size = this->foundItems.size();
	if (size < 3)
	{
		CCLOG("NOT ENOUGH");
		return;
	}

	this->callbackCount = size;
	this->disableTouches = true;

	//	set points
	this->checkForBonus();

	//	check for bonus

	//	hide all
	for (int i = 0; i < size; i++)
	{
		MATRIXPOS p = this->foundItems.at(i);
		CCLog("Post processing item: %d x %d", p.row, p.col);

		CCGemSprite* sp = this->matrix[p.row][p.col];
		CC_ASSERT(sp);

		//	fade out
		//CCBlink* fo = CCBlink::create(0.6f, 2);		
		CCHide *hi = CCHide::create();
		CCCallFuncN *cf = CCCallFuncN::create(this, callfuncN_selector(MainScene::callbackItemHidden));
		CCAction* seq = CCSequence::create(/*fo,*/ hi, cf, NULL);
		
		//	Add points won per gem
		CCPoint pos = positionForElement(p.row, p.col);
		CCLabelTTF *lab  = CCLabelTTF::create("10", "Impact", 16);
		pos.x += GEM_WIDTH / 2;
		pos.y += GEM_HEIGHT / 2;
		lab->setPosition(pos);
		this->addChild(lab);

		CCFadeOut* fo2 = CCFadeOut::create(0.8f);				
		CCMoveBy* mb2 = CCMoveBy::create(0.4f, ccp(0, 10));

		sp->runAction(seq);
		lab->runAction(mb2);
		lab->runAction(fo2);

		/*
		//http://www.cocos2d-iphone.org/forum/topic/19523
		CCParticleExplosion *ex = CCParticleExplosion::create();		
		ex->setPosition(pos);
		CCTexture2D *tx = CCTextureCache::sharedTextureCache()->addImage(IMG_SNOWFLAKE);
		ex->setTexture(tx);
		this->addChild(ex);
		*/
		//	set zero
		this->matrix[p.row][p.col] = NULL;
	}
}

void MainScene::checkForBonus()
{	
	unsigned int size = this->foundItems.size();
	
	//	now bonus
	unsigned int bonusOver = 5;
	int bonus = size - bonusOver;
	if (bonus > 0)
	{
		int bonusWon = 100 * bonus;
		this->pointsCount += bonusWon;

		char bonusText[100] = "";
		sprintf(bonusText, "%d x BONUS POINTS", bonusWon);
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
		
		CCLabelTTF* bonusLabel = CCLabelTTF::create(bonusText, "Impact", 32);
		bonusLabel->setPosition(ccp(g_width / 2, g_height / 2));
		bonusLabel->setOpacity(0);
		this->addChild(bonusLabel);

		bonusLabel->runAction(CCSequence::create(
			CCFadeTo::create(0.2f, 255), 
			CCScaleTo::create(0.5, 2),
			//CCMoveTo::create(0.5, this->pointsLabel->getPosition()),
			CCFadeTo::create(0.5, 0),
			NULL));
	}

	//	set points
	this->pointsCount += size * 10;
	char points[100] = "";
	sprintf(points, "%d Points", this->pointsCount);
	this->pointsLabel->setString(points);
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
	CCLog("Handle Time Update");
    
    if (!this->wasInit)
    {
        this->postInit();
        this->wasInit = true;
    }

	//	check for new element insertion
	this->timerCount++;

	char tmp[100];
	sprintf(tmp, "%ds", this->timerCount);
	this->timeLabel->setString(tmp);

	int secTillReset = GAME_LEVELS[currentLevel].insertRowTime;
	float fade = SPARE_ROW_FADE_START + this->timerCount * (SPARE_ROW_FADE_END / secTillReset);
	if (fade > SPARE_ROW_FADE_END)
		fade = SPARE_ROW_FADE_END;


	for (int i = 0; i < this->columns; i++)
		this->spareRow[i]->runAction( CCFadeTo::create(1, fade) );

	if (this->timerCount >= secTillReset)
	{
		this->disableTouches = true;

        this->insertRowFromBottom();
    
		this->disableTouches = false;
	}
}

void MainScene::insertRowFromBottom()
{
    //	first check if game is ended
    //	basically if last row is filled then we end
    for (int i = 0; i < this->columns; i++)
    {
        if (this->matrix[this->rows - 1][i] != NULL)
        {
            CCLog("EXIT SCENE");
            CCScene* died = DiedScene::scene();
            CCDirector::sharedDirector()->replaceScene( CCTransitionFadeBL::create(1, died) );
            return;
        }
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
    this->timerCount = 0;
}