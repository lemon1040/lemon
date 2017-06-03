#include "StartScene.h"
#include<math.h>
USING_NS_CC;

Scene* Start::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Start::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Start::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设置瓦片地图
	_tileMap = TMXTiledMap::create("Map.tmx");
	addChild(_tileMap, 0, 100);

	TMXObjectGroup* group = _tileMap->getObjectGroup("objects");
	ValueMap spawnPoint = group->getObject("ball");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();

	_player = Sprite::create("ball.png");
	_player->setPosition(Vec2(x, y));
	addChild(_player, 2, 200);
	_collidable = _tileMap->getLayer("collidable");
	_collidable->setVisible(false);

	//关闭按钮
	auto pbutton3 = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Start::menuCloseCallback, this));
	auto closemenu = Menu::create(pbutton3, NULL);
	closemenu->setPosition(2180, 30);
	addChild(closemenu);
	//鼠标控制
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

void Start::menuCloseCallback(Ref *pSender)
{
	Director::getInstance()->end();
}
bool Start::onTouchBegan(Touch* touch, Event* event)
{
	log("onTouchBegan");
	return true;
}

void Start::onTouchMoved(Touch *touch, Event *event)
{
	log("onTouchMoved");
	//Vec2 touchLocation = touch->getLocation();
	//log("touchLocation (%f ,%f) ", touchLocation.x, touchLocation.y);
	//Vec2 playerPos = _player->getPosition();
	//Vec2 diff = touchLocation - playerPos;
	//playerPos.x += _tileMap->getTileSize().width*diff.x/sqrt(diff.x*diff.x+diff.y*diff.y);
	//playerPos.y += _tileMap->getTileSize().height*diff.y / sqrt(diff.x*diff.x + diff.y*diff.y);

	//log("playerPos (%f ,%f) ", playerPos.x, playerPos.y);
	//this->setPlayerPosition(playerPos);

}

void Start::onTouchEnded(Touch *touch, Event *event)
{
	log("onTouchEnded");
	Vec2 touchLocation = touch->getLocation();
	_player->runAction(MoveTo::create(0.5, touchLocation));
}
void Start::setPlayerPosition(Vec2 position)
{
	Vec2 tileCoord = this->tileCoordFromPosition(position);
	int tileGid = _collidable->getTileGIDAt(tileCoord);

	if (tileGid > 0) {
		Value prop = _tileMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string collision = propValueMap["Collidable"].asString();

		if (collision == "true") { 
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("empty.wav");
			return;
		}
	}

	_player->setPosition(position);
}

Vec2 Start::tileCoordFromPosition(Vec2 pos)
{
	int x = pos.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;
	return Vec2(x, y);
}

