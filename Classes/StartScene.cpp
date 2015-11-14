/*
 * StartScene.cpp
 *
 *  Created on: 2015/10/11
 *      Author: 和仁
 */

#include "StartScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

const int MENU_BUTTON_POS_Y = 50;

const int START_BUTTON_POS_Y = 130;

const int BUTTON_INTERVAL = 75;

StartScene::StartScene()
: _atmain(true)
, _ruleScene(nullptr)
, _rankScene(nullptr)
{
	// TODO 自動生成されたコンストラクター・スタブ

}

StartScene::~StartScene() {
	// TODO Auto-generated destructor stub
	CC_SAFE_RELEASE_NULL(_ruleScene);
	CC_SAFE_RELEASE_NULL(_rankScene);
}

Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if(!Layer::init()){
		return false;
	}

	// オブジェクトを配置する
	this->itemArrange();

	// 画面をタッチした時にメイン画面へ遷移
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event){
		if (_ruleScene->getAtmain() && _rankScene->getAtmain()){// メイン画面が表示されていれば

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
			// 何度も押せないように一度押したらアクションを無効化する
			this->getEventDispatcher()->removeAllEventListeners();

			// ゲームを始めるアクション
			auto startGame = CallFunc::create([]{
					auto scene = GameScene::createScene();
					auto transition = TransitionPageTurn::create(0.5, scene, false);
					Director::getInstance()->replaceScene(transition);
			});
			this->runAction(startGame);
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void StartScene::update(float dt)
{

}

void StartScene::itemArrange()
{
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();

	// 背景の作成
	auto background = Sprite::create("Title.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(background);

	// スタートロゴの作成
	auto startLogo = Sprite::create("start_logo.png");
	startLogo->setPosition(Vec2(winSize.width / 2, START_BUTTON_POS_Y));
	this->addChild(startLogo);
	// 点滅アクション
	auto blink = Sequence::create(FadeOut::create(1.0), FadeIn::create(1.0), NULL);
	startLogo->runAction(RepeatForever::create(blink));

	// ルール画面の作成
	auto rulescene = RuleScene::create();
	this->setRuleScene(rulescene);

	// ランク画面の作成
	auto rankscene = RankScene::create();
	this->setRankScene(rankscene);

	// ルールボタンの作成
	auto ruleButton = MenuItemImage::create("rule_button.png", "rule_button_pressed.png", [this](Ref *sender){
		if(_ruleScene->getAtmain() && _rankScene->getAtmain()){// メイン画面が表示されていれば
			// ルール画面を表示する
			this->addChild(_ruleScene, 7);

			// メイン画面でないフラグを立てる
			_ruleScene->setAtmain(false);
		}
		//auto transition = TransitionPageTurn::create(0.5, scene, true);
		//Director::getInstance()->replaceScene(transition);
	});

	// ランキングボタンの作成
	auto rankButton = MenuItemImage::create("rank_button_home.png", "rank_button_home_pressed.png",
			[this](Ref* ref){
		if(_ruleScene->getAtmain() && _rankScene->getAtmain()){// メイン画面が表示されていれば
			// ランク画面を表示する
			this->addChild(_rankScene, 7);

			// メイン画面でないフラグを立てる
			_rankScene->setAtmain(false);
		}
		//auto transition = TransitionPageTurn::create(0.5, scene, true);
		//Director::getInstance()->replaceScene(transition);
	});

	// ボタンメニューの作成
	auto menu = Menu::create(ruleButton, rankButton, nullptr);
	this->addChild(menu);
	menu->setPosition(winSize.width / 2.0, MENU_BUTTON_POS_Y);
	menu->alignItemsHorizontallyWithPadding(BUTTON_INTERVAL);

}


void StartScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("opening.mp3", true);
}
