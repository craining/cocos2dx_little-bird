#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2Sprite.h"
#include "cocos-ext.h"
#include "AlertDlgLayer.h"

using namespace cocos2d;
using namespace extension;

#define RATIO 80.0f//像素与米的比例，在物理引擎中单位是MKS【米、千克、米】这里是为游戏中像素和物理世界中的米转换做的准备
#define LEVEL_ONE 1000//第一档
#define LEVEL_TWO 2500//第一档

class HelloWorld: public cocos2d::CCLayer, public b2ContactListener {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


    void onExitTransitionDidStart();//2，创建完layer的时候调用，也就是1调用完之后调用
    void onEnter();//1,创建时调用
    void onExit();//3，退出当前layer的时候调用，在这里都是做一些清除工作
    void onEnterTransitionDidFinish();//在3完成之后，调用该成员方法

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
	//
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
//	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void BeginContact(b2Contact* contact);

	virtual void update(float dt); //随时触发事件这里的dt就是你运行程序左下角三行数中的第二行数字

	virtual void keyBackClicked(); //监听Android 返回键



	b2World *world;
	B2Sprite *bird;
	CCMenuItemImage *pausebtn;
	CCSprite *barContainer;
	CCSprite *pReadyLabelBg;
	CCLabelTTF *pReadyLabel;
	CCLabelTTF* pScoreLabel;
	CCSprite* pPauseTip;
	CCSize screenSize;
	B2Sprite * ground1;
	B2Sprite * ground2;
	CCSprite* sky1;
	CCSprite* sky2;
	CCSprite* moon;
	CCSprite* sun;
	CCSprite* translateCover;

	CCSprite* tipSprit;
	CCLabelTTF* tipLabel;

	int scores;
	bool isOver;
	bool isReady;
	int flyHeightMax;//记录鸟儿飞翔的最大高度，落地时根据此高度判断弹起的高度
	int barMoveRate;
	int sky_ground_movePix;

	bool inNight;
	bool barColorChanged;

private:
	void addViews();
	void updateScore();
	void initWorld();
	void addBar(float dt);
	void startGame(float dt);
	void stopGame();
	void readyOver();
	void showPauseBtn();
	void gameOver();
	void changeTimer(); //倒计时
	void onPauseBtnClicked();
	void onPauseContinue(bool animTipIfPause);
	void buttonCallback(cocos2d::CCNode *pNode);//弹窗关闭按钮的回调
	void moveSun();
	void moveMoon();

};

#endif // __HELLOWORLD_SCENE_H__
