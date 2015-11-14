/*
 * GameScene.cpp
 *
 *  Created on: 2015/10/01
 *      Author: 和仁
 */

#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

// ゲームのプレイ時間
const int TIME_LIMIT_SECOND = 60;

// 残り時間ヘッダ位置
const int TIME_HEADER_POS_X = 80;
const int TIME_HEADER_POS_Y = 380;

// 残り時間ラベル位置
const int TIME_LABEL_POS_X = 80;
const int TIME_LABEL_POS_Y = 370;

// 獲得スコアヘッダ位置
const int SCORE_HEADER_POS_X = 215;
const int SCORE_HEADER_POS_Y = 380;

// 獲得スコアラベル位置
const int SCORE_LABEL_POS_X = 215;
const int SCORE_LABEL_POS_Y = 370;

// ハイスコアヘッダ位置
const int BEST_HEADER_POS_X = 215;
const int BEST_HEADER_POS_Y = 445;

// ハイスコアラベル位置
const int BEST_LABEL_POS_X = 215;
const int BEST_LABEL_POS_Y = 435;

// 中断ボタンの位置
const int STOP_BUTTON_POS_X = 75;
const int STOP_BUTTON_POS_Y = 450;

// コンボ計上のタイムリミット
const float COMBO_TIME_LIMIT = 3;

// コンボのスコアへの係数(獲得スコア＝獲得ポイント×COMBO_INDEX^コンボ数)
const float INIT_COMBO_INDEX = 1.2;

// 中断時の画面設定
// 中断画面の位置
const int STOP_LAYER_POS_Y = 185;

// 継続ボタンとリプレイボタンの間隔
const int MENU_INTERVAL = 20;

// ホームボタンの位置
const int HOME_BUTTON_POS_Y = 35;


GameScene::GameScene()
: _nPanel(nullptr)
, _choosingTID(INT_MAX)
, _lastChosen(nullptr)
, _choosingSum(0)
, _firstNum(0)
, _panelCount(0)
, _timeLabel(NULL)
, _second(TIME_LIMIT_SECOND)
, _scoreLabel(NULL)
, _score(0)
, _point(1)
//, _comboFlag(false)
, _comboTime(0)
//, _comboIndex(1)
, _comboCount(0)
, _comboLabel(NULL)
, _chooseLabel(NULL)
, _state(GameState::READY)
, _maxCombo(0)
, _stopButton(NULL)
, _stopLayer(NULL)
//, _lastPos(Vec2::ZERO)
, _pointLabel(NULL)
{
	// TODO 自動生成されたコンストラクター・スタブ
}

