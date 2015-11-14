/*
 * GameScene.h
 *
 *  Created on: 2015/10/01
 *      Author: 和仁
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "NPanel.h"
#include "PanelCoordinate.h"
#include "ResultLayer.h"
#include "StartScene.h"


class GameScene :public cocos2d::Layer
{
protected:
	GameScene();
	virtual ~GameScene();
	bool init() override;

public:
	// ナンバーパネル
	CC_SYNTHESIZE_RETAIN(NPanel *, _nPanel, NPanel);

	// 現在選択しているパネル
	CC_SYNTHESIZE_RETAIN(PanelCoordinate *, _lastChosen, LastChosen);
	// パネルが配置される座標、高さ方向はパネル待機場所も含めて表示領域の２倍にしている
	PanelCoordinate* coordinateArray[PANEL_IND_WIDTH][2 * PANEL_IND_HEIGHT];
	// 選択パネル群
	CC_SYNTHESIZE(cocos2d::Vector<NPanel*>, _chosenNPanels, ChosenNPanels);

	// ゲーム状態
	enum class GameState
	{
		// プレイ前の待機状態、パネル移動中
		READY,
		// プレイ中
		PLAYING,
		// 中断中
		PAUSE,
		// 結果表示
		RESULT
	};
	// 現在のゲーム状態
	CC_SYNTHESIZE(GameState, _state, State);
	// パネルを選択しているタッチID
	CC_SYNTHESIZE(int, _choosingTID, ChoosingTID);
	// 選択しているパネルの数字合計
	CC_SYNTHESIZE(int, _choosingSum, ChoosingSum);
	// 最初に選択されたパネルの数字
	CC_SYNTHESIZE(int, _firstNum, FirstNum);
	// 選択されたパネル枚数
	CC_SYNTHESIZE(int, _panelCount, PanelCount);
	// タイム表示用のラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _timeLabel, TimeLabel);
	// 中断ボタン
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _stopButton, StopButton);
	// 中断画面
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _stopLayer, StopLayer);
	// 残り時間
	CC_SYNTHESIZE(float, _second, Second);
	// 最初に選択したパネルの位置
	CC_SYNTHESIZE(cocos2d::Vec2, _firstPos, FirstPos);
	// スコア表示用のラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
	// コンボ数表示用ラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _comboLabel, ComboLabel);
	// 選択合計表示用ラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _chooseLabel, ChooseLabel);
	// 獲得ポイント表示用ラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _pointLabel, PointLabel);
	// パネル獲得時に最後に選択されたパネルの位置
	CC_SYNTHESIZE(cocos2d::Vec2, _lastPos, LastPos);
	// 獲得中スコア
	CC_SYNTHESIZE(int, _score, Score);
	// 獲得ポイント
	CC_SYNTHESIZE(int, _point, Point);
	// 前回パネル獲得後からの経過時間
	CC_SYNTHESIZE(float, _comboTime, ComboTime);
	// コンボ数
	CC_SYNTHESIZE(int, _comboCount, ComboCount);
	// １プレイの最大コンボ数
	CC_SYNTHESIZE(int, _maxCombo, MaxCombo);

	static cocos2d::Scene* createScene();

	// 座標全体に隣の座標を紐付けるメソッド
	void nextCoordinate();

	// パネルを選択するメソッド
	void choosePanels(NPanel* nPanel);

	// 選択中パネルを開放するメソッド
	void releasePanels();

	// パネルに紐付けられた座標を抽出するメソッド
	PanelCoordinate* getCoordinate(NPanel* nPanel);

	// パネル選択状態の初期化（選択パネルの解放、変数の初期化）
	void initStates();

	// パネルを獲得するメソッド
	void gainPanels();

	// パネル選択をミスし、選択状態を初期化するメソッド
	void cancelPanels();

	// ゲーム開始直前のアニメーションを実装するメソッド
	void addReadyLabel();

	// 消えた部分にパネルを追加するメソッド
	void replaceCoordinate(cocos2d::Vector<NPanel*> chosenNPanels);

	// タイムアップ時の処理を実装するメソッド
	void onTimeup();

	// 中断時の処理を実装するメソッド
	void stopGame();

	// objectを配置するメソッド
	void itemArrange();

	// パネルとパネル座標を配置するメソッド
	void panelArrange();

	// スコアランキングを更新するメソッド
	void scoreRankUpdate();

	// シーン遷移直後の処理を実装するメソッド
	void onEnterTransitionDidFinish() override;
	void update(float dt) override;
	CREATE_FUNC(GameScene);
};

#endif /* GAMESCENE_H_ */
