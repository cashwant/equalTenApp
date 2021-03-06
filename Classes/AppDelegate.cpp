#include "AppDelegate.h"
#include "GameScene.h"
#include "NPanel.h"
#include "StartScene.h"
#include "RuleScene.h"
#include "SimpleAudioEngine.h"

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
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	glview->setDesignResolutionSize(320, 480, ResolutionPolicy::SHOW_ALL);

	FileUtils::getInstance()->addSearchPath("images");
	FileUtils::getInstance()->addSearchPath("music");

	// create a scene. it's an autorelease object
	auto scene = StartScene::createScene();

	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	log("tyuudan");
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	auto game = GameScene::create();
	auto node = Director::getInstance()->getRunningScene();
	for (auto scene : node->getChildren()){
		if (typeid(*scene) == typeid(*game)) {
			auto gamescene = dynamic_cast<GameScene*>(scene);
			gamescene->stopGame();
		}
	}
}
