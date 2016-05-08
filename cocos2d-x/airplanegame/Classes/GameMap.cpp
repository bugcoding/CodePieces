#include "GameMap.h"
#include "GamePlayer.h"
#include "GameBullet.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define BLUTTE_SPEED 0.15f

GameMap::GameMap(void):scoreLabel(NULL), scores(0),explosionSpr(NULL), 
					m_bullet(NULL), m_enemy(NULL), m_topBar(NULL), resideOpps(3)
{
}

GameMap::~GameMap(void)
{
}

//单例map
static GameMap *map;
GameMap *GameMap::sharedGameMap()
{
	if (!map)
	{
		map = GameMap::create();
	}
	return map;
}

CCScene *GameMap::createGameScence()
{
	//创建地图场景，在delegate中加载
	CCScene *scence = CCScene::create();
	GameMap *map = GameMap::sharedGameMap();
	scence->addChild(map);
	return scence;
}

//根据文件名前缀得到一系列的序列帧动画
//@singleName: 文件名前缀
//@framesNum: 序列帧帧数
//@return  动画对象
CCAnimation* GameMap::explosionAnimation(const char* preName, int framesNum) 
{
	CCAnimation * animation = CCAnimation::create();  
	for (int i = 1; i < framesNum; i++)  
	{
		const char *name = CCString::createWithFormat("%s%d.png", preName, i)->getCString();
		animation->addSpriteFrameWithFileName(name);  
	}
	return animation;
}

void GameMap::update(float dt)
{

	CCArray *bl = m_bullet->getBulletArray();
	for (unsigned int i = 0; i < bl->count(); i++)
	{
		CCSprite *bullet = static_cast<CCSprite *>(bl->objectAtIndex(i));
		//计算子弹所处的区域
		CCRect bulletRect = CCRectMake(bullet->getPositionX() - bullet->getContentSize().width * bullet->getAnchorPoint().x, 
			bullet->getPositionY() - bullet->getContentSize().height * bullet->getAnchorPoint().y, 
			bullet->getContentSize().width - 10, bullet->getContentSize().height - 20);

		//修正坐标系//问题待修正
		bulletRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2 - 10;
		bulletRect.origin.x + 5;
		//此处检测每个敌机和子弹的碰撞
		CCArray *ene = m_enemy->getEnemyArray();
		for (unsigned int j = 0; j < ene->count(); j++)
		{
			CCSprite *existScreenEnemy = static_cast<CCSprite *>(ene->objectAtIndex(j));
			//计算敌机所处的矩形区域
			CCRect eneRect = CCRectMake(existScreenEnemy->getPositionX() - existScreenEnemy->getContentSize().width * existScreenEnemy->getAnchorPoint().x, 
				existScreenEnemy->getPositionY() - existScreenEnemy->getContentSize().height * existScreenEnemy->getAnchorPoint().y, 
				existScreenEnemy->getContentSize().width, existScreenEnemy->getContentSize().height);
			
			//如果二个矩形相交，碰撞
			if (eneRect.intersectsRect(bulletRect))
			{	
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion2.wav");
				CCAnimation * animation = this->explosionAnimation("spr_explosion1_", 7);  
				//每帧的间隔
				animation->setDelayPerUnit(1.2f/20.0f);  
				//animation->setRestoreOriginalFrame(false);  
				//构造成可执行的action
				CCAnimate *eff=CCAnimate::create(animation);  

				//设置第一个单帧
				CCSprite *explosionSpr = CCSprite::create("spr_explosion1_1.png");
				explosionSpr->setPosition(ccpAdd(existScreenEnemy->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2)));
				this->addChild(explosionSpr);
				//动作队列
				CCSequence *seq = CCSequence::create(eff, CCCallFuncO::create(this, callfuncO_selector(GameMap::setExplosionSpr), explosionSpr),NULL);

				explosionSpr->runAction(seq);

				//敌机与子弹碰撞，移除对应的对象
				ene->removeObject(existScreenEnemy);
				bl->removeObject(bullet);
				//移除对应的child
				m_enemy->getEnemySprite()->removeChild(existScreenEnemy, true);
				m_bullet->getBulletSprite()->removeChild(bullet, true);
				//分数加10
				scores += 10;
			}

		}
	}
}

