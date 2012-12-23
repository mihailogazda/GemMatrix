#include "cocos2d.h"
//#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameResources.h"
#include "utils.h"
#include "LevelLoader.h"

using namespace cocos2d;

#define BOMB_ID 4
#define BOMB_FACTOR 5
#define BOMB_SHAKE 0.6
#define GEM_POINS 10
#define BONUS_FACTOR 10

#define TIME_ID 5

static const char* g_gemNames[] = 
{
	"Gem Blue.png",
	"Gem Orange.png",
	"Gem Green.png",
    "Gem Gray.png",
	"bomb.png",
	"Key.png",
	"Rock.png",
	NULL
};

extern IOSTYPE phoneType;

static int lastTag = -1;
static int sameLastTagCount = 0;

class CCGemSprite : public CCSprite
{
public:



	static CCGemSprite* create(bool includeRock = false)
	{
		extern GAMELEVEL globalCurrentLevel;

		int r = 0;
		if (CCUtils::uniformProbabilityBombs(globalCurrentLevel.bombProbability))
			r = BOMB_ID;

		if (r == 0 && CCUtils::uniformProbabilityTime(globalCurrentLevel.timeProbability) )
			r = TIME_ID;

		if (!(r == TIME_ID || r == BOMB_ID))
			r = CCUtils::randomInInterval(0, 3);

		//	never generate more than 3 of the same item so close
		if (lastTag == r)
		{
			sameLastTagCount++;
			if (sameLastTagCount == 4)
			{
				CCLog("Sprite %d generated 3 times in a row, skipping...", r);
				return CCGemSprite::create(includeRock);
			}
		}
		else
		{
			lastTag = r;
			sameLastTagCount = 0;
		}

		const char* gemname = g_gemNames[r];

		CCLog("Spawning sprite (%d) %s", r,  gemname);
        CCGemSprite* t = (CCGemSprite*) CCSprite::createWithSpriteFrameName(gemname);
		t->setTag(r);		    

        if (phoneType == IPAD3)        			
            GEM_SCALE = GEM_SCALE_IPAD3;        
        else if (phoneType == IPHONE4)                    
			GEM_SCALE = GEM_SCALE_IPHONE4;
                
        t->setScale(GEM_SCALE);
		return t;
	}

};