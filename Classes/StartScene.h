#ifndef __Start_SCENE_H__
#define __Start_SCENE_H__

#include "cocos2d.h"
#include"HelloWorldScene.h"
#include"ball.h"
#include"food.h"
#include"Music.h"
#include<math.h>
#define random(x) (rand()%x)
#define playerTag 1
#define enemyTag 2
#define foodScore 2
using namespace std;
class Start : public cocos2d::Layer
{
	int foodTag = 3;
	int enemylife = 3;
	int playerlife = 3;

	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _collidable;
	ball* _player;
	ball* _enemy;
	vector<thefood*>food;
	vector<ball*>P_group;
	vector<ball*>E_group;
	vector<Sprite*>P_life;
	vector<Sprite*>E_life;
	cocos2d::EventListenerPhysicsContact *contactListener;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void menucloseCallback(cocos2d::Ref *pSender);
	void menuresetCallback(Ref *pSender);
	//鼠标控制
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	//键盘控制
	void keyboard();
	//初始化食物
	void createfood(int n,const char *_ptr);
	//检验进食与更新食物
	void eatfood(thefood* food1, ball* player);
	void feed(float dt);
	void _update(ball* player,double score);
	void kill(float dt);
	void onekill(ball*player, ball*group);
	void resetenemy();
	void resetplayer();
	double Distance(Vec2 pos1, Vec2 pos2);
	//分裂
	void division(ball* player, vector<ball*>&P_group);//分裂后主球会获得3秒的加速
	void slowplayer(float dt);
	void slowenemy(float dt);
	//检验边界问题
	void stopaction(float dt);


	void setPlayerPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	// implement the "static create()" method manually
	CREATE_FUNC(Start);
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
