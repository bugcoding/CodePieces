#include "GameScene.h"

USING_NS_CC;

#define LOG_FUN		log("[%s]", __FUNCTION__)

GameScene::GameScene()
{
	highestScore = UserDefault::sharedUserDefault()->getIntegerForKey("high_score");
	currSocre = 0;
}
GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameScene::setFirstTouchPos(Point pos)
{
	touchFirstPos = pos;
	log("fisrt touch [%f, %f]", touchFirstPos.x, touchFirstPos.y);
}

void GameScene::createCards(Size winSize)
{
	int cardWid = (winSize.width - 30) / ROW;
	int cardHgt = (winSize.height - 135) / COL;


	//初始化整个卡片数组，全部置0，并摆放好位置
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			Cards *cardTemp = Cards::create(0, Size(cardWid - 10, cardHgt - 10), Point(cardWid * i + 25, cardHgt * j + 125), Color4B(200,190,180, 255));
			this->addChild((Sprite *)cardTemp);
			cardsMatrix[i][j] = cardTemp;
		}
	}
}

int GameScene::scrollLeft()
{
	LOG_FUN;
	int leftScore = 0;
	for (int i = 0; i < COL; i++)
	{
		for (int j = 0; j < ROW; j++)
		{
			for (int k = j + 1; k < ROW; k++)
			{
				if (cardsMatrix[k][i]->getCardNum() > 0)
				{
					if (cardsMatrix[j][i]->getCardNum() <= 0)
					{
						cardsMatrix[j][i]->setCardNum(cardsMatrix[k][i]->getCardNum());
						cardsMatrix[k][i]->setCardNum(0);

						j--;
					}
					else if (cardsMatrix[j][i]->getCardNum() == cardsMatrix[k][i]->getCardNum())
					{
						cardsMatrix[j][i]->setCardNum(cardsMatrix[k][i]->getCardNum() * 2);
						leftScore += cardsMatrix[j][i]->getCardNum();
						cardsMatrix[k][i]->setCardNum(0);
					}
					break;
				}
			}
		}
	}

	return leftScore;
}
int GameScene::scrollUp()
{
	LOG_FUN;
	int upScore = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = COL - 1; j >= 0; j--)
		{
			for (int k = j - 1; k >= 0; k--)
			{
				if (cardsMatrix[i][k]->getCardNum() > 0)
				{
					if (cardsMatrix[i][j]->getCardNum() <= 0)
					{
						cardsMatrix[i][j]->setCardNum(cardsMatrix[i][k]->getCardNum());
						cardsMatrix[i][k]->setCardNum(0);

						j++;
					}
					else if (cardsMatrix[i][j]->getCardNum() == cardsMatrix[i][k]->getCardNum())
					{
						cardsMatrix[i][j]->setCardNum(cardsMatrix[i][k]->getCardNum() * 2);
						upScore += cardsMatrix[i][j]->getCardNum();
						cardsMatrix[i][k]->setCardNum(0);
					}
					break;
				}
			}
		}
	}
	return upScore;
}
int GameScene::scrollRight()
{
	LOG_FUN;
	int rightScore = 0;
	for (int i = 0; i < COL; i++)
	{
		for (int j = ROW - 1; j >= 0; j--)
		{
			for (int k = j - 1; k >= 0; k--)
			{
				if (cardsMatrix[k][i]->getCardNum() > 0)
				{
					if (cardsMatrix[j][i]->getCardNum() <= 0)
					{
						cardsMatrix[j][i]->setCardNum(cardsMatrix[k][i]->getCardNum());
						cardsMatrix[k][i]->setCardNum(0);
						j++;
					}
					else if (cardsMatrix[j][i]->getCardNum() == cardsMatrix[k][i]->getCardNum())
					{
						cardsMatrix[j][i]->setCardNum(cardsMatrix[k][i]->getCardNum() * 2);
						rightScore += cardsMatrix[j][i]->getCardNum();
						cardsMatrix[k][i]->setCardNum(0);
					}
					break;
				}
			}
		}
	}
	return rightScore;
}
int GameScene::scrollDown()
{
	LOG_FUN;
	int downScore = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			for (int k = j + 1; k < COL; k++)
			{
				if (cardsMatrix[i][k]->getCardNum() > 0)
				{
					if (cardsMatrix[i][j]->getCardNum() <= 0)
					{
						cardsMatrix[i][j]->setCardNum(cardsMatrix[i][k]->getCardNum());
						cardsMatrix[i][k]->setCardNum(0);
						
						j--;
					}
					else if (cardsMatrix[i][j]->getCardNum() == cardsMatrix[i][k]->getCardNum())
					{
						cardsMatrix[i][j]->setCardNum(cardsMatrix[i][k]->getCardNum() * 2);
						downScore += cardsMatrix[i][j]->getCardNum();
						cardsMatrix[i][k]->setCardNum(0);

					}
					break;
				}
			}
		}
	}
	return downScore;
}
//设置分数
void GameScene::setScore()
{
	currScoreLabel->setString(String::createWithFormat("Score: %d", currSocre)->getCString());
	//如果当前分数大于历史最高分数，直接覆盖
	if (currSocre > highestScore)
	{
		highestScore = currSocre;
		highestScoreLabel->setString(String::createWithFormat("Highest: %d", highestScore)->getCString());
		UserDefault::sharedUserDefault()->setIntegerForKey("high_score", highestScore);
	}
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B::GRAY) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
	this->setColor(Color3B::GRAY);

	//二个分数的label
	currScoreLabel = LabelTTF::create(String::createWithFormat("Score: %d", 0)->getCString(), "Arial", 30);
	this->addChild(currScoreLabel);
	currScoreLabel->setPosition(Point(80, 50));

	highestScoreLabel = LabelTTF::create(String::createWithFormat("Highest: %d", highestScore)->getCString(),
											"Arial", 30);
	this->addChild(highestScoreLabel);
	highestScoreLabel->setPosition(currScoreLabel->getPosition() + Point(currScoreLabel->getContentSize().width / 2 + 
									highestScoreLabel->getContentSize().width / 2 + 55, 0));
	currScoreLabel->setColor(Color3B::BLACK);
	highestScoreLabel->setColor(Color3B::BLACK);

    /////////////////////////////
    // 3. add your codes below...
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = [this](Touch *touches, Event *events)
	{
		//获取触摸的位置
		this->setFirstTouchPos(touches->getLocation());
		return true;
	};

	touchListener->onTouchEnded = [this](Touch *touches, Event *events)
	{
		Point endTouchPos = touches->getLocation();
		log("end touch [%f, %f]", endTouchPos.x, endTouchPos.y);

		//先确定是左右滑动，还是上下滑动
		if (abs(static_cast<long>(touchFirstPos.x - endTouchPos.x)) > 
			abs(static_cast<long>(touchFirstPos.y) - endTouchPos.y))
		{
			//横向滑动比纵向大，确定为左右滑动
			if (touchFirstPos.x > endTouchPos.x)
			{
				//向左
				currSocre += scrollLeft();
			}
			else
			{
				//向右
				currSocre += scrollRight();
			}
		}
		else
		{
			//纵向比横向大，为上下滑动
			if (touchFirstPos.y > endTouchPos.y)
			{
				//向下
				currSocre += scrollDown();
			}
			else
			{
				// 向上
				currSocre += scrollUp();
			}
		}
		//设置分数
		this->setScore();
		//移动后检测是否能继续
		this->checkGameCanGo();
		//每次移动都产生一个新的随机数字
		this->randomCreateCard();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), this);

	this->createCards(visibleSize);
	//初始产生二个卡片
	this->randomCreateCard();
	this->randomCreateCard();
    return true;
}

