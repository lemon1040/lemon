#pragma once
#include "cocos2d.h"
USING_NS_CC;
class ball :public cocos2d::Sprite
{
public:
	double speed;
	double size;
	double score;
	double pastscore = 0;
	int life = 4;
	int Tag = 1000;
	cocos2d::MoveTo* moveUp;
	cocos2d::MoveTo* moveDown;
	cocos2d::MoveTo* moveLeft;
	cocos2d::MoveTo* moveRight;

	static ball* createball(const char *_ptr,int Tag);
	void ballinit(const char *_ptr,int Tag);
	void setPosition(const cocos2d::Vec2& newPosition);
	virtual bool init();
	CREATE_FUNC(ball);
};

