#include "HelloWorldScene.h"
#include "AppDelegate.h"
#include "StartPageScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

//增加游戏难度，时间越久(分数越高)，柱子越多
//鸟儿坠地弹起的高度，时刻记录鸟儿飞行的最大高度，每当触摸屏幕时，重置最大高度，坠地时根据最大高度，来决定弹起的高度
//增加声音、鸟儿多状态展示

CCScene* HelloWorld::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	CCLog("init  HelloWorld");
	if (!CCLayer::init()) {
		return false;
	}

	setTouchEnabled(false);
	setKeypadEnabled(true);	//设置可以监听按键

	screenSize = CCDirector::sharedDirector()->getVisibleSize();
	scores = 0;
	isOver = false;
	isReady = false;
	inNight = false;
	barColorChanged = true;
	barMoveRate = -2;
	sky_ground_movePix = 2;
	initWorld();
	addViews();
	return true;
}
void HelloWorld::initWorld() {

	b2Vec2 gravity;
	gravity.Set(0.0f, -12.0f);	//x不变，y递减10
	world = new b2World(gravity);	//创建世界，gravity可以理解为万有引力，物理中的g(重力加速度)

	world->SetContactListener(this);	//监听这个world里的碰撞
}
void HelloWorld::addViews() {
//	ground1 = B2Sprite::createWithSpriteFrameName("ground.png");
//	ground2 = B2Sprite::createWithSpriteFrameName("ground.png");
	ground1 = B2Sprite::create("ground.png");
	ground2 = B2Sprite::create("ground.png");
	CCSize size = ground1->getContentSize();
	//	//添加蓝天，因为鸟儿在蓝天中，所以和蓝天碰撞，是不会终止游戏的，因此，需保证蓝天不在鸟儿所在的world中
	sky1 = CCSprite::create("sky_2.png");
	sky2 = CCSprite::create("sky_2.png");
	sky1->setPosition(ccp(sky1->getContentSize().width/2, size.height + sky1->getContentSize().height/2));
	sky2->setPosition(ccp(sky1->getContentSize().width/2, size.height + sky1->getContentSize().height/2));
	addChild(sky1);
	addChild(sky2);

	sun = CCSprite::create("sun.png");
	moon = CCSprite::create("moon.png");
	moon->setPosition(ccp(screenSize.width+sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*4));
	sun->setPosition(ccp(screenSize.width+sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*4));
	addChild(sun);
	addChild(moon);
	//CCSize mapSize = bgCell1->getContentSize();
	//本次需要滚动的像素数

	//添加bar的容器
	barContainer = CCSprite::create();
	addChild(barContainer);
	//添加地板
//	ground1->setTag(5);
//	ground2->setTag(6);
//	ground1->setContentSize(ccp(screenSize.width, ground1->getContentSize().height));
//	ground2->setContentSize(ground1->getContentSize());

	b2BodyDef bDef;
	bDef.type = b2_staticBody;
	bDef.position = b2Vec2(size.width / 2 / RATIO, size.height / 2 / RATIO);
	b2Body *groundBody = world->CreateBody(&bDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundBody->CreateFixture(&groundFixtureDef);

	ground1->setB2Body(groundBody);
	ground1->setPTMRatio(RATIO);

	b2BodyDef bDef2;
	bDef2.type = b2_staticBody;
	bDef2.position = b2Vec2((ground1->getContentSize().width + size.width / 2) / RATIO, size.height / 2 / RATIO);
	b2Body *groundBody2 = world->CreateBody(&bDef2);

	b2PolygonShape groundShape2;
	groundShape2.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);
	b2FixtureDef groundFixtureDef2;
	groundFixtureDef2.shape = &groundShape2;
	groundBody2->CreateFixture(&groundFixtureDef2);

	ground2->setB2Body(groundBody2);
	ground2->setPTMRatio(RATIO);

	addChild(ground2);
	addChild(ground1);

//	translateCover = CCSprite::create("conver.png");
//	translateCover->setPosition(ccp(screenSize.width/2, screenSize.height/2));
//
//	addChild(translateCover);
	//添加顶部bar，防止小鸟飞出顶部边界
	//TODO
	B2Sprite *topbar = B2Sprite::create("top_bar.png");
//	B2Sprite *topbar = B2Sprite::createWithSpriteFrameName("top_bar.png");
	size = topbar->getContentSize();

	b2BodyDef bDefbar;
	bDefbar.type = b2_staticBody;
	bDefbar.position = b2Vec2(screenSize.width / 2 / RATIO, screenSize.height / RATIO);
	b2Body *barBody = world->CreateBody(&bDefbar);

	b2PolygonShape barShape;
	barShape.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);
	b2FixtureDef barFixtureDef;
	barFixtureDef.shape = &barShape;
	barBody->CreateFixture(&barFixtureDef);
	topbar->setB2Body(barBody);
	topbar->setPTMRatio(RATIO);
	addChild(topbar);

	//添加飞行的鸟儿
	//将鸟儿添加到world中，在world中的物体，可以监听到碰撞
	//TODO
	bird = B2Sprite::create("bird.png");
