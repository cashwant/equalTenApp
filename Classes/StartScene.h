/*
 * StartScene.h
 *
 *  Created on: 2015/10/11
 *      Author: 和仁
 */

#ifndef STARTSCENE_H_
#define STARTSCENE_H_

#include "cocos2d.h"
#include "RankScene.h"
#include "RuleScene.h"

class StartScene :public cocos2d::Layer
{
protected:
	StartScene();
	virtual ~StartScene();
	bool init() override;
public:
	CC_SYNTHESIZE(bool, _atmain, Atmain);
	CC_SYNTHESIZE_RETAIN(RuleScene*, _ruleScene, RuleScene);
	CC_SYNTHESIZE_RETAIN(RankScene*, _rankScene, RankScene);

	void itemArrange();
	static cocos2d::Scene* createScene();
	void update(float dt) override;
	void onEnterTransitionDidFinish() override;
	CREATE_FUNC(StartScene);
};

#endif /* STARTSCENE_H_ */
