#include "StartScene.h"
USING_NS_CC;
using namespace CocosDenshion;
Scene* Start::createScene()
{
	auto scene = Scene::createWithPhysics();
	//phyWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	PhysicsWorld* phyWorld = scene->getPhysicsWorld();
	phyWorld->setGravity(Vect(0, 0));
	auto layer = Start::create();
	scene->addChild(layer);
	return scene;
}

bool Start::init()
{
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
	/*auto body = PhysicsBody::createEdgeBox(edgeSize
		, PHYSICSBODY_MATERIAL_DEFAULT, 300.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);
	*/
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
	_enemy->Tag = 3000;
	_enemy->setPosition(Vec2(1100,700));
	_enemy->setTag(enemyTag);
	addChild(_enemy);
	//添加生命值
	for (int i = 0; i < 3; ++i) {
		auto a = Sprite::create("ball.png");
		auto b = Sprite::create("ball2.png");
		a->setScale(0.6);
		b->setScale(0.6);
		a->setPosition(50 + 40 * i,1200);
		b->setPosition(50 + 40 * i, 1160);
		P_life.push_back(a);
		E_life.push_back(b);
		addChild(a, 1);
		addChild(b, 1);
	}
	//添加食物
	createfood(10,"food.png");
	createfood(10, "food2.png");
	createfood(10, "food3.png");
	createfood(10, "food4.png");
	_collidable = _tileMap->getLayer("collidable");
	_collidable->setVisible(false);

	//关闭按钮
	auto pbutton3 = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Start::menucloseCallback, this));
	auto closemenu = Menu::create(pbutton3, NULL);
	closemenu->setPosition(2180, 30);
	addChild(closemenu);
	//鼠标控制
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	//键盘控制
	Start::keyboard();
	//背景音乐
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Bgm.mp3", true);
	}
	

	return true;
}
void Start::onEnter()
{
	Layer::onEnter();
	log("size:%f", _player->getContentSize().width);
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();
		if (spriteA == nullptr || spriteB == nullptr)
			return false;
		log("A = %d  ------------ B = %d", spriteA->getTag(), spriteB->getTag());
		Node* food1 = nullptr;
		Node* player = nullptr;
		Node* enemy = nullptr;
		//////////////////////////////////////////食物//////////////////////////////////
		if ((spriteA->getTag() == playerTag|| spriteA->getTag() ==enemyTag) && (spriteB->getTag() >=3&& spriteB->getTag() <1000) )
		{
			food1 = spriteB;
			player = spriteA;
		}
		if ((spriteA->getTag() >= 3 && spriteA->getTag() <1000) && (spriteB->getTag() == playerTag || spriteB->getTag() == enemyTag))
		{
			food1 = spriteA;
			player = spriteB;
		}
		if (food1 != nullptr) {//发生碰撞
			this->eatfood((thefood*)food1,(ball*)player);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/EatingFood.mp3");
			}
			return true;
		}
		//////////////////////////////////////////////////////////


		/////////////////分裂物进食////////////////////
		if (spriteA->getTag() >= 1000 && (spriteB->getTag() >= 3 && spriteB->getTag() < 1000)) {
			food1 = spriteB;
			player = spriteA;
		}
		if ((spriteA->getTag() >= 3 && spriteA->getTag() < 1000) && spriteB->getTag() >= 1000) {
			food1 = spriteA;
			player = spriteB;
		}
		if (food1 != nullptr) {
			this->eatfood((thefood*)food1, (ball*)player);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/EatingFood.mp3");
			}
			return true;
		}
		///////////////////////////////////////////////////////


		/////////////////////////相杀////////////////////////////////////////////
		if( (spriteA->getTag() == playerTag && spriteB->getTag() == enemyTag)|| (spriteA->getTag() == enemyTag && spriteB->getTag() == playerTag) ){
			player = spriteA;
		}
		if (player != nullptr) {
			schedule(schedule_selector(Start::kill), 0.1f);
			
			return true;
		}
		/////////////////////吃分裂球/////////////////////////////
		if((spriteA->getTag() == playerTag|| spriteA->getTag() == enemyTag)&&spriteB->getTag() >= 1000)
		{
			onekill((ball*)spriteA, (ball*)spriteB);
			return true;
		}
		if ((spriteB->getTag() == playerTag || spriteB->getTag() == enemyTag) && spriteA->getTag() >= 1000)
		{
			onekill((ball*)spriteB, (ball*)spriteA);
			return true;
		}
		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
	schedule(schedule_selector(Start::stopaction), 0.01f);
	schedule(schedule_selector(Start::feed),1.0f);
}