GameScene::~GameScene() {
	CC_SAFE_RELEASE_NULL(_nPanel);
	// TODO Auto-generated destructor stub
}
Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	// オブジェクトを配置する
	this->itemArrange();

	// パネルとパネル座標を配置する
	this->panelArrange();

	// タッチの取得
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = [this](std::vector<Touch*> touches, Event *event) {
		for (auto touch : touches) {
			if (_state == GameState::PLAYING) {// ゲーム状態がプレイ中のとき
				for (int i = 0; i < PANEL_IND_WIDTH; i++) {
					for (int j = 0; j < PANEL_IND_HEIGHT; j++) {
						auto nPanel = coordinateArray[i][j]->getNPanel();
						if (nPanel->getBoundingBox().containsPoint(touch->getLocation())) {// 任意のパネルに触れていれば
							if (_chosenNPanels.size()==0){// 最初の選択であれば
								// 触れているパネルを選択する処理をする
								this->choosePanels(nPanel);
								// タッチIDを設定する
								this->setChoosingTID(touch->getID());
							}
						}
					}
				}
			}
		}
		return true;
	};
	listener->onTouchesEnded = [this](std::vector<Touch*> touches, Event *event) {
		for (auto touch : touches) {
			if (touch->getID() == this->getChoosingTID()){// 最初に触れたタッチIDと一致すれば
				this->releasePanels();
			}
		}
	};
	listener->onTouchesCancelled = [this](std::vector<Touch*> touches, Event *event) {
		for (auto touch : touches) {
			if (touch->getID() == this->getChoosingTID()){// 最初に触れたタッチIDと一致すれば
				this->releasePanels();
			}
		}
	};
	listener->onTouchesMoved = [this](std::vector<Touch*> touches, Event *event) {
		for (auto touch : touches) {
			if (_choosingTID == touch->getID()) {// 最初に触れたタッチIDと一致すれば
				for (int i = 0; i < PANEL_IND_WIDTH; i++) {
					for (int j = 0; j < PANEL_IND_HEIGHT; j++) {
						auto nPanel = coordinateArray[i][j]->getNPanel();
						if (nPanel->getBoundingBox().containsPoint(touch->getLocation())) {
							if (_lastChosen->getNextCoordinate().contains(coordinateArray[i][j])) {// 最後に選択されたパネルの隣であれば
								if (!_chosenNPanels.contains(nPanel)) {// すでにパネルが選択されていなければ
									this->choosePanels(nPanel);
								}
							}
						}
					}
				}
				if (_choosingSum == 10) {// 選択中合計が10になれば
					this->gainPanels();
					return true;
				} else if (_choosingSum > 10) {// 選択中合計が10を超えれば
					this->cancelPanels();
					return true;
				} else {
					return true;
				}
			}
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleUpdate();

	return true;

}


void GameScene::update(float dt)
{
	auto winSize = Director::getInstance()->getWinSize();
	if (_state == GameState::PLAYING) {// プレイ中の時
		// 残り秒数を減らす
		_second -= dt;
		// 残り秒数の表示を更新する
		int second = static_cast<int>(_second); // int型にキャストする
		_timeLabel->setString(StringUtils::toString(second));

		// コンボの初期化判断
		// コンボ秒数を増やす
		_comboTime += dt;
		if (_comboTime > COMBO_TIME_LIMIT) {
			this->setComboCount(0);
			//_comboLabel->setString(StringUtils::toString(_comboCount) + " combo");
		}
	}

	if (_second < 0) {
		this->onTimeup();
	}
}

void GameScene::nextCoordinate()
{
	PanelCoordinate* left = nullptr;
	PanelCoordinate* right = nullptr;
	PanelCoordinate* bottom = nullptr;
	PanelCoordinate* top = nullptr;
	for (int i = 0; i < PANEL_IND_WIDTH; i++) {
		for (int j = 0; j < PANEL_IND_HEIGHT; j++) {
			if (i == 0) {
				left = nullptr;
				right = coordinateArray[i+1][j];
			} else if (i == PANEL_IND_WIDTH - 1) {
				left = coordinateArray[i-1][j];
				right = nullptr;
			} else {
				left = coordinateArray[i-1][j];
				right = coordinateArray[i+1][j];
			}
			if (j == 0) {
				bottom = nullptr;
				top = coordinateArray[i][j+1];
			} else if (j == PANEL_IND_HEIGHT - 1) {
				bottom = coordinateArray[i][j-1];
				top = nullptr;
			} else {
				bottom = coordinateArray[i][j-1];
				top = coordinateArray[i][j+1];
			}
			auto nextCoordinate = coordinateArray[i][j]->getNextCoordinate();
			if (left != nullptr) {
				nextCoordinate.pushBack(left);
			}
			if (right != nullptr) {
				nextCoordinate.pushBack(right);
			}
			if (bottom != nullptr) {
				nextCoordinate.pushBack(bottom);
			}
			if (top != nullptr) {
				nextCoordinate.pushBack(top);
			}
			coordinateArray[i][j]->setNextCoordinate(nextCoordinate);
		}
	}
}

void GameScene::choosePanels(NPanel* nPanel)
{// パネルが選択された時の処理

	if (_chosenNPanels.size() == 0) {// 最初に選択されたパネルであれば
		// 初期選択数字に設定
		auto firstPos = nPanel->getPosition();
		this->setFirstPos(firstPos);
		this->addChild(_chooseLabel, 5);
	}

	// 選択されたパネルを半透明にする
	nPanel->setOpacity(50);

	// パネル選択群に追加する
	_chosenNPanels.pushBack(nPanel);

	// 最終選択座標に設定する
	this->setLastChosen(this->getCoordinate(nPanel));

	// 選択合計に数値を追加する
	auto sum = this->getChoosingSum();
	sum += nPanel->getNum();
	this->setChoosingSum(sum);

	// 選択中パネルの右上に選択合計を表示する
	auto panelPos = nPanel->getPosition();
	_chooseLabel->setString(StringUtils::toString(_choosingSum));
	_chooseLabel->setPosition(Vec2(panelPos.x + 40, panelPos.y + 40));
	_chooseLabel->setScale(3.0);
	_chooseLabel->runAction(ScaleTo::create(0.2, 1, 1));

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("touchpanel_kaka.mp3");
}

void GameScene::releasePanels()
{// パネル選択セッションが終了した時の処理
	for (NPanel* nPanel: _chosenNPanels) {
		nPanel->setOpacity(255);
	}
	this->initStates();

	// コンボ数を０にする
	this->setComboCount(0);
}

void GameScene::gainPanels()
{// 合計が10になった時にパネルの点数を獲得する処理
	for (NPanel* nPanel :  _chosenNPanels) {
		auto blink = Blink::create(0.3, 10);
		auto opacity = CallFuncN::create([this](Node* node){
			node->setOpacity(0);
		});
		auto sequence = Sequence::create(blink, opacity, NULL);
		nPanel->runAction(sequence);

		// パーティクルの作成
		auto particle = ParticleSystemQuad::create("particle_texture_rect.plist");
		this->addChild(particle, 4);
		particle->setPosition(nPanel->getPosition());

		// スコアの計算
		auto num = nPanel->getNum();
		auto point = this->getPoint();
		point = point * num;
		this->setPoint(point);

		// 最後のパネルの位置を取得
		this->setLastPos(nPanel->getPosition());

		// パネル獲得枚数を更新する
		auto panelCount = this->getPanelCount();
		panelCount++;
		this->setPanelCount(panelCount);
	}
	if (_comboCount < 4) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect.mp3");
	} else if (_comboCount < 9){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect2.mp3");
	} else if (_comboCount < 14) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect3.mp3");
	} else {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect4.mp3");
	}

	// コンボの計上
	auto comboCount = this->getComboCount();
	comboCount++;
	this->setComboCount(comboCount);
	// 経過時間の初期化
	this->setComboTime(0);
	if (_maxCombo < comboCount) {// 最大コンボ数を超えていれば
		this->setMaxCombo(comboCount);
	}

	// コンボ系数の計算
	auto comboIndex = pow(INIT_COMBO_INDEX, comboCount - 1);

	// コンボ数ラベルの作成
	auto comboLabel = Label::createWithSystemFont("0", "Latha", 24);
	comboLabel->setColor(Color3B::BLUE);
	comboLabel->enableShadow(Color4B::WHITE, Size(1, 1), 3);
	comboLabel->enableOutline(Color4B::WHITE, 1.5);
	comboLabel->setOpacity(150);
	this->setComboLabel(comboLabel);

	// コンボラベルの出現
	comboLabel->setString(StringUtils::toString(comboCount)+" combo");
	comboLabel->setPosition(Vec2(_lastPos.x + 40, _lastPos.y + 50));
	auto comboMoveby = MoveBy::create(0.5, Vec2(0, 20));
	auto comboRemove = RemoveSelf::create();
	comboLabel->runAction(Sequence::create(comboMoveby, comboRemove, NULL));
	this->addChild(comboLabel, 6);

	// ポイントラベルの作成
	auto pointLabel = Label::createWithSystemFont("0", "Latha", 24);
	pointLabel->setColor(Color3B::BLUE);
	pointLabel->enableShadow(Color4B::WHITE, Size(1, 1), 3);
	pointLabel->enableOutline(Color4B::WHITE, 1.5);
	pointLabel->setOpacity(100);
	this->setPointLabel(pointLabel);

	// ポイントラベルの出現
	pointLabel->setString(StringUtils::toString(static_cast<int>(_point * comboIndex)));
	pointLabel->setPosition(Vec2(_lastPos.x + 40, _lastPos.y + 30));
	auto pointMoveby = MoveBy::create(0.5, Vec2(0, 20));
	auto pointRemove = RemoveSelf::create();
	pointLabel->runAction(Sequence::create(pointMoveby, pointRemove, NULL));
	this->addChild(pointLabel, 6);

	/**
	// 獲得ポイントの計算
	auto firstNum = this->getFirstNum();
	auto sheets = _chosenNPanels.size();
	auto getPoint = pow(firstNum + 1, sheets);
	 **/

	// スコア計上
	auto score = this->getScore();
	score += _point * comboIndex;
	this->setScore(score);
	_scoreLabel->setString(StringUtils::toString(_score));

	// 消えた部分へのパネルの追加
	this->replaceCoordinate(_chosenNPanels);
	this->initStates();
}

