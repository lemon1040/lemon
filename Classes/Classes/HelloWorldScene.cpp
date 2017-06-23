#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

using namespace CocosDenshion;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	// ������ǩ
	auto label = LabelTTF::create("EXIT", "Helvetica", 18);
	label->setPosition(1104,100);

	//���뱳��ͼƬ
	auto backpic = Sprite::create("timg.jpg");
	backpic->setPosition(1104, 621);
	addChild(backpic);
	

	//��ʼ��ť����
	auto pbutton = MenuItemImage::create(
		"start.png",
		"start2.png",
		CC_CALLBACK_1(HelloWorld::menuSettingCallback, this));
	auto startmenu = Menu::create(pbutton, NULL);
	//�ر�
	auto pbutton2 = MenuItemImage::create(
		"HELP.png",
		"HELP2.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	auto exitmenu = Menu::create(pbutton2, NULL);
	//�ر�
	

	startmenu->setPosition(1104, 160);
	exitmenu->setPosition(1104, 100);
	addChild(startmenu);
	addChild(exitmenu);
	addChild(label);



	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Bgm.mp3", true);
	}

    return true;
}
void HelloWorld::menuCloseCallback(Ref *pSender)
{
	Director::getInstance()->end();
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}

void HelloWorld::menuSettingCallback(Ref *pSender)
{
	auto sc = Setting::createScene();
	auto reScene = TransitionSlideInL::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}


