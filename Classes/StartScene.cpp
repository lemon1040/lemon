#include "StartScene.h"
USING_NS_CC;

Scene* Start::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//phyWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	PhysicsWorld* phyWorld = scene->getPhysicsWorld();
	phyWorld->setGravity(Vect(0, 0));
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
	Size edgeSize;
	edgeSize.height = visibleSize.height*1.3;
	edgeSize.width = visibleSize.width*1.2;
	//物理边界
	auto body = PhysicsBody::createEdgeBox(edgeSize
		, PHYSICSBODY_MATERIAL_DEFAULT, 300.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);
	//设置瓦片地图
	_tileMap = TMXTiledMap::create("Map.tmx");
	addChild(_tileMap, 0, 100);

	TMXObjectGroup* group = _tileMap->getObjectGroup("objects");
	ValueMap spawnPoint = group->getObject("ball");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();

	_player = Sprite::create("ball.png");
	//设置物体的物理属性
	auto body2 = PhysicsBody::createCircle(_player->getContentSize().width / 2);
	_player->setPhysicsBody(body2);

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
	//键盘控制
	Start::keyboard();

	return true;
}

void Start::menuCloseCallback(Ref *pSender)
{
	Director::getInstance()->end();
}
bool Start::onTouchBegan(Touch* touch, Event* event)
{
	log("onTouchBegan");
	_player->stopAllActions();
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
	Vec2 playerPos = _player->getPosition();
	Vec2 diff = touchLocation - playerPos;
	Vec2 moveLocation;
	moveLocation.x= diff.x / sqrt(diff.x*diff.x + diff.y*diff.y);
	moveLocation.y= diff.y / sqrt(diff.x*diff.x + diff.y*diff.y);
	float moveSpeed = 500;
	FiniteTimeAction* ac1 = (FiniteTimeAction*)MoveTo::create(2, playerPos+2*moveLocation*moveSpeed);
	ActionInterval *ac = Sequence::create(ac1, NULL);
	_player->runAction(Speed::create(ac,1.0));
}

void Start::keyboard()
{
	Layer::onEnter();
	log("Start keyboard");
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)
	{
		Vec2 playerPos = _player->getPosition();
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: {
			Vec2 moveSpeedUp;
			moveSpeedUp.x = 0;
			moveSpeedUp.y = 500;
			moveUp = MoveTo::create(10, 10 * moveSpeedUp + playerPos);
			_player->runAction(moveUp);
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {
			Vec2 moveSpeedDown;
			moveSpeedDown.x = 0;
			moveSpeedDown.y = -500;
			moveDown = MoveTo::create(10, 10 * moveSpeedDown + playerPos);
			_player->runAction(moveDown);
			break;
		}
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
			Vec2 moveSpeedLeft;
			moveSpeedLeft.x = -500;
			moveSpeedLeft.y = 0;
			moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + playerPos);
			_player->runAction(moveLeft);
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
			Vec2 moveSpeedRight;
			moveSpeedRight.x = 500;
			moveSpeedRight.y = 0;
			moveRight = MoveTo::create(10, 10 * moveSpeedRight + playerPos);
			_player->runAction(moveRight);
			break;
		}


		}
	};

	listener->onKeyReleased = [&](EventKeyboard::KeyCode keycode, Event* event) {
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: {
			_player->stopAction(moveUp);
			break;
			}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {
			_player->stopAction(moveDown);
			break;
		}
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
			_player->stopAction(moveLeft);
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
			_player->stopAction(moveRight);
			break;
		}
		}

	};
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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