void Start::onExit()
{
	Layer::onExit();
	unschedule(schedule_selector(Start::stopaction));
	unschedule(schedule_selector(Start::kill));
	unschedule(schedule_selector(Start::feed));

	//注销事件监听器.
	Director::getInstance()->getEventDispatcher()->removeEventListener(contactListener);
	removeAllChildren();
	food.clear();
	P_group.clear();
	E_group.clear();
	P_life.clear();
	E_life.clear();
}

void Start::createfood(int n,const char *_ptr)
{
	srand((int)time(0) + foodTag*100);
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
			break;
		}
	}
	//变大动作
	if (player->getTag() == playerTag) {
		_update(_player, foodScore);
	}
	if (player->getTag() == enemyTag) {
		_update(_enemy, foodScore);
	}
	if (player->getTag() >= 1000 && player->getTag() < 3000) {
		for (auto i = P_group.begin(); i != P_group.end(); ++i) {
			auto &a = *i;
			if (a->getTag() == player->getTag()) {
				_update(a, foodScore);
				break;
			}
		}
	}
	if (player->getTag() >= 3000) {
		for (auto i = E_group.begin(); i != E_group.end(); ++i) {
			auto &a = *i;
			if (a->getTag() == player->getTag()) {
				_update(a, foodScore);
				break;
			}
		}
	}
	//重生食物
	//if (num>0)
		//createfood(num, "food.png");
}

void Start::feed(float dt) {
	if (food.size() < 20) {
		createfood(10, "food2.png");
		createfood(10, "food3.png");
	}
}

void Start::_update(ball*player,double score)
{
	player->pastscore = player->score;
	player->score = player->score + score;
	ScaleBy * scaleBy = ScaleBy::create(0.5f, (1 + player->score / 10) / (1 + player->pastscore / 10), (1 + player->score / 10) / (1+ player->pastscore / 10));
	player->runAction(scaleBy);
	player->size = player->size*(1 + player->score / 10) / (1 + player->pastscore / 10);
	log("pastscore:%f----------------score:%f-------------%f", player->pastscore, player->score, (1 + player->score / 10) / (1 + player->pastscore / 10));
	log("size:%f", player->size);
	if (player->score < 46||score<0) {
		player->speed = 500 - 10 * player->score;
		log("speed:%f", player->speed);
	}
}
void Start::kill(float dt)
{
	Vec2 pos1 = _player->getPosition();
	Vec2 pos2 = _enemy->getPosition();
	double dis = Distance(pos1, pos2);
	if (_player->size / 2 - _enemy->size / 2>dis) {
		_update(_player, _enemy->score);
		if (enemylife > 1) {
			if (enemylife == 2) {
				if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
					SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/End.mp3", true);
				}
			}
			removeChild(_enemy);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/EatingPlayer.wav");
			}
		}
		for (auto i = E_group.begin(); i != E_group.end(); ++i) {
			auto&a = *i;
			a->setVisible(false);
		}
		resetenemy();
	}
	if (_enemy->size / 2 - _player->size / 2 > dis) {
		_update(_enemy, _player->score);
		if (playerlife > 1) {
			if (playerlife == 2) {
				if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
					SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/End.mp3", true);
				}
			}
			removeChild(_player);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/EatingPlayer.wav");
			}
		}
		for (auto i = P_group.begin(); i != P_group.end(); ++i) {
			auto&a = *i;
			a->setVisible(false);
		}
		resetplayer();
	}
}
void Start::onekill(ball*player, ball*group)
{
	log("player:%d", player->getTag());
	if (group->getTag() < 3000) {
		for (auto i = P_group.begin(); i != P_group.end(); ++i) {
			auto&a = *i;
			if (a->getTag() == group->getTag()) {
				if (player->getTag() == playerTag)
					_update(_player, a->score);
				else
					_update(_enemy, a->score);
				removeChild(a);
				if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
					SimpleAudioEngine::getInstance()->playEffect("sound/EatingPlayer.wav");
				}
				P_group.erase(i);
				break;
			}
		}
	}
	else {
		for (auto i = E_group.begin(); i != E_group.end(); ++i) {
			auto&a = *i;
			if (a->getTag() == group->getTag()) {
				if (player->getTag() == playerTag)
					_update(_player, a->score);
				else
					_update(_enemy, a->score);

				removeChild(a);
				if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
					SimpleAudioEngine::getInstance()->playEffect("sound/EatingPlayer.wav");
				}
				E_group.erase(i);
				break;
			}
		}
	}

}

