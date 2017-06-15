#ifndef __Start_SCENE_H__
#define __Start_SCENE_H__

#include "cocos2d.h"
#include"HelloWorldScene.h"
#include"ball.h"
#include"food.h"
#include<math.h>
#define random(x) (rand()%x)
#define playerTag 1
#define enemyTag 2
using namespace std;
class Start : public cocos2d::Layer
{
	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _collidable;
	ball* _player;
	ball* _enemy;
	int foodTag = 3;
	vector<thefood*>food;
	cocos2d::MoveTo* moveUp;
	cocos2d::MoveTo* moveDown;
	cocos2d::MoveTo* moveLeft;
	cocos2d::MoveTo* moveRight;
	cocos2d::EventListenerPhysicsContact *contactListener;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void menuCloseCallback(cocos2d::Ref *pSender);
	//鼠标控制
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	//键盘控制
	void keyboard();
	//初始化食物
	void createfood(int n,const char *_ptr);
	//检验进食与更新食物
	void eat_update(float dt);
	void eatfood(thefood* food1, ball* player);
	void _update(ball* player);

	double Distance(Vec2 pos1, Vec2 pos2);
	void setPlayerPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	// implement the "static create()" method manually
	CREATE_FUNC(Start);
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
