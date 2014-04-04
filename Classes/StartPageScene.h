/*
 * StartPage.h
 *
 *  Created on: 2014-2-24
 *      Author: zhuanggy
 */

#ifndef STARTPAGE_H_
#define STARTPAGE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2Sprite.h"

class StartPage: public cocos2d::CCLayer {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void onExitTransitionDidStart(); //2，创建完layer的时候调用，也就是1调用完之后调用
	void onEnter(); //1,创建时调用
	void onExit(); //3，退出当前layer的时候调用，在这里都是做一些清除工作
	void onEnterTransitionDidFinish(); //在3完成之后，调用该成员方法

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	// implement the "static node()" method manually

	CREATE_FUNC(StartPage)
	;
	//
	CCMenuItemImage* pScoreItem;
	CCLabelTTF* labelScore;

	CCMenuItemImage* pAudioItem;
	CCLabelTTF* labelAudioSwitch;

	CCMenuItemImage * pExitItem;
	CCLabelTTF* labelExit;

	CCMenuItemImage * pAboutItem;
	CCLabelTTF* labelAbout;


	CCMenuItemImage* pFlowerRed;
	CCMenuItemImage* pFlowerWhite;

	int scoreRecord;

	void onStartGame(CCObject* pSender);
	void onExitOpera(CCObject* pSender);
	void onAbout(CCObject* pSender);
	void onSet(CCObject* pSender);
	void onAudioSwitch(CCObject* pSender);
	void onShowSortOfWorld(CCObject* pSender);
	void onFlowerRedClicked(CCObject* pSender);
	void onFlowerWhiteClicked(CCObject* pSender);
	void addViews();

	void buttonCallback(cocos2d::CCNode *pNode); //弹窗关闭按钮的回调
};

#endif /* STARTPAGE_H_ */