double Start::Distance(Vec2 pos1, Vec2 pos2)
{
	return sqrt((pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y));
}
void Start::resetenemy()
{
	--enemylife;
	if (enemylife > 0) {
		_enemy = ball::createball("ball2.png", enemyTag);
		_enemy->setPosition(Vec2(600, 700));
		_enemy->Tag = 3000;
		_enemy->setTag(enemyTag);
		addChild(_enemy,2);
	}
	else {
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
			SimpleAudioEngine::getInstance()->playEffect("sound/Win.wav");
		}
		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto end = Sprite::create("bluewin.png");
		end->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		addChild(end);

		auto pbutton3 = MenuItemImage::create(
			"done.png",
			"done2.png",
			CC_CALLBACK_1(Start::menucloseCallback, this));
		auto closemenu = Menu::create(pbutton3, NULL);
		closemenu->setPosition(1080, 550);
		addChild(closemenu,0);
		auto back = LabelTTF::create("Back to menu", "Helvetica", 18);
		back->setPosition(1080, 550);
		this->addChild(back,0);

		auto pbutton2 = MenuItemImage::create(
			"HELP.png",
			"HELP2.png",
			CC_CALLBACK_1(Start::menuresetCallback, this));
		auto close = Menu::create(pbutton2, NULL);
		close->setPosition(1080, 490);
		addChild(close, 0);
		auto back1 = LabelTTF::create("Reset", "Helvetica", 18);
		back1->setPosition(1080, 490);
		this->addChild(back1, 0);
		_enemy->setPosition(Vec2(20, 20));
		_player->setPosition(Vec2(2150, 1160));

	}
	if (!E_life.empty()) {
		auto i = E_life.end() - 1;
		removeChild(*i);
		E_life.erase(i);
	}
	
}
void Start::resetplayer()
{
	--playerlife;
	if (playerlife > 0) {
		_player = ball::createball("ball.png", playerTag);
		_player->setTag(playerTag);
		_player->setPosition(Vec2(900, 600));
		addChild(_player, 2);
	}
	else {
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
			SimpleAudioEngine::getInstance()->playEffect("sound/Win.wav");
		}
		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto end = Sprite::create("yellowwin.png");
		end->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		addChild(end);

		auto pbutton3 = MenuItemImage::create(
			"done.png",
			"done2.png",
			CC_CALLBACK_1(Start::menucloseCallback, this));
		auto closemenu = Menu::create(pbutton3, NULL);
		closemenu->setPosition(1080, 550);
		addChild(closemenu, 0);
		auto back = LabelTTF::create("Back to menu", "Helvetica", 18);
		back->setPosition(1080, 550);
		this->addChild(back, 0);

		auto pbutton2 = MenuItemImage::create(
			"HELP.png",
			"HELP2.png",
			CC_CALLBACK_1(Start::menuresetCallback, this));
		auto close = Menu::create(pbutton2, NULL);
		close->setPosition(1080, 490);
		addChild(close, 0);
		auto back1 = LabelTTF::create("Reset", "Helvetica", 18);
		back1->setPosition(1080, 490);
		this->addChild(back1, 0);
		_enemy->setPosition(Vec2(20, 20));
		_player->setPosition(Vec2(2150, 1160));
	}
	if (!P_life.empty()) {
		auto i = P_life.end() - 1;
		removeChild(*i);
		P_life.erase(i);
	}
}