//	bird = B2Sprite::createWithSpriteFrameName("bird.png");
	size = bird->getContentSize();

	b2BodyDef bodyDef;	//定义刚体
	bodyDef.type = b2_dynamicBody;	//使刚体能够在力的作用下运行，刚体有三种：静态的、运动的、动态的
	bodyDef.position = b2Vec2(screenSize.width / 2 / RATIO, screenSize.height / 2 / RATIO);	//设置刚体的初始位置
	flyHeightMax = screenSize.height / 2;
	b2Body * birdBody = world->CreateBody(&bodyDef);	//在world中创建

	b2PolygonShape birdShape;
	birdShape.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);

	b2FixtureDef birdFixtureDef;
	birdFixtureDef.shape = &birdShape;
	birdBody->CreateFixture(&birdFixtureDef);

	/*
	 *
	 *  b2CircleShape dynamicBox;
	 * dynamicBox.m_radius = 18.0f/PTM_RATIO;//设置半径
	 * b2FixtureDef fixtureDef;
	 * fixtureDef.shape = &dynamicBox;//绑定形状
	 * fixtureDef.density = 1.0f;//设置密度
	 * fixtureDef.friction = 0.3f;//设置摩擦，当然还可以设置恢复，即反弹，这里我就不一一赘述
	 * body->CreateFixture( &fixtureDef );
	 *
	 * 给body施加一个力
	 * b2Vec2 force = body->GetWorldVector(b2Vec2(00.0f,0.0f));
	 * b2Vec2 point_force = body->GetWorldPoint( b2Vec2(0.4f,0.4f) );
	 * body->ApplyForce(force,point_force);
	 *
	 */

	bird->setPTMRatio(RATIO);
	bird->setB2Body(birdBody);
	addChild(bird);

	//添加得分展示

	CCLabelTTF* label = CCLabelTTF::create("得分: ", "", 20);

	label->setPosition(ccp(40, screenSize.height - label->getContentSize().height/2 - 15));
//	label->setPosition(ccp(40, ground1->getPositionY()));
	label->setColor(ccc3(0, 0, 0));
	pScoreLabel = CCLabelTTF::create("0", "", 20);

	// position the label on the center of the screen
//	pScoreLabel->setPosition(ccp(label->getContentSize().width + 40, ground1->getPositionY()));
	pScoreLabel->setPosition(ccp(label->getContentSize().width + 40, label->getPositionY()));
	pScoreLabel->setColor(ccc3(0, 0, 0));
	// add the label as a child to this layer
	addChild(label);
	addChild(pScoreLabel);
	//TODO
	tipSprit = CCSprite::create("bubble_red_gametip.png");
//	tipSprit = CCSprite::createWithSpriteFrameName("bubble_red_gametip.png");
//	tipSprit->setPosition(ccp(screenSize.width / 2, screenSize.height / 2 + bird->getContentSize().height + size.height));
	tipSprit->setPosition(bird->getPosition());
	tipLabel = CCLabelTTF::create("点击屏幕控制飞行\n别让小鸟飞出你的视线", "", 25);
	tipLabel->setColor(ccc3(0, 128, 255));
	tipLabel->setPosition(bird->getPosition());

	addChild(tipSprit);
	addChild(tipLabel);
//	CCActionInterval * blink = CCBlink::create(2, 3);
//	CCActionInterval* sequence = CCSequence::create(blink, CCCallFunc::create(this, callfunc_selector(HelloWorld::changeTimer)), NULL);
//	tipSprit->runAction(sequence);

	scheduleOnce(schedule_selector(HelloWorld::changeTimer), 3);
}

