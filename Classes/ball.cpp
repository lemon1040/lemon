#include"ball.h"
USING_NS_CC;

ball* ball::createball(const char *_ptr,int Tag)
{
	ball* _player = new ball();
	if (_player && _player->init())
	{
		_player->autorelease();
		_player->ballinit(_ptr,Tag);
		return _player;
	}
	CC_SAFE_DELETE(_player);
	return NULL;

}

void ball::ballinit(const char *_ptr,int Tag)
{
	speed = 500;
	score = 0;
	auto _player = Sprite::create(_ptr);
	_player->setTag(Tag);
	auto body = PhysicsBody::createCircle(_player->getContentSize().width / 2);
	body->setContactTestBitmask(0xFF);//½Ó´¥²âÊÔÑÚÂë
	//body->setCollisionBitmask(0x01);//Åö×²ÑÚÂë
	size = _player->getContentSize().width;
	log("size:%f", _player->getContentSize().width);
	_player->setPhysicsBody(body);
	addChild(_player);
}

void ball::setPosition(const cocos2d::Vec2& newPosition)
{
	Sprite::setPosition(newPosition);
	Sprite::setAnchorPoint(Vec2(0.5f, 0.5f));
}
bool ball::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