void GameScene::cancelPanels()
{// 合計が10を超えてしまった時にパネルを開放する処理
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("misschoose.mp3");
	for (NPanel* nPanel : _chosenNPanels) {
		auto stay = CallFunc::create([this]{
			this->setState(GameState::READY);
		});
		auto blink = Blink::create(0.3, 10);
		auto opacity = CallFuncN::create([this](Node* node){
			node->setOpacity(255);
		});
		auto release = CallFunc::create([this]{
			this->setState(GameState::PLAYING);
		});
		auto sequence = Sequence::create(stay, blink, opacity, release, NULL);
		nPanel->runAction(sequence);
	}
	this->initStates();
	// コンボ数を０にする
	this->setComboCount(0);
}



void GameScene::initStates()
{
	// 選択中パネル群をクリアする
	_chosenNPanels.clear();
	if (_lastChosen != nullptr) {// 最終選択パネルがあれば(最初の配置時はパネルが選択されていないため)
		// 初期化する
		this->setLastChosen(nullptr);
	}
	// パネルを選択しているタッチIDを初期化する
	this->setChoosingTID(INT_MAX);

	// 選択中合計を初期化する
	this->setChoosingSum(0);

	this->setFirstNum(0);

	// 獲得ポイントを初期化する
	this->setPoint(1);

	// パネルの位置を再構成する
	for(int i = 0; i < PANEL_IND_WIDTH; i++) {
		for (int j = 0; j < PANEL_IND_HEIGHT; j++) {
			auto panelCoordinate = coordinateArray[i][j];
			auto nPanel = panelCoordinate->getNPanel();
			if (panelCoordinate->getIndPos() != nPanel->getPosition()){// 紐付けられたパネル座標の位置にパネルがなければ
				if (_state == GameState::PLAYING) {// ゲーム状態がプレイ中であれば
					// パネルの移動中のタッチを防ぐため、移動前後でゲーム状態を切り替える
					nPanel->runAction(Sequence::create(
							CallFunc::create([this]{
						// ゲーム状態を待機状態にする
						this->setState(GameState::READY);
					}),
					MoveTo::create(0.2, panelCoordinate->getIndPos()),
					CallFunc::create([this]{
						// ゲーム状態をプレイ中にする
						this->setState(GameState::PLAYING);
					}),
					NULL
					));
				} else if (_state == GameState::READY) {// ゲーム状態が待機状態であれば
					nPanel->runAction(MoveTo::create(0.5, panelCoordinate->getIndPos()));
				}
			}
		}
	}
	// 選択中合計ラベルを削除する
	_chooseLabel->removeFromParent();
}

