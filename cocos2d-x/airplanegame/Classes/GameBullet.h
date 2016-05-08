#ifndef _GAME_BULLET_H
#define _GAME_BULLET_H

#include "cocos2d.h"

USING_NS_CC;

class GameBullet : public CCLayer
{
	//放所有的子弹，在屏幕上的
	CC_SYNTHESIZE(CCArray *, m_Bullets, BulletArray);
	//构建子弹sprite BatchNode
	CC_SYNTHESIZE(CCSpriteBatchNode *, m_BulltesSprite, BulletSprite);
public:
	GameBullet();
	~GameBullet();
	CREATE_FUNC(GameBullet);
	virtual bool init();
	void createNewBullet();//创建一个新的子弹

private:

	void initBullet();//初始化子弹
	void moveBullet(float delay);//移动子弹

};


#endif