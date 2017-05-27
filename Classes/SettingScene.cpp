#include "SettingScene.h"

USING_NS_CC;

Scene* Setting::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Setting::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Setting::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("timgsetting.png");

	// position the label on the center of the screen
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);
	log("Hello");

	//音效
	auto soundOnMenuItem = MenuItemImage::create(
		"on.png",
		"on.png");
	auto soundOffMenuItem = MenuItemImage::create(
		"off.png",
		"off.png");

	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuSoundToggleCallback, this),
		soundOnMenuItem,
		soundOffMenuItem,
		NULL);
	soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(2000, 200)));
	auto sound = LabelTTF::create("Sound", "Helvetica", 25);
	sound->setPosition(1850, 1042);
	this->addChild(sound);
	//音乐
	auto musicOnMenuItem = MenuItemImage::create(
		"on.png",
		"on.png");
	auto musicOffMenuItem = MenuItemImage::create(
		"off.png",
		"off.png");
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuMusicToggleCallback, this),
		musicOnMenuItem,
		musicOffMenuItem,
		NULL);
	musicToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(2000,300)));
	auto music = LabelTTF::create("Music", "Helvetica", 25);
	music->setPosition(1850, 942);
	this->addChild(music);
	//back
	auto okMenuItem = MenuItemImage::create(
		"done.png",
		"done2.png",
		CC_CALLBACK_1(Setting::menuOkCallback, this));

	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(1250, 1140)));

	Menu* mn = Menu::create(soundToggleMenuItem, musicToggleMenuItem, okMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	auto ok = LabelTTF::create("DONE", "Helvetica", 18);
	ok->setPosition(1250, 102);
	this->addChild(ok);
	return true;
}

void Setting::menuOkCallback(Ref* pSender)
{
	auto hw = HelloWorld::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f,hw));
}

void Setting::menuSoundToggleCallback(Ref* pSender)
{

}


void Setting::menuMusicToggleCallback(Ref* pSender)
{

}