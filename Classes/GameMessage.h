#ifndef __GAME_MESSAGE_BOX_H__
#define __GAME_MESSAGE_BOX_H__

#include "cocos2d.h"

/*
 *	Game message popup.
 *	forward SEL_CallFunc handler to process layer recieved by getMessageLayer().
 *	use exitCallback to know when closed.
 *	use create()
*/

class CCGameMessage : public CCObject
{
	private:

		bool messageShowing;
	
		CCLayer* parent;
		CCLayer* messageParent;
		CCLayer* messageLayer;
		
		//	use these to link directly to actions inc cocoa, e.g. CCMenuItemFont touch or similar
		SEL_CallFunc callback;
		SEL_CallFunc exitCallback;		

		CCGameMessage(CCLayer* parent, SEL_CallFunc callback)
		{
			this->messageShowing = false;
			
			this->parent = parent;
			this->callback = callback;
			this->exitCallback = NULL;

			this->sel_hideMessageBox = menu_selector(CCGameMessage::hideMessageBox);
			this->sel_showMessageBox = menu_selector(CCGameMessage::showMessageBox);
		}

	public:

		static CCGameMessage* create(CCLayer* parent, SEL_CallFunc callback)
		{
			return new CCGameMessage(parent,  callback);
		}

		static CCGameMessage* create(CCLayer* parent, SEL_CallFunc callback, SEL_CallFunc exitCallback)
		{
			CCGameMessage* p = new CCGameMessage(parent, callback);
			p->exitCallback = exitCallback;
			return p;
		}

		CCLayer *getMessageLayer()
		{
			return this->messageLayer;
		}

		SEL_MenuHandler sel_hideMessageBox;
		SEL_MenuHandler sel_showMessageBox;

		void showMessageBox(CCObject *sender = NULL)
		{
			if (this->messageShowing)
				return;    

			this->messageShowing = true;
	
			messageParent = CCLayer::create();
			messageLayer = CCLayer::create();

			CCLayerColor* darkBack = CCLayerColor::create(ccc4(0, 0, 0, 255 * 0.5));
			messageParent->addChild(darkBack);	

			int g_width = CCDirector::sharedDirector()->getWinSize().width;
			int g_height = CCDirector::sharedDirector()->getWinSize().height;

			CCSprite* levelList = CCSprite::createWithSpriteFrameName("levelList.png");
			levelList->setPosition(ccp(g_width / 2, g_height / 2));
			messageLayer->addChild(levelList);
			messageLayer->setScale(0);

			messageParent->addChild(messageLayer);
			parent->addChild(messageParent, 100);
			
			CCCallFunc *cf1 = CCCallFunc::create(parent, callback);
			CCSequence *seq = CCSequence::createWithTwoActions(CCEaseBounceOut::create(CCScaleTo::create(0.5, 1, 1)), cf1);
			messageLayer->runAction(seq);
		}

		void hideMessageBox(CCObject *sender = NULL)
		{
			CCCallFunc *cf1 = CCCallFunc::create(this, callfunc_selector(CCGameMessage::handleHideCleanup));
			CCSequence *seq = CCSequence::createWithTwoActions(CCEaseBounceIn::create(CCScaleTo::create(0.5, 0, 0)), cf1);
			messageLayer->runAction(seq);
		}

		void handleHideCleanup()
		{
			messageParent->removeAllChildrenWithCleanup(true);
			messageParent->removeFromParent();
			this->messageShowing = false;
			
			if (this->exitCallback)
				parent->runAction(CCCallFunc::create(parent, this->exitCallback));

			
		}
};


#endif