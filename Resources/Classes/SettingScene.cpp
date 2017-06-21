#include "SettingScene.h"

USING_NS_CC;

using namespace CocosDenshion;

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

	//begin
	auto doneMenuItem = MenuItemImage::create(
		"done.png",
		"done2.png",
		CC_CALLBACK_1(Setting::menuStartCallback, this));

	doneMenuItem->setPosition(1250, 202);

	Menu* mn = Menu::create(soundToggleMenuItem, musicToggleMenuItem, doneMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	auto ok = LabelTTF::create("DONE", "Helvetica", 18);
	ok->setPosition(1250, 202);
	this->addChild(ok);
	//back
	auto pbutton2 = MenuItemImage::create(
		"HELP.png",
		"HELP2.png",
		CC_CALLBACK_1(Setting::menuOkCallback, this));
	auto exitmenu = Menu::create(pbutton2, NULL);
	exitmenu->setPosition(Vec2(1250, 142));
	addChild(exitmenu);
	auto back = LabelTTF::create("BACK", "Helvetica", 18);
	back->setPosition(1250, 142);
	this->addChild(back);
	//添加帮助

	UserDefault * defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		musicToggleMenuItem->setSelectedIndex(0);
	}
	else {
		musicToggleMenuItem->setSelectedIndex(1);
	}
	if (defaults->getBoolForKey(SOUND_KEY)) {
		soundToggleMenuItem->setSelectedIndex(0);
	}
	else {
		soundToggleMenuItem->setSelectedIndex(1);
	}
	//音效设置持久化

	//if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
	//	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Bgm.mp3", true);
	//}


}
void Setting::menuStartCallback(Ref *pSender)
{
	auto sc = Start::createScene();
	auto reScene = TransitionSlideInL::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}

void Setting::menuOkCallback(Ref* pSender)
{
	auto hw = HelloWorld::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f,hw));
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}

void Setting::menuSoundToggleCallback(Ref* pSender)
{
	auto soundToggleMenuItem = (MenuItemToggle*)pSender;
	log("soundToggleMenuItem % d", soundToggleMenuItem->getSelectedIndex());

	UserDefault * defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(SOUND_KEY)) {
		defaults->setBoolForKey(SOUND_KEY, false);
	}
	else {
		defaults->setBoolForKey(SOUND_KEY, true);
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}


void Setting::menuMusicToggleCallback(Ref* pSender)
{
	auto musicToggleMenuItem = (MenuItemToggle*)pSender;
	log("musicToggleMenuItem % d", musicToggleMenuItem->getSelectedIndex());

	UserDefault * defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		defaults->setBoolForKey(MUSIC_KEY, false);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
	else {
		defaults->setBoolForKey(MUSIC_KEY, true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Bgm.mp3", true);
	}
}
