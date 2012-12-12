#include "cocos2d.h"
//#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameResources.h"
#include "utils.h"

using namespace cocos2d;

static const char* g_gemNames[] = 
{
	"Gem Blue.png",
	"Gem Orange.png",
	"Gem Green.png",
    "Rock.png"
};

extern IOSTYPE phoneType;

class CCGemSprite : public CCSprite
{
public:

	static CCGemSprite* create(bool includeRock = false)
	{		
		int r = CCUtils::uniformRandomGems();

		if (includeRock && CCUtils::uniformProbabilityRock())
			r = 3;

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