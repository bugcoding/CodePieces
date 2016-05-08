#include "GameStart.h"
#include "GameMap.h"


GameStart::GameStart() : gameTitle(NULL), startFont(NULL)
{

}
GameStart::~GameStart()
{

}

bool GameStart::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//背景
	CCSprite *bck = CCSprite::create("start_game.png");
	bck->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(bck);

	//标题
	gameTitle = CCLabelTTF::create("---Airplane Game---", "Arial", 32);
	gameTitle->setColor(ccYELLOW);
	this->addChild(gameTitle);
	gameTitle->setPosition(ccp(winSize.width / 2, winSize.height / 5 * 4));

	startFont = CCLabelTTF::create("Start!", "Arial", 28);
	startFont->setColor(ccRED);
	this->addChild(startFont);
	startFont->setPosition(ccp(winSize.width / 2, winSize.height / 4));

	//”开始“的特效
	CCScaleTo *fir = CCScaleTo::create(0.2f,0.8f);
	CCScaleTo *sec = CCScaleTo::create(0.3f, 0.7f);
	CCScaleTo *thi = CCScaleTo::create(0.1f, 0.9f);
	CCScaleTo *fou = CCScaleTo::create(0.2f, 1.0f);

	CCSequence *seq = CCSequence::create(fir, sec, thi, fou, NULL);
	CCRepeatForever *forever = CCRepeatForever::create(seq);

	startFont->runAction(forever);

}
//创建启动游戏场景
CCScene *GameStart::createGameStartScene()
{
	CCScene *s = CCScene::create();
	GameStart *startScene = GameStart::create();
	s->addChild(startScene);
	return s;
}

void GameStart::onEnter()
{
	CCLayer::onEnter();
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameStart::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameStart::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void GameStart::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint p = pTouch->getLocation();

	//计算“开始”的矩形区域
	CCRect rec = CCRectMake(startFont->getPositionX() - startFont->getContentSize().width * startFont->getAnchorPoint().x, 
						startFont->getPositionY() - startFont->getContentSize().height * startFont->getAnchorPoint().y, 
						startFont->getContentSize().width, startFont->getContentSize().height);

	//如果是点中start字体，进入游戏
	if (rec.containsPoint(p))
	{
		CCScene *secne = GameMap::createGameScence();
		CCTransitionScene *effSec = CCTransitionCrossFade::create(0.8f, secne);
		CCDirector::sharedDirector()->replaceScene(effSec);
	}

}