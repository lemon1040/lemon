#pragma once
#include "cocos2d.h"
USING_NS_CC;
class ball :public cocos2d::Sprite
{
public:
	double speed;
	double size;
	int score;

	static ball* createball(const char *_ptr,int Tag);
	void ballinit(const char *_ptr,int Tag);
	void setPosition(const cocos2d::Vec2& newPosition);
	virtual bool init();
	CREATE_FUNC(ball);
};

