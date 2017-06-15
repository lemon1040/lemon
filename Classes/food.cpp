#include"food.h"
USING_NS_CC;

thefood* thefood::create(const char *_ptr,int Tag)
{
	thefood* food = new thefood();
	if (food && food->init())
	{
		food->autorelease();
		food->foodinit(_ptr,Tag);
		return food;
	}
	CC_SAFE_DELETE(food);
	return NULL;

}

void thefood::foodinit(const char *_ptr,int Tag)
{
	auto food = Sprite::create(_ptr);
	food->setTag(Tag);
	auto body = PhysicsBody::createCircle(food->getContentSize().width / 2);
	body->setContactTestBitmask(0xFFFFFFFF);//½Ó´¥²âÊÔÑÚÂë
	//body->setCollisionBitmask(0x01);//Åö×²ÑÚÂë
	food->setPhysicsBody(body);
	addChild(food);
}

bool thefood::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}