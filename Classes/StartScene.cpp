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

	// return the scen
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
	//添加玩家
	_player = ball::createball("ball.png",playerTag);
	_player->setTag(playerTag);
	_player->setPosition(Vec2(x, y));
	addChild(_player,2);
	//添加敌人
	_enemy = ball::createball("ball2.png",enemyTag);
	_enemy->setPosition(Vec2(1100,700));
	_enemy->setTag(enemyTag);
	addChild(_enemy);
	//添加食物
	//createfood(10,"food.png");
	//createfood(10, "food2.png");
	//createfood(10, "food3.png");
	createfood(10, "food4.png");
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
void Start::onEnter()
{
	Layer::onEnter();
	log("size:%f", _player->getContentSize().width);
	//schedule(schedule_selector(Start::eat_update), 0.5f);
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();
		log("A = %d  ------------ B = %d", spriteA->getTag(), spriteB->getTag());
		Node* food1 = nullptr;
		Node* player = nullptr;
		Node* player2 = nullptr;
		if (spriteA->getTag() == playerTag && spriteB->getTag() >=3)
		{
			food1 = spriteB;
			player = spriteA;
		}
		if (spriteA->getTag() >=3  && spriteB->getTag() == playerTag)
		{
			food1 = spriteA;
			player = spriteB;
		}
		if (food1 != nullptr) {//发生碰撞
			this->eatfood((thefood*)food1,(ball*)player);
			return true;
		}

		if (spriteA->getTag() == playerTag && spriteB->getTag() == enemyTag) {
			player = (ball*)spriteA;
			player2 = (ball*)spriteB;
		}

		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
}

void Start::onExit()
{
	Layer::onExit();
}

void Start::createfood(int n,const char *_ptr)
{
	srand((int)time(0));
	for (int i = 0; i < n; ++i) {
		auto Food = thefood::create(_ptr,foodTag);
		Food->setTag(foodTag);
		++foodTag;
		Food->setPosition(Vec2(random(2200), random(1220)));
		log("%d,%d", random(2200), random(1220));
		food.push_back(Food);
		addChild(Food);
	}
}
void Start::eatfood(thefood* food1, ball* player)
{
	int num = 0;
	for (auto i = food.begin(); i != food.end(); ++i) {
		auto a = *i;
		if (a->getTag() == food1->getTag()) {
			this->removeChild(*i);
			food.erase(i);
			++num;
			//变大动作
			if (player->getTag() == playerTag) {
				_update(_player);
			}
			if (player->getTag() == enemyTag) {
				_update(_enemy);
			}
			
			break;
		}
	}
	if (num>0)
		createfood(num, "food.png");
}
void Start::_update(ball*player)
{
	ScaleBy * scaleto = ScaleBy::create(1.0f, (1 + player->score / 10) / (0.9 + player->score / 10), (1 + player->score / 10) / (0.9 + player->score / 10));
	player->size = player->size*(1 + player->score / 10) / (0.9 + player->score / 10);
	++player->score;
	player->runAction(scaleto);
	log("size:%f", player->size);
	if (player->score < 45) {
		player->speed = 500 - 10 * player->score;
		log("speed:%f", player->speed);
	}

}
void Start::eat_update(float dt)
{
	int num=0;
	Vec2 playerPos = _player->getPosition();
	for (auto i = food.begin(); i != food.end(); ++i) {
		Sprite*a= *i;
		Vec2 foodPos =a ->getPosition();
		if (Distance(playerPos, foodPos) < _player->size/2 ) {
			++_player->score;
			++num;
			log("food:%f,%f", foodPos.x, foodPos.y);
			log("player:%f,%f",playerPos.x, playerPos.y);
			this->removeChild(*i);
			food.erase(i);
			//变大动作
			ScaleBy * scaleto = ScaleBy::create(1.0f, (1 + _player->score / 10) / (0.9 + _player->score / 10), (1 + _player->score / 10) / (0.9 + _player->score / 10));
			_player->size = _player->size*(1 + _player->score / 10) / (0.9 + _player->score / 10);
			_player->runAction(scaleto);
			log("size:%f", _player->size);
			break;
		}
	}
	if(_player->score<45)
	_player->speed = 500 - 10 * _player->score;
	if(num>0)
	createfood(num, "food.png");
}

double Start::Distance(Vec2 pos1, Vec2 pos2)
{
	return sqrt((pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y));
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
	FiniteTimeAction* ac1 = (FiniteTimeAction*)MoveTo::create(2, playerPos+2*moveLocation*_player->speed);
	ActionInterval *ac = Sequence::create(ac1, NULL);
	_player->runAction(Speed::create(ac,1.0));
}

void Start::keyboard()
{
	log("Start keyboard");
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)
	{
		Vec2 playerPos = _player->getPosition();
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: {
			Vec2 moveSpeedUp;
			moveSpeedUp.x = 0;
			moveSpeedUp.y = _player->speed;
			moveUp = MoveTo::create(10, 10 * moveSpeedUp + playerPos);
			_player->runAction(moveUp);
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {
			Vec2 moveSpeedDown;
			moveSpeedDown.x = 0;
			moveSpeedDown.y = -_player->speed;
			moveDown = MoveTo::create(10, 10 * moveSpeedDown + playerPos);
			_player->runAction(moveDown);
			break;
		}
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
			Vec2 moveSpeedLeft;
			moveSpeedLeft.x = -_player->speed;
			moveSpeedLeft.y = 0;
			moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + playerPos);
			_player->runAction(moveLeft);
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
			Vec2 moveSpeedRight;
			moveSpeedRight.x = _player->speed;
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

