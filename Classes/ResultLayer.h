/*
 * ResultLayer.h
 *
 *  Created on: 2015/10/13
 *      Author: 和仁
 */

#ifndef RESULTLAYER_H_
#define RESULTLAYER_H_

#include "cocos2d.h"
#include "GameScene.h"

class ResultLayer :public cocos2d::Layer
{
protected:
	ResultLayer();
	virtual ~ResultLayer();
	bool init() override;

public:
	static cocos2d::Layer* createLayer();
	/// スコア表示用のラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _resultScoreLabel, ResultScoreLabel);
	// パネル枚数表示用のラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _resultPanelsLabel, ResultPanelsLabel);
	// コンボ数表示用のラベル
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _resultMaxComboLabel, ResultMaxComboLabel);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _resultBackground, ResultBackground);
	void update(float dt) override;
	// 結果ランクの作成メソッド
	void createStar(int score);
	CREATE_FUNC(ResultLayer);
};

#endif /* RESULTLAYER_H_ */