void GameScene::replaceCoordinate(Vector<NPanel*> chosenNPanels)
{
	// 列ごとのパネル消失数のベクトル
	int count[PANEL_IND_WIDTH] = {0};
	for (auto nPanel : chosenNPanels) {
		// 選択されているパネルのパネル座標を取得
		auto coordinate = this->getCoordinate(nPanel);
		// パネル座標からパネルのひも付けを削除する
		coordinate->setNPanel(nullptr);
		// パネルを削除する
		nPanel->removeFromParent();
		// 列番号を取得
		auto latitude = coordinate->getLatitude();
		// 行番号を取得
		auto longitude = coordinate->getLongitude();
		for (int j = latitude; j < PANEL_IND_HEIGHT; j++) {// 消失パネルより上に位置するパネルに対して
			if (j != PANEL_IND_HEIGHT - 1) {// 最上部のパネル以外
				// ひとつ上のパネルを取得して、パネル座標を紐付ける
				auto abovePanel = coordinateArray[longitude][j+1]->getNPanel();
				coordinateArray[longitude][j]->setNPanel(abovePanel);
			} else {// 最上部のパネル
				// 消失パネル枚数を計上する
				count[longitude]++;
				// 新たなパネルを作成し、同列の待機領域に積む
				auto newNPanel = NPanel::create();
				newNPanel->setPosition(coordinateArray[longitude][j+count[longitude]]->getIndPos());
				// パネル座標最上部に紐付ける
				coordinateArray[longitude][j]->setNPanel(newNPanel);
				this->addChild(newNPanel, 2);
			}
		}
	}
}


