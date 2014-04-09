/*
 * StartPage.cpp
 *
 *  Created on: 2014-2-24
 *      Author: zhuanggy
 */

#include "StartPageScene.h"
#include "HelloWorldScene.h"
#include "AlertDlgLayer.h"
#include "AppDelegate.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#include <android/log.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif

#endif

CCScene* StartPage::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	StartPage *layer = StartPage::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartPage::init() {
	CCLog("init  StartPage");
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}
	addViews();
	return true;
}

void StartPage::addViews() {

	//添加背景图片
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

//	CCSprite* background = CCSprite::createWithSpriteFrameName("bg_main_two.png");
	CCSprite* background = CCSprite::create("bg_main_start.png");
	background->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);
	//添加鸟儿

	CCSprite* bird = CCSprite::create("bird_fly_down_yellow.png");
//	CCSprite* bird = CCSprite::createWithSpriteFrameName("bird_fly_down_yellow.png");
	bird->setPosition(ccp(95, visibleSize.height-80));
	addChild(bird);
	CCAnimation *animation = CCAnimation::create();
	//从本地文件系统中加载图片文件到CCSpriteFrame中区，然后添加到CCAnimation中
	animation->addSpriteFrameWithFileName("bird_fly_down_yellow.png");
	animation->addSpriteFrameWithFileName("bird_fly_up_yellow.png");
//	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName("bird_fly_down_yellow.png")->displayFrame());
//	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName("bird_fly_up_yellow.png")->displayFrame());
	animation->setDelayPerUnit(0.2f / 2.0f); // 这个动画包含2帧，将会持续0.2秒.
	animation->setLoops(-1);
	CCAnimate *fly = CCAnimate::create(animation);
	CCActionInterval *moveTo = CCMoveTo::create(0.5, ccp(140, visibleSize.height-30));
	CCEaseIn *moveTo_ = CCEaseIn::create(moveTo, 2.0f);
	CCActionInterval *moveBack = CCMoveTo::create(0.5, ccp(95, visibleSize.height-80));
	CCEaseInOut *moveBack_ = CCEaseInOut::create(moveBack, 2.0f);
	CCFiniteTimeAction *jumpjump = CCSequence::create(moveTo_, moveBack_, NULL);
	CCActionInterval * jumpjumpforever = CCRepeatForever::create((CCActionInterval*) jumpjump);
	bird->runAction(jumpjumpforever);  // 运行精灵对象
	bird->runAction(fly);

	//获得得分
	scoreRecord = CCUserDefault::sharedUserDefault()->getIntegerForKey("maxscore");
	labelScore = CCLabelTTF::create(CCString::createWithFormat("最高得分: %d ", scoreRecord)->getCString(), "", 30);
	labelScore->setColor(ccc3(205, 55, 0));

	//添加菜单
	CCMenuItemImage *pStartItem = CCMenuItemImage::create("btn_start.png", "btn_start_p.png", this, menu_selector(StartPage::onStartGame));
//	CCMenuItemImage *pStartItem = CCMenuItemImage::create(NULL, NULL, this, menu_selector(StartPage::onStartGame));
//	pStartItem->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("btn_start.png")->displayFrame());
//	pStartItem->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("btn_start_p.png")->displayFrame());
	int itemHeight = pStartItem->getContentSize().height;
	pStartItem->setPosition(ccp((visibleSize.width/2 ), (visibleSize.height/2 - pStartItem->getContentSize().height*2) ));

//	CCMenuItemImage *pSetItem = CCMenuItemImage::create("btn_set.png", "btn_set_p.png", this, menu_selector(StartPage::onSet));
//	pSetItem->setPosition(ccp((visibleSize.width/2) , pStartItem->getPositionY()-itemHeight*2));

	pAboutItem = CCMenuItemImage::create("btn_about.png", "btn_about_p.png", this, menu_selector(StartPage::onAbout));
//	CCMenuItemImage *pAboutItem = CCMenuItemImage::create(NULL, NULL, this, menu_selector(StartPage::onAbout));
//	pAboutItem->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("btn_about.png")->displayFrame());
//	pAboutItem->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("btn_about_p.png")->displayFrame());
	pAboutItem->setPosition(ccp((visibleSize.width/2) , pStartItem->getPositionY()-itemHeight*2));

	pExitItem = CCMenuItemImage::create("btn_exit.png", "btn_exit_p.png", this, menu_selector(StartPage::onExitOpera));