void GameMap::setExplosionSpr(CCObject *obj)
{
	//CCLOG("set explosion status!");
	CCSprite *explosionSpr = (CCSprite *)obj;
	explosionSpr->setVisible(false);
	this->removeChild(explosionSpr, true);
}
//初始化地图上的各种东西 
bool GameMap::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	//小地图铺满
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			CCSprite *temp = CCSprite::create("back_scence.png");
			temp->setAnchorPoint(ccp(0, 0));
			this->addChild(temp);
			temp->setPosition(ccp(temp->getContentSize().width * i, temp->getContentSize().height * j));
		}
	}

	//底总分数显示
	m_topBar = CCSprite::create("spr_bottom.png");
	m_topBar->setScaleX(0.75);
	this->addChild(m_topBar);
	m_topBar->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 
						CCDirector::sharedDirector()->getWinSize().height - m_topBar->getContentSize().height / 2));
	scoreLabel = CCLabelTTF::create(CCString::createWithFormat("%d", scores)->getCString(), "Arial", 17);
	scoreLabel->setColor(ccYELLOW);
	scoreLabel->setPosition(ccpAdd(m_topBar->getPosition(), ccp(-55, 19)));
	this->addChild(scoreLabel);



	//游戏玩家飞机
	GamePlayer *player = GamePlayer::shareGamePlayer();
	this->addChild(player);
	player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));

	//添加子弹
	m_bullet = GameBullet::create();
	m_bullet->setPosition(player->getPosition());
	this->addChild(m_bullet);

	//添加敌机
	m_enemy = GameEnemy::create();
	m_enemy->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
	this->addChild(m_enemy);
	
	//剩余次数
	resideSpr = CCSprite::create("bar_blue.png");
	resideSpr->setAnchorPoint(ccp(0, 0.5));
	resideSpr->setScaleX(1.70);
	resideSpr->setScaleY(0.55);
	resideSpr->setPosition(ccpSub(m_topBar->getPosition(), ccp(m_topBar->getContentSize().width / 4 + 71.5, 12)));
	this->addChild(resideSpr);

	
	//加入定时器，定时刷新
	this->schedule(schedule_selector(GameMap::autoLauchBullet), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::autoCreateEnemy), BLUTTE_SPEED * 10);
	this->schedule(schedule_selector(GameMap::checkPlayerHitEnemy), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::freshScore), BLUTTE_SPEED);
	this->scheduleUpdate();

	this->setTouchEnabled(true);
	return true;
}
//刷新分数
void GameMap::freshScore(float dt)
{
	scoreLabel->setString(CCString::createWithFormat("%d", scores)->getCString());
}

//自动发射子弹
void GameMap::autoLauchBullet(float dt)
{
	m_bullet->createNewBullet();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion3.wav");
}
//检测玩家碰撞
void GameMap::checkPlayerHitEnemy(float dt)
{

	for (int i = 0; i < m_enemy->getEnemyArray()->count(); i++)
	{
		CCSprite *enemy = static_cast<CCSprite *>(m_enemy->getEnemyArray()->objectAtIndex(i));

		GamePlayer *player = GamePlayer::shareGamePlayer();
		CCRect eneRect = CCRectMake(enemy->getPositionX() - enemy->getContentSize().width * enemy->getAnchorPoint().x, 
			enemy->getPositionY() - enemy->getContentSize().height * enemy->getAnchorPoint().y,
			enemy->getContentSize().width, enemy->getContentSize().height);
		CCRect playerRect = CCRectMake(player->getPositionX() - player->getContentSize().width * player->getAnchorPoint().x, 
			player->getPositionY() - player->getContentSize().height * player->getAnchorPoint().y,
			player->getContentSize().width, player->getContentSize().height);

		playerRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2 + 15;
		playerRect.origin.x -= CCDirector::sharedDirector()->getWinSize().width / 2;

		if (eneRect.intersectsRect(playerRect))
		{
			CCLOG("######################HIT#########################");
			m_enemy->getEnemyArray()->removeObject(enemy);
			m_enemy->getEnemySprite()->removeChild(enemy, true);

			CCAnimation * animation = this->explosionAnimation("spr_explosion2_", 7);  
			//每帧的间隔
			animation->setDelayPerUnit(1.2f/20.0f);  
			//animation->setRestoreOriginalFrame(false);  
			//构造成可执行的action
			CCAnimate *eff=CCAnimate::create(animation);  

			//设置第一个单帧
			CCSprite *explosionSpr = CCSprite::create("spr_explosion1_1.png");
			explosionSpr->setPosition(player->getPosition());
			this->addChild(explosionSpr);
			//动作队列
			CCSequence *seq = CCSequence::create(eff, CCCallFuncO::create(this, callfuncO_selector(GameMap::setExplosionSpr), explosionSpr),NULL);

			explosionSpr->runAction(seq);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion2.wav");

			player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));
			player->setScale(0.3);
			player->setScale(1.0);
			CCScaleTo *scaleTo = CCScaleTo::create(1.0, 1.0);
			player->runAction(scaleTo);
			resideOpps--;//每次碰撞，剩余次数减少

			//刷新剩余机会
			freshResideOpps();
		}
	}

}
void GameMap::freshResideOpps()
{
	if (resideOpps == 2)
	{
		CCSprite *t = CCSprite::create("bar_yellow.png");
		resideSpr->setDisplayFrame(t->displayFrame());
		resideSpr->setScaleX(1.0);
	}
	else if (resideOpps == 1)
	{
		CCSprite *t = CCSprite::create("bar_red.png");
		resideSpr->setDisplayFrame(t->displayFrame());
		resideSpr->setScaleX(0.6);
	}
}

//定时创建敌人
void GameMap::autoCreateEnemy(float dt)
{
	m_enemy->createNewEnemy();
}


bool GameMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCLOG("Touch Map!");
	//m_bullet->createNewBullet();
	return true;
}
void GameMap::onEnter()
{
	CCLayer::onEnter();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Battle.mp3", true);
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameMap::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}