PanelCoordinate* GameScene::getCoordinate(NPanel* nPanel)
{
	for (int i = 0; i < PANEL_IND_WIDTH; i++) {
		for (int j = 0; j < PANEL_IND_HEIGHT; j++) {
			if (coordinateArray[i][j]->getNPanel() == nPanel) {
				return coordinateArray[i][j];
			}
		}
	}
	return nullptr;
}


void GameScene::addReadyLabel()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto center = Vec2(winSize.width / 2.0, winSize.height / 2.0);

	// カウントダウン数字を作成する
	auto one = Label::createWithSystemFont("1", "Latha", 60);
	one->enableShadow(Color4B::GREEN, Size(2, 2), 3);
	one->enableOutline(Color4B::GREEN, 1.5);
	one->setColor(Color3B::WHITE);
	one->setPosition(Vec2(center.x, center.y + 30));

	auto two = Label::createWithSystemFont("2", "Latha", 60);
	two->enableShadow(Color4B::YELLOW, Size(2, 2), 3);
	two->enableOutline(Color4B::YELLOW, 1.5);
	two->setColor(Color3B::WHITE);
	two->setPosition(Vec2(center.x, center.y + 30));

	auto three = Label::createWithSystemFont("3", "Latha", 60);
	three->enableShadow(Color4B::RED, Size(2, 2), 3);
	three->enableOutline(Color4B::RED, 1.5);
	three->setColor(Color3B::WHITE);
	three->setPosition(Vec2(center.x, center.y + 30));

	// STARTの文字を定義する
	auto start = Sprite::create("start.png");
	start->runAction(Sequence::create(
			CCSpawn::create(EaseIn::create(ScaleTo::create(0.5, 3.0), 0.5), FadeOut::create(0.5), NULL),
			RemoveSelf::create(), // 自分を削除する
			CallFunc::create([this]{
		_state = GameState::PLAYING; // ゲーム状態をPLAYINGに切り替える
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("gameplaying.mp3", true);
	}),
	NULL));
	start->setPosition(center);
	one->runAction(Sequence::create(
			MoveTo::create(0.1, center),
			FadeOut::create(0.9), // 1.0秒待つ
			CallFunc::create([this, start]{// ラムダのなかでthisとstart変数を使っているのでキャプチャに加える
		this->addChild(start, 3); // 「スタート」のラベルを追加する（この時点でスタートのアニメーションが始まる
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("start.mp3");
	}),
	CallFunc::create([this]{
		this->initStates();
	}),
	RemoveSelf::create(),
	NULL)
	);

	two->runAction(Sequence::create(
			MoveTo::create(0.1, center),
			FadeOut::create(0.9),
			CallFunc::create([this, one]{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("countdown.mp3");
		this->addChild(one, 3);
	}),
	RemoveSelf::create(),
	NULL)
	);

	three->runAction(Sequence::create(
			MoveTo::create(0.1, center),
			FadeOut::create(0.9),
			CallFunc::create([this, two]{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("countdown.mp3");
				this->addChild(two, 3);
			}),
			RemoveSelf::create(),
			NULL)
	);
	this->runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create([this, three]{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("countdown.mp3");
				this->addChild(three, 3);
			}),
			NULL)
	);

}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->addReadyLabel();
}