void HelloWorld::changeTimer() {
	//TODO
	pReadyLabelBg = CCSprite::create("bubble_yellow_ready.png");
//	pReadyLabelBg = CCSprite::createWithSpriteFrameName("bubble_yellow_ready.png");
	CCSize size = pReadyLabelBg->getContentSize();
	pReadyLabelBg->setPosition(bird->getPosition());
//	pReadyLabelBg->setPosition(ccp(screenSize.width / 2, screenSize.height / 2 + bird->getContentSize().height + size.height));

	pReadyLabel = CCLabelTTF::create("开始", "", 35);
	pReadyLabel->setColor(ccc3(255, 0, 0));
	pReadyLabel->setPosition(bird->getPosition());

	tipSprit->setVisible(false);
	tipLabel->setVisible(false);
	tipLabel->removeFromParentAndCleanup(true);
	tipSprit->removeFromParentAndCleanup(true);

	addChild(pReadyLabelBg);
	addChild(pReadyLabel);

	CCActionInterval * blink = CCBlink::create(2, 3);
//	CCActionInterval * fadeout = CCFadeOut::create(2);
	CCActionInterval * scale = CCScaleTo::create(2, 0.0);
	CCActionInterval* sequence = CCSequence::create(blink, CCCallFunc::create(this, callfunc_selector(HelloWorld::readyOver)), NULL);
	pReadyLabel->runAction(sequence);
}

void HelloWorld::readyOver() {
	CCLog("ready over");
	setTouchEnabled(true);
	isReady = true;
	pReadyLabel->stopAllActions();
	pReadyLabel->setVisible(false);
	pReadyLabel->removeFromParentAndCleanup(true);
	pReadyLabelBg->stopAllActions();
	pReadyLabelBg->setVisible(false);
	pReadyLabelBg->removeFromParentAndCleanup(true);
	showPauseBtn();

	//鸟儿开始飞
	CCAnimation *animation = CCAnimation::create();
	//TODO
	animation->addSpriteFrameWithFileName("bird_fly_down_yellow.png");
	animation->addSpriteFrameWithFileName("bird_fly_up_yellow.png");
//	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName("bird_fly_down_yellow.png")->displayFrame());
//	animation->addSpriteFrame(CCSprite::createWithSpriteFrameName("bird_fly_up_yellow.png")->displayFrame());
	animation->setDelayPerUnit(0.2f / 2.0f); // 这个动画包含2帧，将会持续0.1秒.
	animation->setLoops(-1);
	CCAnimate *action = CCAnimate::create(animation);
	bird->runAction(action);  // 运行精灵对象

	startGame(0);
//	sun->setPosition(ccp(screenSize.width+sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*4));

//	CCActionInterval * move = CCMoveTo::create(3, ccp(screenSize.width-sun->getContentSize().width/2, ground1->getContentSize().height/2 + ground1->getPositionY()-sun->getContentSize().height/2));
	ccBezierConfig bezierCon;
	bezierCon.controlPoint_1 = CCPointMake(sun->getContentSize().width, screenSize.height-sun->getContentSize().height/2); //控制点1
	bezierCon.controlPoint_2 = CCPointMake(screenSize.width, screenSize.height-sun->getContentSize().height*2); //控制点2
	bezierCon.endPosition = CCPointMake(0-sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*3); // 结束位置

	CCActionInterval * move = CCBezierTo::create(20, bezierCon);
//	CCActionInterval * scaleto = CCScaleTo::create(3, 0.2f);
//	  CCActionInterval * easeExponentialInOut= CCEaseExponentialInOut::create(move);
//	  CCActionInterval * easeSineInOut = CCEaseSineInOut::create(move);
//	CCFiniteTimeAction* seq = CCSequence::create(tinttoDark, tinttoLight, NULL);

	CCActionInterval* sequence = CCSequence::create(move, CCCallFunc::create(this, callfunc_selector(HelloWorld::moveMoon)), NULL);
	sun->runAction(sequence);

}