//	pExitItem = CCMenuItemImage::create(NULL, NULL, this, menu_selector(StartPage::onExitOpera));
//	pExitItem->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("btn_exit.png")->displayFrame());
//	pExitItem->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("btn_exit_p.png")->displayFrame());
	pExitItem->setPosition(ccp((visibleSize.width/2) , (pAboutItem->getPositionY()-itemHeight*2)));

	pAudioItem = CCMenuItemImage::create("bubble_yellow_btn_audio_n.png", "bubble_yellow_btn_audio_p.png", this, menu_selector(StartPage::onAudioSwitch));
//	pAudioItem = CCMenuItemImage::create(NULL, NULL, this, menu_selector(StartPage::onAudioSwitch));
//	pAudioItem->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("bubble_yellow_btn_audio_n.png")->displayFrame());
//	pAudioItem->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("bubble_yellow_btn_audio_p.png")->displayFrame());
	pAudioItem->setPosition(ccp(visibleSize.width-pAudioItem->getContentSize().width, visibleSize.height-pAudioItem->getContentSize().height));

	pScoreItem = CCMenuItemImage::create("bubble_blue_btn_score_n.png", "bubble_blue_btn_score_p.png", this, menu_selector(StartPage::onShowSortOfWorld));
//	pScoreItem = CCMenuItemImage::create(NULL, NULL, this, menu_selector(StartPage::onShowSortOfWorld));
//	pScoreItem->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("bubble_blue_btn_score_n.png")->displayFrame());
//	pScoreItem->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("bubble_blue_btn_score_p.png")->displayFrame());
	pScoreItem->setPosition(ccp(visibleSize.width/2 , bird->getPositionY()-labelScore->getContentSize().height*4));

	pFlowerRed = CCMenuItemImage::create("flower_red.png", "flower_red.png", this, menu_selector(StartPage::onFlowerRedClicked));
	pFlowerRed->setPosition(
			ccp(pAboutItem->getPositionX() - pAboutItem->getContentSize().width / 2 - pFlowerRed->getContentSize().width * 2, pAboutItem->getPositionY() - pAboutItem->getContentSize().height));
	pFlowerWhite = CCMenuItemImage::create("flower_white.png", "flower_white.png", this, menu_selector(StartPage::onFlowerWhiteClicked));
	pFlowerWhite->setPosition(ccp(visibleSize.width - pAboutItem->getPositionX() + pAboutItem->getContentSize().width / 2 + pFlowerRed->getContentSize().width * 2,
			pAboutItem->getPositionY() + pAboutItem->getContentSize().height * 2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pAudioItem, pScoreItem, pStartItem, pAboutItem, pExitItem, pFlowerWhite, pFlowerRed, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);
	labelScore->setPosition(pScoreItem->getPosition());
	addChild(labelScore);
	CCLabelTTF* startLabel = CCLabelTTF::create("开始", "", 25);
	startLabel->setColor(ccc3(255, 255, 255));
	startLabel->setPosition(pStartItem->getPosition());
	this->addChild(startLabel);
//	CCLabelTTF* setLabel = CCLabelTTF::create("设置", "", 25);
//	setLabel->setColor(ccc3(0, 0, 0));
//	setLabel->setPosition(pSetItem->getPosition());
//	this->addChild(setLabel);
	labelAbout = CCLabelTTF::create("关于", "", 25);
	labelAbout->setColor(ccc3(255, 255, 255));
	labelAbout->setPosition(pAboutItem->getPosition());
	this->addChild(labelAbout);
	labelExit = CCLabelTTF::create("退出", "", 25);
	labelExit->setColor(ccc3(255, 255, 255));
	labelExit->setPosition(pExitItem->getPosition());
	this->addChild(labelExit);

	//添加最高分展示
	if (scoreRecord > 0) {
		labelScore->setVisible(true);
	} else {
		labelScore->setVisible(false);
		pScoreItem->setVisible(false);
	}


	if (AppDelegate::audioOn) {
		labelAudioSwitch = CCLabelTTF::create("关闭声音", "", 22);
	} else {
		labelAudioSwitch = CCLabelTTF::create("开启声音", "", 22);
	}

	labelAudioSwitch->setColor(ccc3(34, 139, 34));
	labelAudioSwitch->setPosition(pAudioItem->getPosition());
	this->addChild(labelAudioSwitch);
}