void GameScene::onTimeup()
{
	if (_state == GameState::PLAYING) {
		auto winSize = Director::getInstance()->getWinSize();

		// タイムアップラベルの作成
		auto timeupLabel = Sprite::create("timeup.png");
		timeupLabel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		timeupLabel->setScale(0);
		this->addChild(timeupLabel, 7);
		auto appear = EaseExponentialIn::create(ScaleTo::create(0.2, 1.3));
		auto whistle = CallFunc::create([]{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("timeup_whistle.mp3");
				CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		});
		auto reduce = ScaleTo::create(0.1, 0.9);
		auto expanse = ScaleTo::create(0.1, 1.1);
		auto normal = ScaleTo::create(0.1, 1);
		auto delay = DelayTime::create(0.5);
		auto music = CallFunc::create([]{
				CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("opening.mp3", true);
		});
		auto sequence = Sequence::create(appear, whistle, reduce, expanse, normal, delay, music, NULL);
		timeupLabel->runAction(sequence);

		// 結果画面の作成
		auto resultLayer = ResultLayer::create();
		this->addChild(resultLayer, 8);

		// スコアのセット
		auto resultScore = resultLayer->getResultScoreLabel();
		resultScore->setString(StringUtils::toString(_score));
		resultLayer->setResultScoreLabel(resultScore);

		// 獲得枚数のセット
		auto resultPanels = resultLayer->getResultPanelsLabel();
		resultPanels->setString(StringUtils::toString(_panelCount));
		resultLayer->setResultPanelsLabel(resultPanels);

		// 最大コンボ数のセット
		auto resultMaxCombo = resultLayer->getResultMaxComboLabel();
		resultMaxCombo->setString(StringUtils::toString(_maxCombo));
		resultLayer->setResultMaxComboLabel(resultMaxCombo);

		// ランクスターの作成
		resultLayer->createStar(_score);


		// スコアランキングの更新
		this->scoreRankUpdate();
	}
	// リザルト状態移行
	_state = GameState::RESULT;
	this->releasePanels();
}

void GameScene::stopGame()
{
	if ( _state == GameState::PLAYING ) {
		this->setState(GameState::PAUSE);
		this->addChild(_stopLayer, 5);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
	}
}