void HelloWorld::moveMoon() {

	//	    CCActionInterval * tinttoDark = CCTintTo::create(3, 71, 60, 139);
	//	    CCActionInterval * tinttoLight = CCTintTo::create(3, 117, 186, 255);
	//	    CCFiniteTimeAction* seq = CCSequence::create(tinttoDark,tinttoLight,NULL);
	//	  	    CCActionInterval * repeatForeverSky =CCRepeatForever::create((CCActionInterval* )seq);
	//	  	    sky1->runAction(repeatForeverSky);
	//	  	  sky2->runAction(repeatForeverSky);
	//
	//
	//	      CCActionInterval * tinttoDarkGround = CCTintTo::create(3, 0, 100, 0);
	//	  	    CCActionInterval * tinttoLightGround = CCTintTo::create(3, 101, 184, 66);
	//	  	  CCFiniteTimeAction* seqGround = CCSequence::create(tinttoDarkGround,tinttoLightGround,NULL);
	//	  	 	  	    CCActionInterval * repeatForeverGround =CCRepeatForever::create((CCActionInterval* )seqGround);
	//	  	 	  	    ground1->runAction(repeatForeverGround);
	//	  	 	  	    ground2->runAction(repeatForeverGround);

	inNight = true;
	barColorChanged = false;
	sky1->runAction(CCTintTo::create(1, 71, 60, 139));
	sky2->runAction(CCTintTo::create(1, 71, 60, 139));
	ground1->runAction(CCTintTo::create(1, 0, 100, 0));
	ground2->runAction(CCTintTo::create(1, 0, 100, 0));

	moon->setPosition(ccp(screenSize.width+moon->getContentSize().width/2, screenSize.height-moon->getContentSize().height*4));
	ccBezierConfig bezierCon;
	bezierCon.controlPoint_1 = CCPointMake(moon->getContentSize().width, screenSize.height-moon->getContentSize().height/2); //控制点1
	bezierCon.controlPoint_2 = CCPointMake(screenSize.width, screenSize.height-moon->getContentSize().height*2); //控制点2
	bezierCon.endPosition = CCPointMake(0-moon->getContentSize().width/2, screenSize.height-moon->getContentSize().height*3); // 结束位置
	CCActionInterval * move = CCBezierTo::create(20, bezierCon);
	CCActionInterval* sequence = CCSequence::create(move, CCCallFunc::create(this, callfunc_selector(HelloWorld::moveSun)), NULL);
	moon->runAction(sequence);
}

void HelloWorld::moveSun() {
	//太阳开始移动
	inNight = false;
	barColorChanged = false;
	sky1->runAction(CCTintTo::create(1, 255, 255, 255));
	sky2->runAction(CCTintTo::create(1, 255, 255, 255));
	ground1->runAction(CCTintTo::create(1, 255, 255, 255));
	ground2->runAction(CCTintTo::create(1, 255, 255, 255));

//	sky1->runAction(sky1TintTo->reverse());
//	sky2->runAction(sky2TintTo->reverse());
//	ground1->runAction(ground1TintTo->reverse());
//	ground2->runAction(ground2TintTo->reverse());

	sun->setPosition(ccp(screenSize.width+sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*4));

//	CCActionInterval * move = CCMoveTo::create(3, ccp(screenSize.width-sun->getContentSize().width/2, ground1->getContentSize().height/2 + ground1->getPositionY()-sun->getContentSize().height/2));
	ccBezierConfig bezierCon;
	bezierCon.controlPoint_1 = CCPointMake(sun->getContentSize().width, screenSize.height-sun->getContentSize().height/2); //控制点1
	bezierCon.controlPoint_2 = CCPointMake(screenSize.width, screenSize.height-sun->getContentSize().height*2); //控制点2
	bezierCon.endPosition = CCPointMake(0-sun->getContentSize().width/2, screenSize.height-sun->getContentSize().height*3); // 结束位置

	CCActionInterval * move = CCBezierTo::create(20, bezierCon);
//	CCActionInterval * scaleto = CCScaleTo::create(3, 0.2f);
//	  CCActionInterval * easeExponentialInOut= CCEaseExponentialInOut::create(move);
//	  CCActionInterval * easeSineInOut = CCEaseSineInOut::create(move);
//	CCFiniteTimeAction* seq = CCSequence::create(tinttoDark, tinttoLight, NULL);

	CCActionInterval* sequence = CCSequence::create(move, CCCallFunc::create(this, callfunc_selector(HelloWorld::moveMoon)), NULL);
	sun->runAction(sequence);

//	addChild(translateCover);
//	CCActionInterval * fadeOut = CCFadeOut::create(3);
//	CCActionInterval * fadeIn = CCFadeIn::create(3);
//	  CCFiniteTimeAction* seq = CCSequence::create(fadeIn,fadeOut,NULL);
//	  CCActionInterval * repeatForeverSky =CCRepeatForever::create((CCActionInterval* )seq);
//	translateCover->runAction(repeatForeverSky);
}

