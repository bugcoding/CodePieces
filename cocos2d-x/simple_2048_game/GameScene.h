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

	//�������ҷ��ÿ�Ƭ
	void createCards(cocos2d::Size winSize = cocos2d::Size(0, 0));

private:
	//��һ�δ�����λ��
	cocos2d::Point touchFirstPos;

	//������Ƭ���ڵ�ƽ��
	Cards *cardsMatrix[ROW][COL];


	//��������߷�
	int currSocre;
	int highestScore;

	//��ʾ��߷��뵱ǰ����
	cocos2d::LabelTTF *currScoreLabel;
	cocos2d::LabelTTF *highestScoreLabel;


private:
	//���õ�һ�δ�����λ��
	void setFirstTouchPos(cocos2d::Point pos);

	//���󻬶�
	int scrollLeft();

	//���һ���
	int scrollRight();

	//���»���
	int scrollDown();

	// ���ϻ���
	int scrollUp();

	//����Զ�������Ƭ
	void randomCreateCard();

	//�����Ϸ�ܷ����
	bool checkGameCanGo();

	//�������ڵķ�������߷�
	void setScore();

};

#endif // __HELLOWORLD_SCENE_H__