void Start::division(ball* player, vector<ball*>&P_group) {
	if (player->score == 0)
		return ;
		++player->Tag;
		_update(player, -player->score / 2);
		if (!P_group.empty()) {
			for (auto i = P_group.begin(); i != P_group.end(); ++i) {
				auto a = *i;
				_update(*i, -a->score / 2);
			}
			int n = P_group.size();
			for (int i = 0; i < n; ++i) {
				Vec2 pos = P_group[i]->getPosition();
				ball*p;
				if (player->getTag() == playerTag)
					p = ball::createball("ball.png", playerTag + player->Tag + i);
				else
					p = ball::createball("ball2.png", playerTag + player->Tag + i);

				_update(p, P_group[i]->score / 2);
				p->setTag(playerTag+player->Tag+i);
				p->setPosition(Vec2(pos.x, pos.y-P_group[i]->size-20));
				P_group.push_back(p);
				addChild(p);
			}
			ball*p;
			if (player->getTag() == playerTag)
				p = ball::createball("ball.png", playerTag + player->Tag -1);
			else
				p = ball::createball("ball2.png", playerTag + player->Tag -1);
			Vec2 pos = player->getPosition();
			_update(p,player->score / 2);
			p->setTag(playerTag + player->Tag - 1);
			p->setPosition(Vec2(pos.x , pos.y - player->size-20));
			P_group.push_back(p);
			addChild(p);
		}
		else {
			Vec2 pos = player->getPosition();
			ball*p;
			if (player->getTag() == playerTag)
				p = ball::createball("ball.png", playerTag + player->Tag -1);
			else
				p = ball::createball("ball2.png", playerTag + player->Tag -1);
			_update(p, player->score / 2);
			p->setTag(playerTag + player->Tag -1);
			p->setPosition(Vec2(pos.x - player->size-20 , pos.y));
			P_group.push_back(p);
			addChild(p);
		}
		player->speed = player->speed + 100;
		if(player->getTag()==playerTag)
		scheduleOnce(schedule_selector(Start::slowplayer), 3.0f);
		else
		scheduleOnce(schedule_selector(Start::slowenemy), 3.0f);

}
void Start::slowplayer(float dt) {
	_player->speed = _player->speed - 100;
}
void Start::slowenemy(float dt) {
	_enemy->speed = _enemy->speed - 100;
}
void Start::stopaction(float dt)
{
	Vec2 playerPos = _player->getPosition();
	if (playerPos.x < 40) {
		_player->stopAction(_player->moveLeft);
	}
	else if (playerPos.x > 2170) {
		_player->stopAction(_player->moveRight);
	}
	if (playerPos.y < 30) {
		_player->stopAction(_player->moveDown);
	}
	if (playerPos.y > 1150) {
		_player->stopAction(_player->moveUp);
	}

	Vec2 playerPos1 = _enemy->getPosition();
	if (playerPos1.x < 40) {
		_enemy->stopAction(_enemy->moveLeft);
	}
	else if (playerPos1.x > 2170) {
		_enemy->stopAction(_enemy->moveRight);
	}
	if (playerPos1.y < 30) {
		_enemy->stopAction(_enemy->moveDown);
	}
	if (playerPos1.y > 1150) {
		_enemy->stopAction(_enemy->moveUp);
	}

	for (auto i = P_group.begin(); i != P_group.end(); ++i) {
		ball*&a = *i;
		Vec2 playerPos = a->getPosition();
		if (playerPos.x < 40) {
			a->stopAction(a->moveLeft);
		}
		else if (playerPos.x > 2170) {
			a->stopAction(a->moveRight);
		}
		if (playerPos.y < 30) {
			a->stopAction(a->moveDown);
		}
		if (playerPos.y > 1150) {
			a->stopAction(a->moveUp);
		}
	}
	for (auto i = E_group.begin(); i != E_group.end(); ++i) {
		ball*&a = *i;
		Vec2 playerPos = a->getPosition();
		if (playerPos.x < 40) {
			a->stopAction(a->moveLeft);
		}
		else if (playerPos.x > 2170) {
			a->stopAction(a->moveRight);
		}
		if (playerPos.y < 30) {
			a->stopAction(a->moveDown);
		}
		if (playerPos.y > 1150) {
			a->stopAction(a->moveUp);
		}
	}
}

