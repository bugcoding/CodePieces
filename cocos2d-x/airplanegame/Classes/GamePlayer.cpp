
#include "GamePlayer.h"

GamePlayer::GamePlayer():PlayerIsSelect(false)
{
	//CCLOG("GamePlayer Constructor!");
}

GamePlayer::~GamePlayer()
{
	//CCLOG("GamePlayer Destructor!");
	
}

//进入
void GamePlayer::onEnter()
{
	//CCLOG("GamePlayer OnEnter");
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}
//退出
void GamePlayer::onExit()
{
	//CCLOG("GamePlayer OnExit");
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

static GamePlayer *player;
//单例，一个player
GamePlayer *GamePlayer::shareGamePlayer()
{
	if (!player)
	{
		player = GamePlayer::create();
		CCSprite *t = CCSprite::create("spr_plane.png");
		player->setDisplayFrame(t->displayFrame());
	}
	return player;
}
//点中player移动
void GamePlayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	//CCLOG("GamePlayer Is Moving!");
	CCPoint lastPoint = touch->getLocationInView();//获取对应视图上的精灵位置
	lastPoint = CCDirector::sharedDirector()->convertToGL(lastPoint);//转化成gl坐标
	//X轴范围限制
	(lastPoint.x < this->getContentSize().width / 2) && (lastPoint.x = this->getContentSize().width / 2);
	(lastPoint.x > CCDirector::sharedDirector()->getWinSize().width - this->getContentSize().width / 2)
	&& (lastPoint.x = CCDirector::sharedDirector()->getWinSize().width - this->getContentSize().width / 2);
	//Y轴范围限制
	(lastPoint.y < this->getContentSize().height / 2) && (lastPoint.y = this->getContentSize().height / 2);
	(lastPoint.y > CCDirector::sharedDirector()->getWinSize().height - this->getContentSize().height / 2 
		- GameMap::sharedGameMap()->getTopBar()->getContentSize().height)
	&& (lastPoint.y = CCDirector::sharedDirector()->getWinSize().height - this->getContentSize().height / 2
	- GameMap::sharedGameMap()->getTopBar()->getContentSize().height);


	if (this->PlayerIsSelect)//如果选中了player
		this->setPosition(lastPoint);//更新player位置
}
//开始点击
bool GamePlayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	//CCLOG("GamePlayer Is Press!");
	
	CCPoint temp = touch->getLocation();
	CCPoint p = convertToNodeSpace(temp);
	//算出GamePlayer的所在矩形
	CCRect rec = CCRectMake(	getPositionX() - getContentSize().width * getAnchorPoint().x,
							getPositionY() - getContentSize().height * getAnchorPoint().y,
							getContentSize().width, getContentSize().height); 
	rec.origin = CCPointZero;
	if (rec.containsPoint(p))
	{
		this->PlayerIsSelect = true;
	}
	
	return true;
}
void GamePlayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCLOG("Reset Select Status");
	//重置选中状态
	this->PlayerIsSelect = false;
	
}