void GameScene::itemArrange()
{
	auto winSize = Director::getInstance()->getWinSize();

	// バックグラウンドの作成
	auto background = Sprite::create("background.png");
	this->addChild(background, 3);
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	// タイムヘッダの作成
	auto timeHeader = Sprite::create("time_header.png");
	this->addChild(timeHeader, 4);
	timeHeader->setPosition(Vec2(TIME_HEADER_POS_X, TIME_HEADER_POS_Y));

	// タイムラベルの作成
	int second = static_cast<int>(_second);
	auto timeLabel = Label::createWithSystemFont(StringUtils::toString(second), "Latha", 20);
	timeLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	timeLabel->enableOutline(Color4B::BLACK, 1.5);
	this->addChild(timeLabel, 5);
	this->setTimeLabel(timeLabel);
	timeLabel->setPosition(Vec2(TIME_LABEL_POS_X, TIME_LABEL_POS_Y));

	// ベストスコアヘッダの作成
	auto bestHeader = Sprite::create("best_header.png");
	this->addChild(bestHeader, 4);
	bestHeader->setPosition(Vec2(BEST_HEADER_POS_X, BEST_HEADER_POS_Y));

	// ベストスコアラベルの作成
	UserDefault* _userDef = UserDefault::getInstance();
	auto bestScore = _userDef->getIntegerForKey("Score0");
	auto bestLabel = Label::createWithSystemFont(StringUtils::toString(bestScore), "Latha", 20);
	bestLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	bestLabel->enableOutline(Color4B::BLACK, 1.5);
	this->addChild(bestLabel, 5);
	bestLabel->setPosition(Vec2(BEST_LABEL_POS_X, BEST_LABEL_POS_Y));

	// スコアヘッダの作成
	auto scoreHeader = Sprite::create("score_header.png");
	this->addChild(scoreHeader, 4);
	scoreHeader->setPosition(Vec2(SCORE_HEADER_POS_X, SCORE_HEADER_POS_Y));

	// スコアラベルの作成
	auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score), "Latha", 20);
	scoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
	scoreLabel->enableOutline(Color4B::BLACK, 1.5);
	this->addChild(scoreLabel, 5);
	this->setScoreLabel(scoreLabel);
	scoreLabel->setPosition(Vec2(SCORE_LABEL_POS_X, SCORE_LABEL_POS_Y));

	// ストップレイヤーの作成
	auto stopLayer = Sprite::create("stop_background.png");
	stopLayer->setPosition(Vec2(winSize.width / 2, STOP_LAYER_POS_Y));
	auto stopSize = stopLayer->getContentSize();
	this->setStopLayer(stopLayer);

	// ストップボタンの作成
	auto stopButton = MenuItemImage::create("stop_button.png", "stop_button_pressed.png",
			[this](Ref* ref){
		if ( _state == GameState::PLAYING) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
		}
		this->stopGame();
	});

	// ストップメニューの作成
	auto stopMenu = Menu::create(stopButton, NULL);
	this->addChild(stopMenu, 4);
	stopMenu->setPosition(Vec2(STOP_BUTTON_POS_X, STOP_BUTTON_POS_Y));

	//リプレイボタンの作成
	auto replayButton = MenuItemImage::create("replay_button.png", "replay_button_pressed.png",
			[](Ref* ref){
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
	});

	// 継続ボタンの作成
	auto keepButton = MenuItemImage::create("keepgoing.png", "keepgoing_pressed.png",
			[this](Ref* ref){
		_stopLayer->runAction(RemoveSelf::create(true));
		this->setState(GameState::PLAYING);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	});

	// ボタンメニューの作成
	auto buttonMenu = Menu::create(keepButton, replayButton, NULL);
	stopLayer->addChild(buttonMenu);
	buttonMenu->setPosition(Vec2(stopSize.width / 2, stopSize.height / 2));
	buttonMenu->alignItemsVerticallyWithPadding(MENU_INTERVAL);

	// ホームボタンの作成
	auto homeButton = MenuItemImage::create("home_button_stop.png", "home_button_stop_pressed.png",
			[](Ref* ref){
		auto scene = StartScene::createScene();
		auto transition = TransitionPageTurn::create(0.5, scene, true);
		Director::getInstance()->replaceScene(transition);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonpush.mp3");
	});

	// ホームボタンメニューの作成
	auto homeMenu = Menu::create(homeButton, NULL);
	stopLayer->addChild(homeMenu);
	homeMenu->setPosition(Vec2(stopSize.width / 2, HOME_BUTTON_POS_Y));

	// 選択中合計ラベルの作成
	auto chooseLabel = Label::createWithSystemFont(StringUtils::toString(_choosingSum), "Latha", 20);
	chooseLabel->enableShadow(Color4B::BLACK, Size(1, 1), 3);
	chooseLabel->enableOutline(Color4B::BLACK, 1.5);
	this->setChooseLabel(chooseLabel);

}

