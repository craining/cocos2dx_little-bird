#include "AppDelegate.h"
#include "StartPageScene.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

bool AppDelegate::audioOn = false;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
}
bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	pDirector->setOpenGLView(pEGLView);

	// turn on display FPS
	pDirector->setDisplayStats(false);
//    pEGLView->setDesignResolutionSize(pDirector->getVisibleSize().width, pDirector->getVisibleSize().height, kResolutionExactFit);
	pEGLView->setDesignResolutionSize(480, 800, kResolutionExactFit);
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	audioOn = CCUserDefault::sharedUserDefault()->getBoolForKey("audioOn", true);

	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("bg_robbot.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("flappy.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("over.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("bone.mp3");

	ControlBackgroundSound(audioOn);

	//加载资源
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("littlebird_res.plist", "littlebird_res.png");

// create a scene. it's an autorelease object
	CCScene *pScene = StartPage::scene();
	// run
	pDirector->runWithScene(pScene);
	return true;
}

void AppDelegate::ControlBackgroundSound(bool on) {
	if (on && !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
		//	 SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("bg_mic_littlebird.mp3") );

		//SimpleAudioEngine::sharedEngine()->preloadEffect( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(EFFECT_FILE) );
		//MUSIC_FILE就是音乐文件相对Resources文件夹的相对路径下的文件名，然后再转换为绝对路径，第二个参数就是是否循环播放
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bg_robbot.mp3", true);
		//	         SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("bg_mic_littlebird.mp3")).c_str(), true);

	} else {
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			SimpleAudioEngine::sharedEngine()->end();
		}

	}

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	CCDirector::sharedDirector()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	CCDirector::sharedDirector()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
