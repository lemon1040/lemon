#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("NewCocosProject", Rect(0, 0, 2208, 1242));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(2208, 1242, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);
	//³õÊ¼»¯±³¾°ÒôÀÖ£¨BackgroundMusic)
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/Bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/End.mp3");

	//³õÊ¼»¯ÒôÐ§(Effect)
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Blip.wav");//°´¼üÒôÐ§
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Division.wav");//·ÖÁÑ
	SimpleAudioEngine::getInstance()->preloadEffect("sound/EatingFood.mp3");//³ÔÐ¡Çò
	SimpleAudioEngine::getInstance()->preloadEffect("sound/EatingPlayer.mp3");//³ÔÍæ¼Ò
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Win.wav");//Ê¤Àû
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Wall.wav");//×²Ç½
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
