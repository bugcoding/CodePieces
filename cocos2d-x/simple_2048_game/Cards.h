
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

	//��ȡ��Ƭ�ϵ�����
	int getCardNum();
	
	//���ÿ�Ƭ�ϵ�����
	void setCardNum(int num);
	
	//��ʼ��ÿһ����Ƭ
	void initEveryCard();

	//���ݲ�ͬ���֣����ò�ͬ�ı���ɫ
	void changeBgColor();

	//���ò�ͬ���ֵ�����
	void changeFontSize();

private:
	//��Ƭ�ϵ�����
	int cardNum;

	//��ʾ����
	cocos2d::LabelTTF *cardNumLabel;
	
	//��Ƭ��С
	cocos2d::Size cardSize;
	
	//��Ƭλ��
	cocos2d::Point cardPos;
	
	//��Ƭ����
	cocos2d::LayerColor *cardBg;
	
	//��Ƭ������ɫ
	cocos2d::Color4B cardBgColor;
};


#endif
