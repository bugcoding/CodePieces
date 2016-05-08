#ifndef _GAME_START_H
#define _GAME_START_H

#include "cocos2d.h"
USING_NS_CC;

class GameStart : public cocos2d::CCLayer
{
private:
	CCLabelTTF *startFont;//¡°start¡±
	CCLabelTTF *gameTitle;//¡°ÓÎÏ·Ãû³Æ¡±
public:
	GameStart();
	~GameStart();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	static CCScene *createGameStartScene();
	CREATE_FUNC(GameStart);
};

#endif