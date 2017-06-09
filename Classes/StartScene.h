#ifndef __Start_SCENE_H__
#define __Start_SCENE_H__

#include "cocos2d.h"
#include"HelloWorldScene.h"
#include<math.h>
class Start : public cocos2d::Layer
{
	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _collidable;
	cocos2d::Sprite* _player;

	cocos2d::MoveTo* moveUp;
	cocos2d::MoveTo* moveDown;
	cocos2d::MoveTo* moveLeft;
	cocos2d::MoveTo* moveRight;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void menuCloseCallback(cocos2d::Ref *pSender);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	void setPlayerPosition(cocos2d::Vec2 position);
	void keyboard();
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	// implement the "static create()" method manually
	CREATE_FUNC(Start);
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
