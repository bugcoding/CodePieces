#ifndef __SnakeGame_SCENE_H__
#define __SnakeGame_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum{LEFT, RIGHT, UP, DOWN} DIR;

class SnakeGame : public cocos2d::Layer
{
public:

	SnakeGame();
	~SnakeGame();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	// implement the "static create()" method manually
	CREATE_FUNC(SnakeGame);

private:
	//蛇的容器
	std::vector<Sprite *> snake;
	//每次出现的食物
	Sprite *food;
	//当前蛇的方向
	DIR snakeDir;
	//每次移动维护一个最后节点的位置
	Point snakeTailPos;
private:
	//通用的移动方法
	void commonMove(DIR dir, float speed);
	//蛇移动
	void snakeMove(float delayTime);
	//蛇吃食物
	void snakeEat();
	//获取node所在的矩形区域
	Rect getRectZone(Node *node);
	//变动食物位置
	void foodRandMove();
	//检测生成的随机位置是否合法
	bool checkLocation(Point p);
	//检测是否出界，或者吃到自己
	bool checkBoundsOrKillSelf();
	//显示结束消息
	void SnakeGame::showGameOver();
};

#endif // __SnakeGame_SCENE_H__
