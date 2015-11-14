/*
 * RankScene.cpp
 *
 *  Created on: 2015/10/21
 *      Author: 和仁
 */

#include "RankScene.h"
#include "GameScene.h"
#include "StartScene.h"

USING_NS_CC;

// ボタンの高さ位置
const int MENU_POS_Y = 50;

// ランク最上部のラベル位置
const float RANK_POS_INIT_Y = 352.5;
// ランクごとの表示間隔
const int RANK_POS_INTERVAL_Y = 26;

// スコアの横方向位置
const float RANK_SCORE_POS_X = 102.5;
// パネル枚数の横方向位置
const float RANK_PANELS_POS_X = 186.25;
// コンボ数の横方向位置
const float RANK_COMBO_POS_X = 258.75;

RankScene::RankScene()
: _atmain(true)
{
	// TODO 自動生成されたコンストラクター・スタブ
}

RankScene::~RankScene() {
	// TODO Auto-generated destructor stub
}

Scene* RankScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RankScene::create();
	scene->addChild(layer);
	return scene;
}

bool RankScene::init()
{
	if(!Layer::init()){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();


	// ランク画面の生成
	auto rankBackground = Sprite::create("rank_background.png");
	rankBackground->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(rankBackground, 1);

	// ホームボタンの生成
	auto homeButton = MenuItemImage::create("home_button_rank.png", "home_button_rank_pressed.png",
			[this](Ref* ref){
		// メイン画面フラグを立てる
		this->setAtmain(true);
		// ランク画面を削除する
		this->runAction(RemoveSelf::create());
	});

	// ホームボタンメニューの作成
	auto menu = Menu::create(homeButton, NULL);
	this->addChild(menu, 2);
	menu->setPosition(Vec2(winSize.width / 2, MENU_POS_Y));

	// 格納用配列の取得
	UserDefault* _userDef = UserDefault::getInstance();

	for (int i = 0; i < RANK_MAX; i++) {
		// ハイスコア一覧を取得
		auto scoreKey = "Score" + StringUtils::toString(i);
		auto charScoreKey = scoreKey.c_str();
		auto highScore = _userDef->getIntegerForKey(charScoreKey);

		// パネル枚数一覧を取得
		auto panelsKey = "Panels" + StringUtils::toString(i);
		auto charPanelsKey = panelsKey.c_str();
		auto highPanels = _userDef->getIntegerForKey(charPanelsKey);

		// コンボ数一覧を取得
		auto comboKey = "Combo" + StringUtils::toString(i);
		auto charComboKey = comboKey.c_str();
		auto highCombo = _userDef->getIntegerForKey(charComboKey);

		if (highScore != 0) {
			// スコアを表示する
			auto highScoreLabel = Label::createWithSystemFont(StringUtils::toString(highScore), "Latha", 18);
			highScoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
			highScoreLabel->enableOutline(Color4B::BLACK, 1.5);
			highScoreLabel->setPosition(RANK_SCORE_POS_X, RANK_POS_INIT_Y - RANK_POS_INTERVAL_Y * i);
			this->addChild(highScoreLabel, 3);

			// パネル枚数を表示する
			auto highPanelsLabel = Label::createWithSystemFont(StringUtils::toString(highPanels), "Latha", 18);
			highPanelsLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
			highPanelsLabel->enableOutline(Color4B::BLACK, 1.5);
			highPanelsLabel->setPosition(RANK_PANELS_POS_X, RANK_POS_INIT_Y - RANK_POS_INTERVAL_Y * i);
			this->addChild(highPanelsLabel, 3);

			// コンボ数を表示する
			auto highComboLabel = Label::createWithSystemFont(StringUtils::toString(highCombo), "Latha", 18);
			highComboLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
			highComboLabel->enableOutline(Color4B::BLACK, 1.5);
			highComboLabel->setPosition(RANK_COMBO_POS_X, RANK_POS_INIT_Y - RANK_POS_INTERVAL_Y * i);
			this->addChild(highComboLabel, 3);
		}
	}
	return true;
}

void RankScene::update(float dt)
{

}
