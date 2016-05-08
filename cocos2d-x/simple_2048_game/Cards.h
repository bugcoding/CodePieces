
//author: by bugcode
//time:   2014-5-2

#ifndef _CARDS_H_
#define _CARDS_H_

#include "cocos2d.h"

class Cards : cocos2d::Sprite
{
public:

	Cards();
	~Cards();

	static Cards *create(int num, cocos2d::Size cardSize, cocos2d::Point cardPos, cocos2d::Color4B cardColor);
	bool init();

	//获取卡片上的数字
	int getCardNum();
	
	//设置卡片上的数字
	void setCardNum(int num);
	
	//初始化每一个卡片
	void initEveryCard();

	//根据不同数字，设置不同的背景色
	void changeBgColor();

	//设置不同数字的字体
	void changeFontSize();

private:
	//卡片上的数字
	int cardNum;

	//显示数字
	cocos2d::LabelTTF *cardNumLabel;
	
	//卡片大小
	cocos2d::Size cardSize;
	
	//卡片位置
	cocos2d::Point cardPos;
	
	//卡片背景
	cocos2d::LayerColor *cardBg;
	
	//卡片背景颜色
	cocos2d::Color4B cardBgColor;
};


#endif
