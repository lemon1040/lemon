#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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
	// 创建标签
	auto label = LabelTTF::create("help", "Helvetica", 18);
	label->setPosition(1104,100);

	//加入背景图片
	auto backpic = Sprite::create("timg.jpg");
	backpic->setPosition(1104, 621);
	addChild(backpic);
	

	//开始按钮精灵
	auto pbutton = MenuItemImage::create(
		"start.png",
		"start2.png",
		CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
	auto startmenu = Menu::create(pbutton, NULL);
	//帮助与设置
	auto pbutton2 = MenuItemImage::create(
		"HELP.png",
		"HELP2.png",
		CC_CALLBACK_1(HelloWorld::menuSettingCallback, this));
	auto helpmenu = Menu::create(pbutton2, NULL);
	//关闭
	auto pbutton3 = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	auto closemenu = Menu::create(pbutton3, NULL);

	startmenu->setPosition(1104, 160);
	helpmenu->setPosition(1104, 100);
	closemenu->setPosition(2180, 30);
	addChild(startmenu);
	addChild(helpmenu);
	addChild(closemenu);
	addChild(label);



    return true;
}
void HelloWorld::menuCloseCallback(Ref *pSender)
{
	Director::getInstance()->end();
}

void HelloWorld::menuSettingCallback(Ref *pSender)
{
	auto sc = Setting::createScene();
	auto reScene = TransitionSlideInL::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}
void HelloWorld::menuStartCallback(Ref *pSender)
{
	auto sc = Start::createScene();
	auto reScene = TransitionSlideInL::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}
