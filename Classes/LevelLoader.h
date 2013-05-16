#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include <cocos2d.h>
#include <tinyxml2.h>
#include "GameResources.h"

typedef struct 
{
	unsigned int timeout;
	unsigned int insertRowTime;
	unsigned int minScore;
	unsigned int medScore;
	unsigned int maxScore;
	unsigned int substractRows;
	unsigned int secondsToTip;
	unsigned int bonusOver;
	static const unsigned int tipPause = 10;	
	int bombProbability;
	int timeProbability;
	bool checksInvalid;
	bool upButtonHidden;
	bool valid;
} GAMELEVEL;

#ifndef MAX_PATH
	#define MAX_PATH 255
#endif

//	singleton class
class LevelLoader;
static LevelLoader* p_sharedLoader = NULL;

static const char* LEVEL_TAG = "level";
static const char* LEVELS_TAG = "levels";


//
//	Enables level loading from resource XML file
//
class LevelLoader
{
private:

	tinyxml2::XMLDocument *sharedDoc;
	tinyxml2::XMLNode* sharedLevels;

	LevelLoader()
	{
		sharedDoc = NULL;
		sharedLevels = NULL;
	}


	static LevelLoader* create()
	{
		LevelLoader* l = new LevelLoader();
		if (l && l->init())
			return l;
		CC_SAFE_DELETE(l);
		return NULL;
	}

	bool init()
	{
		sharedDoc = new tinyxml2::XMLDocument();
		if (!sharedDoc)
			return false;

		//CCUserDefault::sharedUserDefault()->

		tinyxml2::XMLError err = sharedDoc->LoadFile(LEVEL_RESOURCE);
		if (err != tinyxml2::XML_NO_ERROR)
		{
			CC_SAFE_DELETE(sharedDoc);
			return false;
		}

		sharedLevels = sharedDoc->FirstChildElement(LEVELS_TAG);
		if (sharedLevels == NULL)
		{
			CC_SAFE_DELETE(sharedDoc);
			return false;
		}

		return true;
	}

	static const char* _toa(tinyxml2::XMLNode* node)
	{
		if (node)
		{
			tinyxml2::XMLElement* u = node->ToElement();
			if (u)
				return u->GetText();
		}
		return "";
	}

	static int _toi(tinyxml2::XMLNode* node)
	{
		if (!node)
			return 0;
		const char* content = _toa(node);
		return atoi(content);
	}

	static bool _tob(tinyxml2::XMLNode* node)
	{
		if (node)
		{			
			const char* content = _toa(node);
			if (strcmpi(content, "true") == 0)
				return true;
		}
		return false;
	}

	//	populates level structure from XMLNode
	GAMELEVEL populateFromNode(tinyxml2::XMLNode* node)
	{
		GAMELEVEL l = {0};
		if (!node)
			return l;

		const char* content = NULL;
		unsigned int foundCounter = 0;

		///
		///	INTEGERS
		///
		tinyxml2::XMLNode* nptr = node->FirstChildElement("timeout");
		if (nptr)
		{			
			l.timeout = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("insertRowTime");
		if (nptr)
		{			
			l.insertRowTime = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("minScore");
		if (nptr)
		{			
			l.minScore = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("medScore");
		if (nptr)
		{
			l.medScore = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("maxScore");
		if (nptr)
		{			
			l.maxScore = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("substractRows");
		if (nptr)
		{			
			l.substractRows = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("secondsToTip");
		if (nptr)
		{			
			l.secondsToTip = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("bombProbability");
		if (nptr)
		{			
			l.bombProbability = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("timeProbability");
		if (nptr)
		{			
			l.timeProbability = _toi(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("bonusOver");
		if (nptr)
		{			
			l.bonusOver = _toi(nptr);
			foundCounter++;
		}

		///
		///	BOOLEAN
		///
		nptr = node->FirstChildElement("checksInvalid");
		if (nptr)
		{			
			l.checksInvalid = _tob(nptr);
			foundCounter++;
		}

		nptr = node->FirstChildElement("upButtonHidden");
		if (nptr)
		{			
			l.checksInvalid = _tob(nptr);
			foundCounter++;
		}

		///
		///	Set default values
		///
		if (foundCounter >= 3)
			l.valid = true;		

		if (l.secondsToTip == 0)
			l.secondsToTip = 5;

		if (l.substractRows == 0)
			l.substractRows = 3;

		if (l.bonusOver == 0)
			l.bonusOver = 4;

		return l;
	}

public:

	static LevelLoader* sharedLoader()
	{
		if (!p_sharedLoader)
		{
			p_sharedLoader = LevelLoader::create();
			return p_sharedLoader;
		}
		return p_sharedLoader;
	}
	
	~LevelLoader()
	{	
		sharedLevels = NULL;
		CC_SAFE_DELETE(sharedDoc);
	}

	unsigned int getLevelsCount()
	{
		unsigned int levelCount = 0;
		if (sharedLevels != NULL)
		{
			tinyxml2::XMLNode* nptr = sharedLevels->FirstChildElement(LEVEL_TAG);
			while (nptr != NULL)
			{
				nptr = nptr->NextSiblingElement(LEVEL_TAG);
				++levelCount;
			}
		}
		return levelCount;
	}

	GAMELEVEL getGameLevel(unsigned int level)
	{
		GAMELEVEL p = {0};
		
		//	First find XML element
		int counter = 0;
		int FOUND = -100;

		tinyxml2::XMLNode* nptr = sharedLevels->FirstChildElement(LEVEL_TAG);
		while (nptr != NULL)
		{
			if (counter == level - 1)
			{
				counter = FOUND; // flag success find
				break;
			}

			nptr = nptr->NextSiblingElement(LEVEL_TAG);			
			counter++;
		}

		if (counter == FOUND)
		{
			GAMELEVEL p2 = populateFromNode(nptr);
			if (p2.valid)
				p = p2;
		}

		return p;

	}


};


#endif