void StartPage::onStartGame(CCObject* pSender) {
	CCTransitionScene * reScene = CCTransitionFadeTR::create(0.5, HelloWorld::scene());
	CCDirector::sharedDirector()->replaceScene(reScene);
}

void StartPage::onFlowerRedClicked(CCObject* pSender) {

	pFlowerRed->stopAllActions();
	//CCTintTo
	//    作用：创建一个色彩变化的消失动作
	//    参数1：色彩变化的动作
	//    参数2 ：红色分量
	//    参数3：蓝色分量
//	CCActionInterval * tinto = CCTintTo::create(3, 255, 255, 0);
//	    sp->runAction(tinto);

	//    CCFadeIn
	//    作用：创建一个渐变出现的动作
	//    参数是时间
//	CCActionInterval * fadein = CCFadeIn::create(2);
	//    sp->runAction(fadein);

	//    CCFadeOut
	//    作用：创建一个渐变消失的动作
	//    参数是时间
//	CCActionInterval * fadeout = CCFadeOut::create(2);
	//    sp->runAction(fadeout);

	////    CCScaleTo   作用：创建一个缩放的动作
	////    参数1：达到缩放大小所需的时间
	////    参数2 ：缩放的比例
//	CCActionInterval * scaleto = CCScaleTo::create(2, 2);
	//    sp->runAction(scaleto);
	//

	CCActionInterval * tintby = CCTintBy::create(1, 0, 0, 150);
	CCActionInterval * tintby1 = tintby->reverse();

	////    CCScaleBy  作用:创建一个缩放的动作
	////    参数1：达到缩放大小的所需时间  参数2：缩放比例
	CCActionInterval * scaleby = CCScaleBy::create(1, 1.5f);
	CCActionInterval * actionbyback = scaleby->reverse();
	//    sp->runAction(actionbyback);
	//
	////    CCRotateTo
	////    作用创建一个旋转的动作
	////    参数1：旋转的时间  参数2：旋转饿角度  0 - 360
	CCActionInterval * rotateto = CCRotateTo::create(1, 180);
	//    sp->runAction(rotateto);

	CCSequence * sequence = CCSequence::create(scaleby, tintby, actionbyback, rotateto, tintby1, NULL);
	pFlowerRed->runAction(sequence);
}

void StartPage::onFlowerWhiteClicked(CCObject* pSender) {
	pFlowerWhite->stopAllActions();
	////    CCScaleBy  作用:创建一个缩放的动作
	////    参数1：达到缩放大小的所需时间  参数2：缩放比例
	CCActionInterval * scaleby = CCScaleBy::create(1, 1.5f);
	CCActionInterval * actionbyback = scaleby->reverse();
	//    sp->runAction(actionbyback);
	//
	////    CCRotateTo
	////    作用创建一个旋转的动作
	////    参数1：旋转的时间  参数2：旋转饿角度  0 - 360
	CCActionInterval * rotateto = CCRotateTo::create(1, 180);
	//    sp->runAction(rotateto);

	CCActionInterval * tintby = CCTintBy::create(1, 150, 0, 0);
	CCActionInterval * tintby1 = tintby->reverse();

	CCSequence * sequence = CCSequence::create(scaleby, tintby, rotateto, actionbyback, tintby1, NULL);
	pFlowerWhite->runAction(sequence);

}

void StartPage::onSet(CCObject* pSender) {
//	CCTransitionScene *scene = CCTransitionFadeTR::create(0.5, SettingPage::scene());
//	CCDirector::sharedDirector()->replaceScene(scene);
}

