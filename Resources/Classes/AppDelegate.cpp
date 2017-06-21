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
	//��ʼ���������֣�BackgroundMusic)
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/Bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/End.mp3");

	//��ʼ����Ч(Effect)
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Blip.wav");//������Ч
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Division.wav");//����
	SimpleAudioEngine::getInstance()->preloadEffect("sound/EatingFood.mp3");//��С��
	SimpleAudioEngine::getInstance()->preloadEffect("sound/EatingPlayer.mp3");//�����
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Win.wav");//ʤ��
	SimpleAudioEngine::getInstance()->preloadEffect("sound/Wall.wav");//ײǽ
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