void Start::menucloseCallback(Ref *pSender)
{

	auto hw = HelloWorld::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, hw));
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}
void Start::menuresetCallback(Ref *pSender)
{
	auto hw = Start::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, hw));
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
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
		Vec2 enemyPos = _enemy->getPosition();
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: {
			Vec2 moveSpeedUp;
			moveSpeedUp.x = 0;
			moveSpeedUp.y = _player->speed;
			_player->moveUp = MoveTo::create(10, 10 * moveSpeedUp + playerPos);
			_player->runAction(_player->moveUp);
			if (!P_group.empty()) {
				for (auto i = P_group.begin(); i != P_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedUp.x = 0;
					moveSpeedUp.y = a->speed;
					a->moveUp = MoveTo::create(10, 10 * moveSpeedUp + playerPos);
					a->runAction(a->moveUp);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {
			Vec2 moveSpeedDown;
			moveSpeedDown.x = 0;
			moveSpeedDown.y = -_player->speed;
			_player->moveDown = MoveTo::create(10, 10 * moveSpeedDown + playerPos);
			_player->runAction(_player->moveDown);
			if (!P_group.empty()) {
				for (auto i = P_group.begin(); i != P_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedDown.x = 0;
					moveSpeedDown.y = -a->speed;
					a->moveDown = MoveTo::create(10, 10 * moveSpeedDown + playerPos);
					a->runAction(a->moveDown);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
			Vec2 moveSpeedLeft;
			moveSpeedLeft.x = -_player->speed;
			moveSpeedLeft.y = 0;
			_player->moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + playerPos);
			_player->runAction(_player->moveLeft);
			if (!P_group.empty()) {
				for (auto i = P_group.begin(); i != P_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedLeft.x = -a->speed;
					moveSpeedLeft.y = 0;
					a->moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + playerPos);
					a->runAction(a->moveLeft);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
			Vec2 moveSpeedRight;
			moveSpeedRight.x = _player->speed;
			moveSpeedRight.y = 0;
			_player->moveRight = MoveTo::create(10, 10 * moveSpeedRight + playerPos);
			_player->runAction(_player->moveRight);
			if (!P_group.empty()) {
				for (auto i = P_group.begin(); i != P_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedRight.x = a->speed;
					moveSpeedRight.y = 0;
					a->moveRight = MoveTo::create(10, 10 * moveSpeedRight + playerPos);
					a->runAction(a->moveRight);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_ENTER: {
			division(_player,P_group);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/Division.wav");
			}
			break;
		}

		case EventKeyboard::KeyCode::KEY_W:{
			Vec2 moveSpeedUp;
			moveSpeedUp.x = 0;
			moveSpeedUp.y = _enemy->speed;
			_enemy->moveUp = MoveTo::create(10, 10 * moveSpeedUp + enemyPos);
			_enemy->runAction(_enemy->moveUp);
			if (!E_group.empty()) {
				for (auto i = E_group.begin(); i != E_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedUp.x = 0;
					moveSpeedUp.y = a->speed;
					a->moveUp = MoveTo::create(10, 10 * moveSpeedUp + enemyPos);
					a->runAction(a->moveUp);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_S:{
			Vec2 moveSpeedDown;
			moveSpeedDown.x = 0;
			moveSpeedDown.y = -_enemy->speed;
			_enemy->moveDown = MoveTo::create(10, 10 * moveSpeedDown + enemyPos);
			_enemy->runAction(_enemy->moveDown);
			if (!E_group.empty()) {
				for (auto i = E_group.begin(); i != E_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedDown.x = 0;
					moveSpeedDown.y = -a->speed;
					a->moveDown = MoveTo::create(10, 10 * moveSpeedDown + enemyPos);
					a->runAction(a->moveDown);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_A: {
			Vec2 moveSpeedLeft;
			moveSpeedLeft.x = -_enemy->speed;
			moveSpeedLeft.y = 0;
			_enemy->moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + enemyPos);
			_enemy->runAction(_enemy->moveLeft);
			if (!E_group.empty()) {
				for (auto i = E_group.begin(); i != E_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedLeft.x = -a->speed;
					moveSpeedLeft.y = 0;
					a->moveLeft = MoveTo::create(10, 10 * moveSpeedLeft + enemyPos);
					a->runAction(a->moveLeft);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_D: {
			Vec2 moveSpeedRight;
			moveSpeedRight.x = _enemy->speed;
			moveSpeedRight.y = 0;
			_enemy->moveRight = MoveTo::create(10, 10 * moveSpeedRight + enemyPos);
			_enemy->runAction(_enemy->moveRight);
			if (!E_group.empty()) {
				for (auto i = E_group.begin(); i != E_group.end(); ++i) {
					ball*&a = *i;
					moveSpeedRight.x = a->speed;
					moveSpeedRight.y = 0;
					a->moveRight = MoveTo::create(10, 10 * moveSpeedRight + enemyPos);
					a->runAction(a->moveRight);
				}
			}
			break;
		}
		}
	};

	listener->onKeyReleased = [&](EventKeyboard::KeyCode keycode, Event* event) {
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: {
			_player->stopAction(_player->moveUp);
			for (auto i = P_group.begin(); i != P_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveUp);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {
			_player->stopAction(_player->moveDown);
			for (auto i = P_group.begin(); i != P_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveDown);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
			_player->stopAction(_player->moveLeft);
			for (auto i = P_group.begin(); i != P_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveLeft);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
			_player->stopAction(_player->moveRight);
			for (auto i = P_group.begin(); i != P_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveRight);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_W: {
			_enemy->stopAction(_enemy->moveUp);
			for (auto i = E_group.begin(); i != E_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveUp);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_S: {
			_enemy->stopAction(_enemy->moveDown);
			for (auto i = E_group.begin(); i != E_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveDown);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_A: {
			_enemy->stopAction(_enemy->moveLeft);
			for (auto i = E_group.begin(); i !=E_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveLeft);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_D: {
			_enemy->stopAction(_enemy->moveRight);
			for (auto i = E_group.begin(); i != E_group.end(); ++i) {
				ball*&a = *i;
				a->stopAction(a->moveRight);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {
			division(_enemy, E_group);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
				SimpleAudioEngine::getInstance()->playEffect("sound/Division.wav");
			}
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

