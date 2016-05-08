#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Cards.h"

#define ROW		4
#define COL		4

class GameScene : public cocos2d::LayerColor
{
public:

	GameScene();
	~GameScene();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	//产出并且放置卡片
	void createCards(cocos2d::Size winSize = cocos2d::Size(0, 0));

private:
	//第一次触摸的位置
	cocos2d::Point touchFirstPos;

	//整个卡片所在的平面
	Cards *cardsMatrix[ROW][COL];


	//分数，最高分
	int currSocre;
	int highestScore;

	//显示最高分与当前分数
	cocos2d::LabelTTF *currScoreLabel;
	cocos2d::LabelTTF *highestScoreLabel;


private:
	//设置第一次触摸的位置
	void setFirstTouchPos(cocos2d::Point pos);

	//向左滑动
	int scrollLeft();

	//向右滑动
	int scrollRight();

	//向下滑动
	int scrollDown();

	// 向上滑动
	int scrollUp();

	//随机自动产生卡片
	void randomCreateCard();

	//检测游戏能否继续
	bool checkGameCanGo();

	//设置现在的分数与最高分
	void setScore();

};

#endif // __HELLOWORLD_SCENE_H__
