/*
 * NPanel.h
 *
 *  Created on: 2015/10/01
 *      Author: 和仁
 */

#ifndef NPANEL_H_
#define NPANEL_H_

#include "cocos2d.h"

class NPanel :public cocos2d::Sprite
{
protected:
	NPanel();
	virtual ~NPanel();
	bool init() override;

public:
	CC_SYNTHESIZE(int, _num, Num);
	CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _numLabel, NumLabel);
	void update(float dt) override;
	CREATE_FUNC(NPanel);
};

#endif /* NPANEL_H_ */