void GameScene::panelArrange()
{
	// パネル座標の作成
	for (int i = 0; i < PANEL_IND_WIDTH; i++) {
		for (int j = 0; j < 2 * PANEL_IND_HEIGHT; j++) {
			// パネル座標の作成
			auto panelCoordinate = PanelCoordinate::createWithCoodinate(i, j);
			// パネル座標の配列作成
			coordinateArray[i][j] = panelCoordinate;
			// パネルとパネル座標のひも付け
			panelCoordinate->setLongitude(i);
			panelCoordinate->setLatitude(j);

			this->addChild(panelCoordinate, 1);
		}
	}
	// パネルの配置
	for (int k = 0; k < PANEL_IND_WIDTH; k++) {
		for (int l = 0; l < PANEL_IND_HEIGHT; l++) {
			auto nPanel = NPanel::create();
			// 表示領域のパネル座標に紐付ける
			coordinateArray[k][l]->setNPanel(nPanel);
			// 待機領域のパネル座標の位置にパネルを配置する
			nPanel->setPosition(coordinateArray[k][l + PANEL_IND_HEIGHT]->getIndPos());
			this->addChild(nPanel, 2);
		}
	}

	// パネル座標の隣接関係を定義する
	this->nextCoordinate();
}

void GameScene::scoreRankUpdate()
{
	for (int i = 0; i < RANK_MAX; i++) {
		UserDefault* _userDef = UserDefault::getInstance();

		// 現在のハイスコアを取得
		auto scoreKey = "Score" + StringUtils::toString(i);
		auto charScoreKey = scoreKey.c_str();
		auto oldHighScore = _userDef->getIntegerForKey(charScoreKey);

		// 現在のパネル枚数ランクを取得
		auto panelsKey = "Panels" + StringUtils::toString(i);
		auto charPanelsKey = panelsKey.c_str();
		auto oldHighPanels = _userDef->getIntegerForKey(charPanelsKey);

		// 現在のコンボ数ランクを取得
		auto comboKey = "Combo" + StringUtils::toString(i);
		auto charComboKey = comboKey.c_str();
		auto oldHighCombo = _userDef->getIntegerForKey(charComboKey);

		auto newScore = this->getScore();
		auto newPanelCount = this->getPanelCount();
		auto newCombo = this->getMaxCombo();
		if (newScore > oldHighScore) {
			_userDef->setIntegerForKey(charScoreKey, newScore);
			_userDef->setIntegerForKey(charPanelsKey, newPanelCount);
			_userDef->setIntegerForKey(charComboKey, newCombo);
			for (int j = i + 1; j < RANK_MAX; j++) {
				newScore = oldHighScore;
				newPanelCount = oldHighPanels;
				newCombo = oldHighCombo;

				// ひとつ下のスコアを退避して、上のスコアを設定
				auto nextScoreKey = "Score" + StringUtils::toString(j);
				auto charNextScoreKey = nextScoreKey.c_str();
				oldHighScore = _userDef->getIntegerForKey(charNextScoreKey);
				_userDef->setIntegerForKey(charNextScoreKey, newScore);

				// ひとつ下のパネル獲得枚数を退避して、上の獲得枚数を設定
				auto nextPanelsKey = "Panels" + StringUtils::toString(j);
				auto charNextPanelsKey = nextPanelsKey.c_str();
				oldHighPanels = _userDef->getIntegerForKey(charNextPanelsKey);
				_userDef->setIntegerForKey(charNextPanelsKey, newPanelCount);

				// ひとつ下のコンボ数を退避して、上のコンボ数を設定
				auto nextComboKey = "Combo" + StringUtils::toString(j);
				auto charNextComboKey = nextComboKey.c_str();
				oldHighCombo = _userDef->getIntegerForKey(charNextComboKey);
				_userDef->setIntegerForKey(charNextComboKey, newCombo);
			}
			break;
		}
	}
}
