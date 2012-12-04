#include "cocos2d.h"
//#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameResources.h"
#include "utils.h"

using namespace cocos2d;

#ifdef __MACH__
    static float GEM_SCALE = 0.5f;
#else
    static float GEM_SCALE = 0.4f;
#endif

#define GEM_WIDTH	100 * GEM_SCALE
#define GEM_HEIGHT	120 * GEM_SCALE
#define GEM_SPACING	10 * GEM_SCALE

#define SPARE_ROW_FADE_START 50

#define GEM_SCALE_IPAD3		1.0f
#define GEM_SCALE_IPHONE4	0.7f

static const char* g_gemNames[] = 
{
	"Gem Blue.png",
	"Gem Orange.png",
	"Gem Green.png"
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