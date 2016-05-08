#include "Cards.h"

USING_NS_CC;

Cards::Cards()
{
	cardNum = 0;
	cardBg = NULL;
	cardNumLabel = NULL;
	cardPos = Point(0, 0);
	cardSize = Size(0, 0);
}
Cards::~Cards()
{

}

Cards *Cards::create(int num, cocos2d::Size cardSize, cocos2d::Point cardPos, cocos2d::Color4B cardColor)
{
	Cards *card = new Cards();
	if (card && card->init())
	{
		card->cardNum = num;
		card->cardPos = cardPos;
		card->cardSize = cardSize;
		card->cardBgColor = cardColor;
		card->initEveryCard();
		card->autorelease();
		return card;	
	}
	CC_SAFE_RELEASE(card);
	return NULL;
}

void Cards::initEveryCard()
{
	this->cardBg = LayerColor::create(this->cardBgColor, this->cardSize.width, this->cardSize.height);
	this->cardBg->setPosition(this->cardPos);
	Size bgSize = this->cardBg->getContentSize();

	this->cardNumLabel = LabelTTF::create("", "Arial", 40);
	this->cardNumLabel->setColor(Color3B::BLACK);
	this->cardNumLabel->setPosition(Point(bgSize.width / 2, bgSize.height / 2));
	this->cardBg->addChild(this->cardNumLabel, 1);

	this->addChild(this->cardBg);

	if (cardNum <= 0)
	{
		this->cardNumLabel->setString("");
	}
	else
	{
		this->cardNumLabel->setString(String::createWithFormat("%d", cardNum)->getCString());
	}
}

bool Cards::init()
{
	//super initial
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

int Cards::getCardNum()
{
	return cardNum;
}

//设置不同数字的背景色
void Cards::changeBgColor()
{
	switch (cardNum)
	{
	case 0:
		cardBg->setColor(Color3B(200,190,180));
		break;
	case 2:
		cardBg->setColor(Color3B(240,230,225)); 
		break;
	case 4:
		cardBg->setColor(Color3B(240,220,205));  
		break;
	case 8:
		cardBg->setColor(Color3B(240,180,120));  
		break;
	case 16:
		cardBg->setColor(Color3B(240,140,95));  
		break;
	case 32:
		cardBg->setColor(Color3B(240,120,95));  
		break;
	case 64:
		cardBg->setColor(Color3B(240,90,60));  
		break;
	case 128:
		cardBg->setColor(Color3B(240,90,60));  
		break;
	case 256:
		cardBg->setColor(Color3B(240,200,70));  
		break;
	case 512:
		cardBg->setColor(Color3B(240,200,70));  
		break;
	case 1024:
		cardBg->setColor(Color3B(0,130,0));  
		break;
	case 2048:
		cardBg->setColor(Color3B(0,130,0));  
		break;

	default:
		break;
	}
}
//设置不同的字体
void Cards::changeFontSize()
{
	if (cardNum >= 0)
	{
		cardNumLabel->setFontSize(40);
	}
	if (cardNum >= 16)
	{
		cardNumLabel->setFontSize(40);
	}
	if (cardNum >= 128)
	{
		cardNumLabel->setFontSize(36);
	}
	if (cardNum >- 1024)
	{
		cardNumLabel->setFontSize(32);
	}
}

//设置卡片label上的数字，如果是0就不显示
void Cards::setCardNum(int num)
{
	cardNum = num;
	//设置不同的数字，显示不同背景与字号
	this->changeBgColor();
	this->changeFontSize();

	if (cardNum > 0)
	{
		cardNumLabel->setString(String::createWithFormat("%d", cardNum)->getCString());
	}
	else
	{
		cardNumLabel->setString("");
	}
}

