#include "SnakeGame.h"

USING_NS_CC;

SnakeGame::SnakeGame() : food(NULL), snakeDir(RIGHT), snakeTailPos(Point(0, 0))
{

}

SnakeGame::~SnakeGame()
{

}

Scene* SnakeGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SnakeGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SnakeGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	//随机数种子
	srand((unsigned)time(NULL));

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...
    // 
	log("ouput message>>>>>>>>>>>>>>>>>>>>>");
	//创建一个键盘监听器
	auto kbdListener = EventListenerTouchOneByOne::create();
	kbdListener->setSwallowTouches(true);
	kbdListener->onTouchBegan = [this](Touch *touch, Event *event)
	{
		//当前触摸的位置,模拟为一个精灵
		auto pressTarget = static_cast<Sprite *>(event->getCurrentTarget());
		Point tempP = pressTarget->convertToNodeSpace(touch->getLocation());

		//触摸位置的矩形
		Size targetSize = pressTarget->getContentSize();
		Rect targetRect = Rect(0, 0, targetSize.width, targetSize.height);

		auto winSize = Director::getInstance()->getVisibleSize();
		
		//判定触摸位置是不是在触摸的精灵内
		if (targetRect.containsPoint(tempP	))
		{
			if (winSize.height / winSize.width * tempP.x + tempP.y > winSize.height)
			{
				if (winSize.height / winSize.width * tempP.x < tempP.y)
				{
					log("Press Up!");
					if (snakeDir != DOWN)
					{
						this->commonMove(UP, 1);
					}
				}
				else
				{
					log("Press Right!");
					if (snakeDir != LEFT)
					{
						this->commonMove(RIGHT, 1);
					}
				}
			}
			else
			{
				if (winSize.height / winSize.width * tempP.x < tempP.y)
				{
					log("Press Left!");
					if (snakeDir != RIGHT)
					{
						this->commonMove(LEFT, 1);
					}
				}
				else
				{
					log("Press Down!");
					if (snakeDir != UP)
					{
						this->commonMove(DOWN, 1);
					}
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	};
    
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kbdListener, this);

	//加入snake
	this->food = Sprite::create("snake_head.png");
	this->food->setColor(Color3B::GREEN);
	this->food->setPosition(Point(60, 200));
	this->addChild(this->food);

	auto head = Sprite::create("snake_head.png");
	head->setColor(Color3B::RED);
	head->setPosition(Point(220, 100));
	this->snake.push_back(head);
	this->addChild(snake.at(0));

	snakeDir = LEFT;
	schedule(schedule_selector(SnakeGame::snakeMove), 0.3f);

    return true;
}

Rect SnakeGame::getRectZone(Node *node)
{
	return Rect(node->getPositionX() - node->getContentSize().width * node->getAnchorPoint().x, 
		node->getPositionY() - node->getContentSize().height * node->getAnchorPoint().y, 
		node->getContentSize().width - 5, node->getContentSize().height - 5);;
}

//检测边缘，或者是吃到自己
bool SnakeGame::checkBoundsOrKillSelf()
{
	Point headPos = snake.at(0)->getPosition();
	for (int i = 1; i < snake.size(); i++)
	{
		if (headPos == snake.at(i)->getPosition())
		{
			return false;
		}
	}

	Size winSize = Director::getInstance()->getVisibleSize();
	if (headPos.x < 10 || headPos.x > winSize.width - 10)
	{
		return false;
	}
	else if (headPos.y < 10 || headPos.y > winSize.height - 10)
	{
		return false;
	}
	return true;
}

// 检测生成的随机位置是否可以用（即不在蛇本身身体上）
bool SnakeGame::checkLocation(Point p)
{
	for (int i = 0; i < this->snake.size(); i++)
	{
		Point nodePos = snake.at(i)->getPosition();
		if (nodePos == p)
		{
			return false;
		}
	}
	return true;
}

//食物随机移动
void SnakeGame::foodRandMove()
{
	log("Food Moving");

	Size spriteSize = snake.at(0)->getContentSize();
	Size winSize = Director::getInstance()->getVisibleSize();
	//获取随机的横纵上限
	int hgtLimit = winSize.height / spriteSize.height;
	int widLimit = winSize.width / spriteSize.width;

	//随机位置

	Point randomLoc = Point(0, 0);
	while (true)
	{
		randomLoc = Point(spriteSize.width * (rand() % (widLimit - 2) + 1),
							spriteSize.height * (rand() % (hgtLimit - 2) + 1));

		log("Food Loc [%f][%f]", randomLoc.x, randomLoc.y);
		if (this->checkLocation(randomLoc))
		{
			break;
		}
	}
	food->setPosition(randomLoc);
}

//吃掉食物，身体长长，食物移动
void SnakeGame::snakeEat()
{
	Sprite *head = snake.at(0);

	//计算头处于的矩形区域
	Rect headRect = getRectZone(head);
	Rect foodRect = getRectZone(food);
	//如果相交,吃到了这个食物,食物变动位置，自身加长一节
	if ((headRect).intersectsRect(foodRect))
	{
		log("eating it!");
		auto newTail = Sprite::create("snake_head.png");
		newTail->setColor(Color3B::YELLOW);
		log("back snakepos [%f][%f]", snakeTailPos.x, snakeTailPos.y);
		newTail->setPosition(snakeTailPos);
		this->snake.push_back(newTail);
		this->addChild(snake.at(snake.size() - 1));

		//食物移动位置
		this->foodRandMove();
	}
}

void SnakeGame::showGameOver()
{
	LabelTTF *msg = LabelTTF::create("GameOver", "Arial", 32);
	msg->setPosition(Director::getInstance()->getVisibleSize().width / 2,
					Director::getInstance()->getVisibleSize().height / 3 * 2);
	this->addChild(msg);

	unschedule(schedule_selector(SnakeGame::snakeMove));
}

//通用的移动方法
void SnakeGame::commonMove(DIR dir, float speed)
{
	
	if (!this->checkBoundsOrKillSelf())
	{
		log("Game Over");
		this->showGameOver();
		return ;
	}
	 //当前第一个节点的位置
	Sprite *head = snake.at(0);
	Point currPos = head->getPosition();
	Point nextPos = Point(0, 0);

	Sprite *temp = this->snake.back();
	snakeTailPos = temp->getPosition();
	log("pre snakepos [%f][%f]", snakeTailPos.x, snakeTailPos.y);
	//根据传入的dir设置蛇当前的dir
	snakeDir = dir;
	switch (dir)
	{
	case LEFT:
		{
			nextPos = (currPos + Point(-head->getContentSize().width, 0));
		}
		break;
	case RIGHT:
		{
			nextPos = (currPos + Point(head->getContentSize().width, 0));
		}
		break;
	case UP:
		{
			nextPos = (currPos + Point(0, head->getContentSize().height));
		}
		break;
	case DOWN:
		{
			nextPos = (currPos + Point(0, -head->getContentSize().height));
		}
		break;
	default:
		break;
	}
	//头部主导整个身体的方向
	snake.at(0)->setPosition(nextPos);

	for (int i = snake.size() - 1; i >= 1; i--)
	{
		if (i == 1)//如果是第一个节点，需要移动到蛇头的位置
		{
			nextPos = currPos;
		}//其他的节点只需要移动到它前面的节点位置即可
		else
		{
			nextPos = snake.at(i - 1)->getPosition();
		}
		snake.at(i)->setPosition(nextPos);
		log("snake[%d]node moving [%f][%f]", i, nextPos.x, nextPos.y);
	}

	//检测是否吃到
	this->snakeEat();

}

void SnakeGame::snakeMove(float delayTime)
{
	log("snake moving....");
	this->commonMove(snakeDir, 1);
}

void SnakeGame::menuCloseCallback(Ref* pSender)
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
