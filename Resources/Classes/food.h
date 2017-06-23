#include "cocos2d.h"
USING_NS_CC;
class thefood :public cocos2d::Sprite
{
public:

	static thefood* create(const char *_ptr,int Tag);
	void foodinit(const char *_ptr,int Tag);
	virtual bool init();
	CREATE_FUNC(thefood);
};
#pragma once
