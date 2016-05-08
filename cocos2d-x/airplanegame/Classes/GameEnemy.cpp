#include "GameEnemy.h"
#include "GameMap.h"
#include "GamePlayer.h"

GameEnemy::GameEnemy() : m_Enemys(NULL), m_EnemySprite(NULL)
{

}
GameEnemy::~GameEnemy()
{
	CC_SAFE_RELEASE(m_Enemys);
}


bool GameEnemy::init()
{
	if (!CCLayer::init())
		return false;
	initEnemy();
	return true;
}
void GameEnemy::initEnemy()
{
	m_Enemys = CCArray::create();
	m_Enemys->retain();

	//创建敌机贴图
	m_EnemySprite = CCSprite::create();
	m_EnemySprite->setPosition(CCPointZero);
	this->addChild(m_EnemySprite);

	//移动敌机
	schedule(schedule_selector(GameEnemy::moveEnemy));
}
void GameEnemy::createNewEnemy()
{
	//CCLOG("New Enemy");
	srand((unsigned)time(NULL));
	//随机某个敌机贴图
	unsigned int seq = rand() % 3 + 1;
	CCString *eneyName = CCString::createWithFormat("spr_enemy%d.png", seq);
	CCSprite *newEnemy = CCSprite::create(eneyName->getCString());

	//敌机出现的位置
	int randPosX = rand() % static_cast<int>(CCDirector::sharedDirector()->getWinSize().width / 2  - 50) + 10;
	//CCLOG("RandPosX == %d", randPosX);
	int ar[2] = {1, -1};


	newEnemy->setPosition(ccp(randPosX * ar[rand() % 2], CCDirector::sharedDirector()->getWinSize().height - GameMap::sharedGameMap()->getTopBar()->getContentSize().height - 330));
	m_EnemySprite->addChild(newEnemy);
	m_Enemys->addObject(newEnemy);

}
void GameEnemy::moveEnemy(float delta)
{
	for (int i = 0; i < m_Enemys->count(); i++)
	{
		CCSprite *enemy = static_cast<CCSprite *>(m_Enemys->objectAtIndex(i));
		enemy->setPositionY(enemy->getPositionY() - 80 * delta);
		//敌机移动中检测是否与玩家碰撞


		if (enemy->getPositionY() < - CCDirector::sharedDirector()->getWinSize().height / 2 - 10)
		{
			//CCLOG("Enemy Out Of Bounds");
			m_Enemys->removeObjectAtIndex(i);
			m_EnemySprite->removeChild(enemy, true);
		}
	}
}