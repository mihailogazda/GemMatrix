#include "cocos2d.h"
//#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameResources.h"
#include "utils.h"

using namespace cocos2d;

#define BOMB_ID 4
#define BOMB_FACTOR 5
#define BOMB_SHAKE 0.5
#define GEM_POINS 10
#define BONUS_FACTOR 10

static const char* g_gemNames[] = 
{
	"Gem Blue.png",
	"Gem Orange.png",
	"Gem Green.png",
    "Gem Gray.png",
	"bomb.png",
	"Rock.png"
};

extern IOSTYPE phoneType;

class CCGemSprite : public CCSprite
{
public:

	static CCGemSprite* create(bool includeRock = false)
	{		
		//int r = CCUtils::randomInInterval(0, 2); //CCUtils::uniformRandomGems();
		//if (includeRock && CCUtils::randomInInterval(0, 60) == 0)
		//	r = 3;
		int r = CCUtils::uniformRandomGems();
		if (CCUtils::uniformProbabilityRock())
			r = BOMB_ID;

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