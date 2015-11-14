/*
 * RuleScene.h
 *
 *  Created on: 2015/10/19
 *      Author: 和仁
 */

#ifndef RULESCENE_H_
#define RULESCENE_H_

#include "cocos2d.h"

class RuleScene :public cocos2d::Layer
{
protected:
	RuleScene();
	virtual ~RuleScene();
	bool init() override;

public:
	CC_SYNTHESIZE(bool, _atmain, Atmain);
	static cocos2d::Scene* createScene();
	void update(float dt) override;
	CREATE_FUNC(RuleScene);
};

#endif /* RULESCENE_H_ */
