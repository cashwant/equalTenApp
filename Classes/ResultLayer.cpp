/*
 * ResultLayer.cpp
 *
 *  Created on: 2015/10/13
 *      Author: 和仁
 */

#include "ResultLayer.h"
#include "StartScene.h"
#include "RankScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// スコアラベル位置
const int RESULT_SCORE_LABEL_POS_Y = 250;

// パネル枚数ラベル位置
const int RESULT_PANELS_LABEL_POS_X = 68;
const int RESULT_PANELS_LABEL_POS_Y = 170;

// コンボ数ラベル位置
const int RESULT_MAX_COMBO_LABEL_POS_X = 192.7;
const int RESULT_MAX_COMBO_LABEL_POS_Y = 170;

// 評価ランク表示位置
const int STAR_POS_Y = 90;
// 評価用スターの幅と高さ
const int STAR_WIDTH = 125;
const int STAR_HEIGHT = 25;

// ボタンメニューの位置
const int MENU_POS_Y = 35;
// ボタンの間隔
const int MENU_INTERVAL = 30;

ResultLayer::ResultLayer()
: _resultScoreLabel(NULL)
, _resultPanelsLabel(NULL)
, _resultMaxComboLabel(NULL)
, _resultBackground(NULL)
{
	// TODO 自動生成されたコンストラクター・スタブ

}

ResultLayer::~ResultLayer() {
	// TODO Auto-generated destructor stub
}

bool ResultLayer::init()
{
	if(!Layer::init()){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();


	// 結果画面の生成
	auto resultBackground = Sprite::create("result_background.png");
	auto backSize = resultBackground->getContentSize();
	resultBackground->setPosition(Vec2(winSize.width / 2, winSize.height * 3 / 2));
	this->setResultBackground(resultBackground);
	this->addChild(resultBackground, 1);

	// 結果画面の出現
	auto delay = DelayTime::create(1.0);
	auto moveto = MoveTo::create(1.0, Vec2(winSize.width / 2, winSize.height / 2));
	auto resultSequence = Sequence::create(delay, moveto, NULL);
	resultBackground->runAction(resultSequence);

	// スコアラベルの作成
	auto scoreLabel = Label::createWithSystemFont("0", "Latha", 20);
	scoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	scoreLabel->enableOutline(Color4B::BLACK, 1.5);
	scoreLabel->setPosition(Vec2(backSize.width / 2, RESULT_SCORE_LABEL_POS_Y));
	this->setResultScoreLabel(scoreLabel);
	resultBackground->addChild(scoreLabel);

	// パネル枚数ラベルの作成
	auto panelsLabel = Label::createWithSystemFont("0", "Latha", 20);
	panelsLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	panelsLabel->enableOutline(Color4B::BLACK, 1.5);
	panelsLabel->setPosition(Vec2(RESULT_PANELS_LABEL_POS_X, RESULT_PANELS_LABEL_POS_Y));
	this->setResultPanelsLabel(panelsLabel);
	resultBackground->addChild(panelsLabel);


	// maxコンボラベルの作成
	auto maxComboLabel = Label::createWithSystemFont("0", "Latha", 20);
	maxComboLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	maxComboLabel->enableOutline(Color4B::BLACK, 1.5);
	maxComboLabel->setPosition(Vec2(RESULT_MAX_COMBO_LABEL_POS_X, RESULT_MAX_COMBO_LABEL_POS_Y));
	this->setResultMaxComboLabel(maxComboLabel);
	resultBackground->addChild(maxComboLabel);


	//リプレイボタンの作成
	auto replayButton = MenuItemImage::create("replay_button.png", "replay_button_pressed.png",
			[](Ref* ref){
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
	});

	// ホームボタンの作成
	auto homeButton = MenuItemImage::create("home_button_result.png", "home_button_result_pressed.png",
			[](Ref* ref){
		auto scene = StartScene::createScene();
		auto transition = TransitionPageTurn::create(0.5, scene, true);
		Director::getInstance()->replaceScene(transition);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
	});

	// ランキングボタンの作成
	auto rankButton = MenuItemImage::create("rank_button_result.png", "rank_button_result_pressed.png",
			[](Ref* ref){
		auto scene = StartScene::createScene();
		auto layer = RankScene::create();
		layer->setAtmain(false);
		scene->addChild(layer);
		auto transition = TransitionPageTurn::create(0.5, scene, true);
		Director::getInstance()->replaceScene(transition);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
	});

	auto menu = Menu::create(homeButton, replayButton, rankButton, NULL);
	resultBackground->addChild(menu, 2);
	menu->setPosition(Vec2(backSize.width / 2, MENU_POS_Y));

	menu->alignItemsHorizontallyWithPadding(MENU_INTERVAL); // ボタンを横に並べる

	return true;
}

Layer* ResultLayer::createLayer()
{
	auto scene = Layer::create();
	auto layer = ResultLayer::create();
	scene->addChild(layer);
	return scene;
}

void ResultLayer::createStar(int score)
{
	auto scoreRank = 0;
	if (score < 100) {
		scoreRank = 0;
	} else if (score < 1000) {
		scoreRank = 1;
	} else if (score < 10000) {
		scoreRank = 2;
	} else if (score < 100000) {
		scoreRank = 3;
	} else if (score < 1000000) {
		scoreRank = 4;
	} else {
		scoreRank = 5;
	}

	// スコアランクスターの作成
	auto rankStar = Sprite::create("rank_star.png", Rect(0, STAR_HEIGHT * scoreRank, STAR_WIDTH, STAR_HEIGHT));
	auto resultBackground = this->getResultBackground();
	auto backSize = resultBackground->getContentSize();
	rankStar->setPosition(Vec2(backSize.width / 2, STAR_POS_Y));
	resultBackground->addChild(rankStar);
}


void ResultLayer::update(float dt)
{

}
