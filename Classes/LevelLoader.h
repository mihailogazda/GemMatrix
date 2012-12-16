#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "cocos2d.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "GameResources.h"

typedef struct 
{
	unsigned int timeout;
	unsigned int insertRowTime;
	unsigned int minScore;
	unsigned int substractRows;
	unsigned int secondsToTip;
	static const unsigned int tipPause = 10;
	bool hasRocks;
	bool checksInvalid;
	bool valid;
} GAMELEVEL, *LPGAMELEVEL;


#ifndef MAX_PATH
#define MAX_PATH 255
#endif

//	singleton class
class LevelLoader;
static LevelLoader* p_sharedLoader = NULL;

//
//	Enables level loading from resource XML file
//
class LevelLoader 
{
private:

	char path[MAX_PATH];
	xmlDocPtr sharedDoc;

	LevelLoader()
	{
		memset(path, 0, sizeof(path));
		sharedDoc = NULL;
    
    #ifdef __MACH__
    
        const char* resDir = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(LEVEL_RESOURCE);
    
        //char tmp[MAX_PATH];
        //sprintf(tmp, "%s%s", resDir, LEVEL_RESOURCE);
        CCLog(resDir);
    
		sharedDoc = xmlReadFile(resDir, "utf-8", XML_PARSE_RECOVER);
    
    #else
		sharedDoc = xmlReadFile(LEVEL_RESOURCE, "utf-8", XML_PARSE_RECOVER);
    #endif
    
		CCAssert(sharedDoc, "XML Levels document not loaded");
	}

	~LevelLoader()
	{
		xmlFreeDoc(this->sharedDoc);
	}

	//	populates level structure from XMLNode
	GAMELEVEL populateFromNode(xmlNodePtr node)
	{
		GAMELEVEL l = {0};
		if (!node)
		{
			l.valid = false;
			return l;
		}

		xmlNodePtr currNode = node->children;
		const xmlChar* cont = NULL;		
		unsigned int foundCounter = 0;

		while (currNode != NULL)
		{
			if (xmlStrcmp(currNode->name, (const xmlChar*) "timeout") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				l.timeout = atoi((const char*) cont);
				if (l.timeout != 0)
					foundCounter++;
			}
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "insertRowTime") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				l.insertRowTime = atoi((const char*) cont);
				if (l.insertRowTime != 0)
					foundCounter++;
			}
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "minScore") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				l.minScore = atoi((const char*) cont);
				if (l.minScore != 0)
					foundCounter++;
			}
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "substractRows") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				l.substractRows = atoi((const char*) cont);
				if (l.substractRows != 0)
					foundCounter++;
			}
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "secondsToTip") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				l.secondsToTip = atoi((const char*) cont);
				if (l.secondsToTip != 0)
					foundCounter++;
			}
			//
			//	not counting in successfull - optional
			//
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "hasRocks") == 0)
			{
				cont = xmlNodeGetContent(currNode);				
				CCUtils::trim((char*) cont);
				if (xmlStrcmp(cont, (const xmlChar*) "true") == 0)				
					l.hasRocks = true;
			}
			else if (xmlStrcmp(currNode->name, (const xmlChar*) "checksInvalid") == 0)
			{
				cont = xmlNodeGetContent(currNode);
				CCUtils::trim((char*) cont);
				if (xmlStrcmp(cont, (const xmlChar*) "true") == 0)
					l.checksInvalid = true;
			}
			
			
			currNode = currNode->next;
		}

		if (foundCounter >= 3)
			l.valid = true;

		if (l.secondsToTip == 0)
			l.secondsToTip = 5;

		return l;
	}

public:

	static LevelLoader* sharedLoader()
	{
		if (!p_sharedLoader)
		{
			p_sharedLoader = new LevelLoader();
			return p_sharedLoader;
		}
		return p_sharedLoader;
	}

	unsigned int getLevelsCount()
	{		
		xmlNodePtr root = xmlDocGetRootElement(sharedDoc);				
		return xmlChildElementCount(root);		
	}

	GAMELEVEL getGameLevel(unsigned int level)
	{
		GAMELEVEL p = {0};
		xmlNodePtr root = xmlDocGetRootElement(sharedDoc);

		xmlNodePtr currNode = root->children;
		xmlNodePtr foundNode = NULL;
		unsigned int index = 0;		

		while (currNode != NULL)
		{
			if (xmlStrcmp(currNode->name, (const xmlChar*) "level") == 0)
			{
				//CCLog("Name %s at %d", currNode->name, index);
				if (index + 1 == level)
				{
					foundNode = currNode;
					break;
				}
				index++;				
			}
			currNode = currNode->next;
		}

		return populateFromNode(foundNode);
	}


};


#endif