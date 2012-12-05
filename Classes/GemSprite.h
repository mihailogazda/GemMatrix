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
	/*,	"Rock.png"*/
};

extern IOSTYPE phoneType;

class CCGemSprite : public CCSprite
{
public:

	//int currentType;	
	static CCGemSprite* create()
	{
		int count = sizeof(g_gemNames) / sizeof(g_gemNames[0]);        
		int r = CCUtils::randomInInterval(0, count - 1);
    
        CCLog("Spawning sprite (%d) %s", r,  g_gemNames[r]);
        CCGemSprite* t = (CCGemSprite*) CCSprite::createWithSpriteFrameName(g_gemNames[r]);
		t->setTag(r);
    

        if (phoneType == IPAD3)        			
            GEM_SCALE = GEM_SCALE_IPAD3;        
        else if (phoneType == IPHONE4)                    
			GEM_SCALE = GEM_SCALE_IPHONE4;
        
        
        t->setScale(GEM_SCALE);
		return t;
	}

};