void HelloWorld::showPauseBtn() {
	//TODO
	pausebtn = CCMenuItemImage::create("ic_pause_black.png", "ic_pause_black.png", this, menu_selector(HelloWorld::onPauseBtnClicked));
//	pausebtn = CCMenuItemImage::create(NULL, NULL, this, menu_selector(HelloWorld::onPauseBtnClicked));
//	pausebtn->setNormalSpriteFrame(CCSprite::createWithSpriteFrameName("ic_pause_black.png")->displayFrame());
//	pausebtn->setSelectedSpriteFrame(CCSprite::createWithSpriteFrameName("ic_pause_black.png")->displayFrame());

//	pausebtn->setPosition(ccp(screenSize.width-pausebtn->getContentSize().width-5, screenSize.height-pausebtn->getContentSize().height-5));
	pausebtn->setPosition(ccp(screenSize.width-pausebtn->getContentSize().width-5, pScoreLabel->getPositionY()));
	CCMenu* pMenu = CCMenu::create(pausebtn, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
//	pPauseTip = CCSprite::createWithSpriteFrameName("bubble_red.png");
	pPauseTip = CCSprite::create("bubble_red.png");
	pPauseTip->setPosition(ccp(screenSize.width/2, screenSize.height - pPauseTip->getContentSize().height - 20));

	CCLabelTTF *pPauseTiplabel = CCLabelTTF::create("\n触摸屏幕\n继续游戏", "", 35);
	pPauseTiplabel->setPosition(ccp(pPauseTip->getContentSize().width/2, pPauseTip->getContentSize().height/2+20));
	pPauseTiplabel->setColor(ccc3(0, 128, 255));
	this->addChild(pPauseTip);
	pPauseTip->addChild(pPauseTiplabel);
	pPauseTip->setVisible(false);
}

void HelloWorld::onPauseBtnClicked() {
	onPauseContinue(true);
}

void HelloWorld::onPauseContinue(bool animTipIfPause) {
	if (pausebtn->isVisible()) {
		pausebtn->setVisible(false);
		pPauseTip->setVisible(true);
		if (animTipIfPause) {
			CCActionInterval * blink = CCBlink::create(1, 3);
			pPauseTip->runAction(blink);
		}
		stopGame();
	} else {
		pausebtn->setVisible(true);
		pPauseTip->setVisible(false);
		startGame(0);
	}

}

void HelloWorld::startGame(float dt) {
	scheduleUpdate();
	schedule(schedule_selector(HelloWorld::addBar), 2);	//每2秒增加柱子
	sky1->resumeSchedulerAndActions();
	sky2->resumeSchedulerAndActions();
	ground1->resumeSchedulerAndActions();
	ground2->resumeSchedulerAndActions();
	sun->resumeSchedulerAndActions();
	moon->resumeSchedulerAndActions();
}

void HelloWorld::stopGame() {
	unscheduleUpdate();	//取消更新
	unschedule(schedule_selector(HelloWorld::addBar));	//取消柱子的增加

	sky1->pauseSchedulerAndActions();
	sky2->pauseSchedulerAndActions();
	ground1->pauseSchedulerAndActions();
	ground2->pauseSchedulerAndActions();
	sun->pauseSchedulerAndActions();
	moon->pauseSchedulerAndActions();

}

void HelloWorld::addBar(float dt) {

	float offset = -rand() % 5;

	//down bar

	B2Sprite * down_bar;
	B2Sprite * up_bar;
//	switch (barMoveRate) {
//	case -2:
//		//TODO
////		 down_bar = B2Sprite::createWithSpriteFrameName("down_bar.png");
//		down_bar = B2Sprite::create("bar_down_one.png");
//		up_bar = B2Sprite::create("bar_up_one.png");
//		break;
//	case -3:
//		down_bar = B2Sprite::create("bar_down_two.png");
//		up_bar = B2Sprite::create("bar_up_two.png");
//		break;
//	case -4:
//		down_bar = B2Sprite::create("bar_down_three.png");
//		up_bar = B2Sprite::create("bar_up_three.png");
//		break;
//	}
	down_bar = B2Sprite::create("bar_down_one.png");
	up_bar = B2Sprite::create("bar_up_one.png");
	up_bar->setTag(10);
	down_bar->setTag(10);

//	if (inNight) {
//		down_bar = B2Sprite::create("bar_down_three.png");
//		up_bar = B2Sprite::create("bar_up_three.png");
//	} else {
//		down_bar = B2Sprite::create("bar_down_one.png");
//		up_bar = B2Sprite::create("bar_up_one.png");
//	}

	CCSize down_bar_size = down_bar->getContentSize();

	b2BodyDef down_bar_body_def;
	down_bar_body_def.type = b2_kinematicBody;
	down_bar_body_def.position = b2Vec2(screenSize.width / RATIO + 2, down_bar_size.height / RATIO / 2 + offset);
	down_bar_body_def.linearVelocity = b2Vec2(barMoveRate, 0);	//x递减，y不变
	b2Body * down_bar_body = world->CreateBody(&down_bar_body_def);

	b2PolygonShape down_bar_shape;
	down_bar_shape.SetAsBox(down_bar_size.width / 2 / RATIO, down_bar_size.height / 2 / RATIO);
	b2FixtureDef down_bar_fixture_def;
	down_bar_fixture_def.shape = &down_bar_shape;
	down_bar_body->CreateFixture(&down_bar_fixture_def);

	down_bar->setB2Body(down_bar_body);
	down_bar->setPTMRatio(RATIO);
	barContainer->addChild(down_bar);

	CCSize up_bar_size = up_bar->getContentSize();

	b2BodyDef up_bar_body_def;
	up_bar_body_def.type = b2_kinematicBody;
	up_bar_body_def.position = b2Vec2(screenSize.width / RATIO + 2, down_bar_size.height / RATIO + offset + 2 + up_bar_size.height / 2 / RATIO);
	up_bar_body_def.linearVelocity = b2Vec2(barMoveRate, 0);	//x递减，y不变
	b2Body * up_bar_body = world->CreateBody(&up_bar_body_def);

	b2PolygonShape up_bar_shape;
	up_bar_shape.SetAsBox(up_bar_size.width / 2 / RATIO, up_bar_size.height / 2 / RATIO);
	b2FixtureDef up_bar_fixture_def;
	up_bar_fixture_def.shape = &up_bar_shape;
	up_bar_body->CreateFixture(&up_bar_fixture_def);

	up_bar->setB2Body(up_bar_body);
	up_bar->setPTMRatio(RATIO);
	barContainer->addChild(up_bar);

	if (inNight) {
		up_bar->runAction(CCTintTo::create(0.2, 71, 60, 139));
		down_bar->runAction(CCTintTo::create(0.2, 71, 60, 139));
	} else {
		up_bar->runAction(CCTintTo::create(0.2, 255, 255, 255));
		down_bar->runAction(CCTintTo::create(0.2, 255, 255, 255));
	}

}

void HelloWorld::updateScore() {
	scores += 1;
	pScoreLabel->setString(CCString::createWithFormat("%d", scores)->getCString());
	if (scores - LEVEL_TWO > 0 && barMoveRate + 3 == 0) {
		//第二档
//		unscheduleUpdate();	//取消更新
//		unschedule(schedule_selector(HelloWorld::addBar));	//取消柱子的增加
		barMoveRate = -4;
		sky_ground_movePix = 4;
//		scheduleUpdate();
//		schedule(schedule_selector(HelloWorld::addBar), 1);	// 增加柱子
	} else if (scores - LEVEL_ONE > 0 && barMoveRate + 2 == 0) {
		//第一档
//		unscheduleUpdate();	//取消更新
//		unschedule(schedule_selector(HelloWorld::addBar));	//取消柱子的增加
		barMoveRate = -3;
		sky_ground_movePix = 3;
//		scheduleUpdate();
//		schedule(schedule_selector(HelloWorld::addBar), 1.5);	// 增加柱子
	}
}
void HelloWorld::update(float dt) {
	updateScore();	//分数累积
	world->Step(dt, 8, 3);	//这个step是控制鸟儿自由落体的
	CCSprite *s;
	//鸟儿被挤出了，则over
	if ((bird->getPositionX() + bird->getContentSize().width / 2 < 0) || (bird->getPositionX() - bird->getContentSize().width / 2 - screenSize.width > 0)) {
		gameOver();
	} else {
		if (bird->getPositionY() - bird->getContentSize().height / 2 - flyHeightMax > 0) {
			flyHeightMax = bird->getPositionY() - bird->getContentSize().height / 2;
		}
		//背景移动逻辑
		//地面移动
		ground1->setPosition(ccp(ground1->getPosition().x-sky_ground_movePix,ground1->getPositionY() ));
		ground2->setPosition(ccp(ground2->getPosition().x-sky_ground_movePix,ground2->getPositionY()));
		if (ground2->getPositionX() <= ground2->getContentSize().width / 2 && ground1->getPositionX() < ground1->getContentSize().width / 2) {
			ground1->setPosition(ccp(ground2->getPositionX() + ground2->getContentSize().width, ground1->getPositionY()));
		}
		if (ground1->getPositionX() <= ground2->getContentSize().width / 2 && ground2->getPositionX() < ground1->getContentSize().width / 2) {
			ground2->setPosition(ccp(ground1->getPositionX()+ground2->getContentSize().width, ground2->getPositionY()));
		}

//		if (ground1->getPositionX() <= -ground1->getContentSize().width * 0.5) {
//			ground1->setPosition(ccp(ground1->getContentSize().width*1.5f,ground1->getPositionY()));
//		} else {
//			ground1->setPosition(ccpAdd(ground1->getPosition(), ccp(-sky_ground_movePix,0)));
//		}
//		if (ground2->getPositionX() <= -ground2->getContentSize().width * 0.5) {
//			ground2->setPosition(ccp(ground2->getContentSize().width*1.5f,ground2->getPositionY()));
//		} else {
//			ground2->setPosition(ccpAdd(ground2->getPosition(), ccp(-sky_ground_movePix,0)));
//		}

			//蓝天移动
		sky1->setPosition(ccp(sky1->getPositionX() - sky_ground_movePix, (ground1->getContentSize().height + sky1->getContentSize().height/2)));
		sky2->setPosition(ccp(sky2->getPositionX() - sky_ground_movePix, (ground1->getContentSize().height + sky1->getContentSize().height/2)));
		if (sky1->getPositionX() <= sky1->getContentSize().width / 2 && sky2->getPositionX() < sky2->getContentSize().width / 2) {
			sky2->setPosition(ccp(sky1->getPositionX()+sky1->getContentSize().width, sky2->getPositionY()));
		}
		if (sky2->getPositionX() <= sky2->getContentSize().width / 2 && sky1->getPositionX() < sky1->getContentSize().width / 2) {
			sky1->setPosition(ccp(sky2->getPositionX()+sky2->getContentSize().width, sky1->getPositionY()));
		}

//		if (sky1->getPositionX() <= -sky1->getContentSize().width * 0.5) {
//			sky1->setPosition(ccp(sky1->getContentSize().width*1.5f,sky1->getPositionY()));
//		} else {
//			sky1->setPosition(ccpAdd(sky1->getPosition(), ccp(-sky_ground_movePix,0)));
//		}
//
//			//当第二张图超出屏幕外，将其重置坐标，接在当前显示的图片下
//		if (sky2->getPositionX() <= -sky2->getContentSize().width * 0.5) {
//			sky2->setPosition(ccp(sky2->getContentSize().width*1.5f,sky2->getPositionY()));
//		} else {
//			sky2->setPosition(ccpAdd(sky2->getPosition(), ccp(-sky_ground_movePix,0)));
//		}
				}

			//释放移走的柱子
	for (b2Body *b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
		if (b->GetPosition().x < -3) {
			s = (CCSprite*) b->GetUserData();
//			CCLog("s.tag=%d", s->getTag());
			if (s != NULL && s->getTag() == 10) {
				s->removeFromParent();
				world->DestroyBody(b);
			}
		} else {
			s = (CCSprite*) b->GetUserData();
			if (s != NULL && s->getTag() == 10 && !barColorChanged) {
				CCLog("change bar after day changed");
				if (inNight) {
					s->runAction(CCTintTo::create(0.2, 71, 60, 139));
				} else {
					s->runAction(CCTintTo::create(0.2, 255, 255, 255));
				}
			}

		}
	}
	barColorChanged = true;
}

void HelloWorld::BeginContact(b2Contact *contact) {

	//只要撞鸟
	if (contact->GetFixtureA()->GetBody()->GetUserData() == bird || contact->GetFixtureB()->GetBody()->GetUserData() == bird) {
		if (contact->GetFixtureA()->GetBody()->GetUserData() == ground2 || contact->GetFixtureB()->GetBody()->GetUserData() == ground2 || contact->GetFixtureA()->GetBody()->GetUserData() == ground1
				|| contact->GetFixtureB()->GetBody()->GetUserData() == ground1) {
			//flyHeightMax 是鸟儿降落时的高度，根据此高度计算跳跃高度
			CCLog("flyHeightMax %d", flyHeightMax);
			bird->getB2Body()->SetLinearVelocity(b2Vec2(2, flyHeightMax / RATIO));	//鸟儿弹起
			flyHeightMax = 0;
		}
		if (AppDelegate::audioOn) {
			SimpleAudioEngine::sharedEngine()->playEffect("bone.mp3");
		}
	}
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
	CCLog("touch ");
	flyHeightMax = 0;
	if (pausebtn && pausebtn->isVisible() == false) {
		onPauseContinue(true);
	} else {
		bird->getB2Body()->SetLinearVelocity(b2Vec2(0, 5));	//鸟儿跳跃4
		if (AppDelegate::audioOn) {
			SimpleAudioEngine::sharedEngine()->playEffect("flappy.mp3");
		}
	}

}

void HelloWorld::gameOver() {
	isOver = true;
	pausebtn->setVisible(false);
	//我们这里简单存储条数据
	int scorePre = CCUserDefault::sharedUserDefault()->getIntegerForKey("maxscore");
	const char* showContent = CCString::createWithFormat("\n当前得分: %d", scores)->getCString();
	bool blink = false;
	if (scores > scorePre) {
		blink = true;
		showContent = CCString::createWithFormat("\n历史最高分: %d  ！", scores)->getCString();
		CCUserDefault::sharedUserDefault()->setIntegerForKey("maxscore", scores);
		CCUserDefault::sharedUserDefault()->flush();
	}

	stopGame();

	if (AppDelegate::audioOn) {
		SimpleAudioEngine::sharedEngine()->playEffect("over.mp3");
	}

	// 定义一个弹出层，传入一张背景图
	AlertDlg* overDlg = AlertDlg::create("bubble_grey_dlg.png");
	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
	overDlg->setContentSize(ccp(screenSize.width/1.5, screenSize.height/3));
	overDlg->setTitle("游戏结束", 28);
	overDlg->setContentText(showContent, 20, 5, 150, blink);
	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
	// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
	overDlg->setCallbackFunc(this, callfuncN_selector(HelloWorld::buttonCallback));
	// 添加按钮，设置图片，文字，tag 信息
	overDlg->addButton("bubble_blue_btn_ok_n.png", "bubble_blue_btn_ok_p.png", "关闭", 2);
	// 添加到当前层
	this->addChild(overDlg);

}

void HelloWorld::buttonCallback(cocos2d::CCNode *pNode) {

	switch (pNode->getTag()) {
	case 1: {
		pPauseTip->setVisible(true);
		break;
	}
	case 2: {
		CCTransitionScene * reScene = CCTransitionFadeBL::create(0.5, StartPage::scene());
		CCDirector::sharedDirector()->replaceScene(reScene);
		break;
	}
	default:
		break;
	}
}

void HelloWorld::keyBackClicked() {
	if (isOver) {
		return;
	}
	if (isReady == false) {
		CCTransitionScene * reScene = CCTransitionFadeBL::create(0.5, StartPage::scene());
		CCDirector::sharedDirector()->replaceScene(reScene);
		return;
	}
	if (pausebtn && pausebtn->isVisible()) {
		onPauseContinue(false);
	}
	// 定义一个弹出层，传入一张背景图
	AlertDlg* pl = AlertDlg::create("bubble_grey_dlg.png");
	pl->setContentSize(ccp(screenSize.width/1.5, screenSize.height/3));
	pl->setTitle("警告", 28);
	pl->setContentText("您确定要放弃本次游戏吗？", 20, 5, 150, true);
	pl->setCallbackFunc(this, callfuncN_selector(HelloWorld::buttonCallback));
	pl->addButton("bubble_blue_btn_ok_n.png", "bubble_blue_btn_ok_p.png", "确定", 2);
	pl->addButton("bubble_white_dlg_btn_n.png", "bubble_white_dlg_btn_p.png", "取消", 1);

	// 添加到当前层
	this->addChild(pl);
	pPauseTip->setVisible(false);
}

void HelloWorld::onExitTransitionDidStart() {
	CCLog("onExitTransitionDidStart  hello");
	CCLayer::onExitTransitionDidStart();
	//2，创建完layer的时候调用，也就是1调用完之后调用
}
void HelloWorld::onEnter() {
	CCLog("onEnter  hello");
	CCLayer::onEnter();
}
void HelloWorld::onExit() {

	CCLayer::onExit();

	CCLog("onExit  hello");
	//3，退出当前layer的时候调用，在这里都是做一些清除工作
	CCSprite *s;
	for (b2Body *b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
		s = (CCSprite*) b->GetUserData();
		if (s != NULL) {
			s->removeFromParent();
		}
		world->DestroyBody(b);
	}

	removeAllChildren();
	autorelease();
	CCDirector::sharedDirector()->purgeCachedData();
}
void HelloWorld::onEnterTransitionDidFinish() {
	CCLog("onEnterTransitionDidFinish  hello");
	CCLayer::onEnterTransitionDidFinish();
}
