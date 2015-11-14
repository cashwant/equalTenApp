/*
 * RankScene.h
 *
 *  Created on: 2015/10/21
 *      Author: 和仁
 */

#ifndef RANKSCENE_H_
#define RANKSCENE_H_

#include "cocos2d.h"

const int RANK_MAX = 10;

class RankScene :public cocos2d::Layer
{
protected:
	RankScene();
	virtual ~RankScene();
	bool init() override;

public:
	// メイン画面フラグ
	CC_SYNTHESIZE(bool, _atmain, Atmain);
	static cocos2d::Scene* createScene();
	void update(float dt) override;
	CREATE_FUNC(RankScene);
};

#endif /* RANKSCENE_H_ */