void StartPage::onShowSortOfWorld(CCObject* pSender) {
	//展示全球排名
	//分享
	JniMethodInfo minfo;
	jobject jobj;
	bool b = JniHelper::getStaticMethodInfo(minfo, "com/zgy/hellobirdcoco/HelloBirdCoCo",  //类路径
			"cppCall_logsth",   //静态方法名
			"()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
	if (!b) {
		LOGD("JniHelper::getStaticMethodInfo error...");
	} else {
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	//
	b = JniHelper::getMethodInfo(minfo, "com/zgy/hellobirdcoco/HelloBirdCoCo", "showScore", "(I)V");
	if (!b) {
		LOGD("JniHelper::getMethodInfo error...");
	} else {
		LOGD("ready to invoke method...");
		minfo.env->CallVoidMethod(jobj, minfo.methodID, scoreRecord);
	}

}

void StartPage::onAudioSwitch(CCObject* pSender) {
//音乐开关
	if (AppDelegate::audioOn) {
		AppDelegate::audioOn = false;
		labelAudioSwitch->setString("开启声音");
		AppDelegate::ControlBackgroundSound(false);
	} else {
		AppDelegate::audioOn = true;
		labelAudioSwitch->setString("关闭声音");
		AppDelegate::ControlBackgroundSound(true);
	}
	CCUserDefault::sharedUserDefault()->setBoolForKey("audioOn", AppDelegate::audioOn);
}

void StartPage::onAbout(CCObject* pSender) {
	// 定义一个弹出层，传入一张背景图
	AlertDlg* pl = AlertDlg::create("bubble_grey_dlg.png");
	pl->setContentSize(CCSizeMake(CCDirector::sharedDirector()->getVisibleSize().width/1.5, CCDirector::sharedDirector()->getVisibleSize().height/2));
	pl->setTitle("小小鸟", 28);
	pl->setContentText("\n\n本游戏用于开发交流\n\n\nby ZGY.\nv1.0\n\n开发交流群88130145", 20, 5, 130, false);
	pl->setCallbackFunc(this, callfuncN_selector(StartPage::buttonCallback));
	pl->addButton("bubble_blue_btn_ok_n.png", "bubble_blue_btn_ok_p.png", "关闭", 0);
	pl->addButton("bubble_white_dlg_btn_n.png", "bubble_white_dlg_btn_p.png", "分享", 1);
	this->addChild(pl);
	if (scoreRecord > 0) {
		pScoreItem->setVisible(false);
		labelScore->setVisible(false);
	}

	labelAudioSwitch->setVisible(false);
	pAudioItem->setVisible(false);
	labelExit->setVisible(false);
	pExitItem->setVisible(false);
	pAboutItem->setVisible(false);
	labelAbout->setVisible(false);

}

void StartPage::buttonCallback(cocos2d::CCNode *pNode) {

	if (scoreRecord > 0) {
		pScoreItem->setVisible(true);
		labelScore->setVisible(true);
	}
	labelAudioSwitch->setVisible(true);
	pAudioItem->setVisible(true);
	labelExit->setVisible(true);
	pExitItem->setVisible(true);
	pAboutItem->setVisible(true);
	labelAbout->setVisible(true);

	if (pNode->getTag() == 1) {
		//分享
		JniMethodInfo minfo;
		jobject jobj;
		bool b = JniHelper::getStaticMethodInfo(minfo, "com/zgy/hellobirdcoco/HelloBirdCoCo",  //类路径
				"cppCall_logsth",   //静态方法名
				"()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		if (!b) {
			LOGD("JniHelper::getStaticMethodInfo error...");
		} else {
			jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		}
		//
		b = JniHelper::getMethodInfo(minfo, "com/zgy/hellobirdcoco/HelloBirdCoCo", "share", "()V");
		if (!b) {
			LOGD("JniHelper::getMethodInfo error...");
		} else {
			LOGD("ready to invoke method...");
			minfo.env->CallVoidMethod(jobj, minfo.methodID);
		}
	}
}

void StartPage::onExitTransitionDidStart() {
	CCLayer::onExitTransitionDidStart();
	CCLog("onExitTransitionDidStart  StartPage");
}
void StartPage::onEnter() {
	CCLog("onEnter  StartPage");
	CCLayer::onEnter();
}
void StartPage::onExit() {
	// 退出当前layer的时候调用，在这里都是做一些清除工作
	CCLog("onExit  StartPage");
	CCLayer::onExit();
	autorelease();
	CCDirector::sharedDirector()->purgeCachedData();
}
void StartPage::onEnterTransitionDidFinish() {
	CCLog("onEnterTransitionDidFinish  StartPage");
	CCLayer::onEnterTransitionDidFinish();
}

void StartPage::onExitOpera(CCObject* pSender) {
	AppDelegate::ControlBackgroundSound(false);
	CCLog(" start page onExit");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

