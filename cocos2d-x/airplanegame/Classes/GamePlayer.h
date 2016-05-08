#ifndef _GAME_PLAYER_H
#define _GAME_PLAYER_H

#include "cocos2d.h"
#include "GameMap.h"
USING_NS_CC;

class GamePlayer : public CCSprite, public CCTargetedTouchDelegate
{
private:
	bool PlayerIsSelect;
public:
	CCSprite *m_player;
	GamePlayer();
	~GamePlayer();
	
	static GamePlayer *shareGamePlayer();
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(GamePlayer);

};

#endif