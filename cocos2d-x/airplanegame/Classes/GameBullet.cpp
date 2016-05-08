#include "GameBullet.h"
#include "GamePlayer.h"

GameBullet::GameBullet() : m_Bullets(NULL), m_BulltesSprite(NULL)
{

}

GameBullet::~GameBullet()
{
	//释放对应的子弹数组
	CC_SAFE_RELEASE(m_Bullets);
}

bool GameBullet::init()
{
	if (!CCLayer::init())
		return false;
	initBullet();
	return true;
}

void GameBullet::initBullet()
{
	
	//构建子弹数组
	m_Bullets = CCArray::create();
	m_Bullets->retain();

	//构建子弹贴图

	m_BulltesSprite = CCSpriteBatchNode::create("spr_bullet.png");
	m_BulltesSprite->setPosition(CCPointZero);
	this->addChild(m_BulltesSprite);
	
	//定时移动子弹
	schedule(schedule_selector(GameBullet::moveBullet));
}

//产生新的子弹
void GameBullet::createNewBullet()
{
	//CCLOG("Create New Bullet");
	CCSprite *newBullet = CCSprite::createWithTexture(m_BulltesSprite->getTexture());
	newBullet->setPosition(ccpSub(GamePlayer::shareGamePlayer()->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width / 2, -8)));
	m_BulltesSprite->addChild(newBullet);

	m_Bullets->addObject(newBullet);
}

void GameBullet::moveBullet(float delay)
{
	for (unsigned int i = 0; i < m_Bullets->count(); i++)
	{
		CCSprite *bullet = static_cast<CCSprite *>(m_Bullets->objectAtIndex(i));

		//移动新的位置
		bullet->setPositionY(bullet->getPositionY() + 200.0 * delay);
		
		if (bullet->getPositionY() >= CCDirector::sharedDirector()->getWinSize().height - 
			GameMap::sharedGameMap()->getTopBar()->getContentSize().height * 2 + 30)
		{
			//CCLOG("Bullets Out Of Bounds");
			m_Bullets->removeObjectAtIndex(i);
			m_BulltesSprite->removeChild(bullet, true);
		}
	}
}