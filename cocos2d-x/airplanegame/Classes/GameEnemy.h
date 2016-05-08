#ifndef _GAME_ENEMY_H
#define _GAME_ENEMY_H

#include "cocos2d.h"
USING_NS_CC;

class GameEnemy : public CCLayer
{
	//敌机数组
	CC_SYNTHESIZE(CCArray *, m_Enemys, EnemyArray);
	//敌机sprite
	CC_SYNTHESIZE(CCSprite *, m_EnemySprite, EnemySprite);
public:
	GameEnemy();
	~GameEnemy();
	virtual bool init();
	void createNewEnemy();//创建新的敌人
	
	CREATE_FUNC(GameEnemy);
private:
	
	

	void initEnemy();
	void moveEnemy(float delta);
};

#endif