
#ifndef _GAME_MAP_H
#define _GAME_MAP_H

#include "cocos2d.h"
#include "GameBullet.h"
#include "GameEnemy.h"
USING_NS_CC;

class GameMap : public cocos2d::CCLayer
{
	//顶部栏
	CC_SYNTHESIZE(CCSprite *, m_topBar, TopBar);
public:
	GameMap(void);
	~GameMap(void);

	static GameMap *sharedGameMap();
	virtual bool init();
	static cocos2d::CCScene *createGameScence();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void update(float dt);
	virtual void onEnter();
	virtual void onExit();
	void setExplosionSpr(CCObject *obj);
	CREATE_FUNC(GameMap);
private:
	void freshResideOpps();//刷新剩余生命次数
	void checkPlayerHitEnemy(float dt);
	CCAnimation* explosionAnimation(const char* singleName, int frameNum) ;//创建动画帧序列，爆炸
	void autoCreateEnemy(float dt);//产生敌机
	void autoLauchBullet(float dt);//自动发子弹
	void freshScore(float dt);//刷新分数
	
	
	GameBullet *m_bullet;//地图上存在的子弹
	GameEnemy *m_enemy;//地图上存在的敌机
	CCLabelTTF *scoreLabel;//分数label
	CCSprite *resideSpr;//剩余次数
	CCSprite *explosionSpr;//爆炸第一帧
	int scores;//分数
	int resideOpps;//剩余机会
};


#endif // !_GAME_MAP_H