bool GameScene::checkGameCanGo()
{
	bool isOver = true;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			//有空位，相临有相等的情，可以继续游戏
			if (cardsMatrix[i][j]->getCardNum() == 0 || 
				(i > 0 && cardsMatrix[i][j]->getCardNum() == cardsMatrix[i - 1][j]->getCardNum())||
				(i < ROW - 1 && cardsMatrix[i][j]->getCardNum() == cardsMatrix[i + 1][j]->getCardNum()) ||
				(j > 0 && cardsMatrix[i][j]->getCardNum() == cardsMatrix[i][j - 1]->getCardNum()) ||
				(j < COL - 1 && cardsMatrix[i][j]->getCardNum() == cardsMatrix[i][j + 1]->getCardNum()))
			{
				isOver = false;
			}
		}
	}

	if (isOver)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1, GameScene::createScene()));
	}
	return isOver;
}

void GameScene::randomCreateCard()
{
	int row = CCRANDOM_0_1() * 4;
	int col = CCRANDOM_0_1() * 4;

	//产生随机位置并填入数字，如果位置有数，再次随机
	if (cardsMatrix[row][col]->getCardNum() > 0)
	{
		randomCreateCard();
	}
	else
	{
		cardsMatrix[row][col]->setCardNum((CCRANDOM_0_1() * 10 > 1 ? 2 : 4));
	}
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
