/*
 * PanelCoordinate.h
 *
 *  Created on: 2015/10/02
 *      Author: 和仁
 */

#ifndef PANELCOORDINATE_H_
#define PANELCOORDINATE_H_

#include "cocos2d.h"
#include "NPanel.h"

const int PANEL_IND_WIDTH = 5;
const int PANEL_IND_HEIGHT = 6;

class PanelCoordinate :public cocos2d::Sprite
{
protected:
	PanelCoordinate();
	virtual ~PanelCoordinate();
	bool initWithCoordinate(int longitude, int latitude);

public:
	void update(float dt) override;
	// パネル座標の列番号
	CC_SYNTHESIZE(int, _latitude, Latitude);
	// パネル座標の行番号
	CC_SYNTHESIZE(int, _longitude, Longitude);
	// パネル座標の絶対座標
	CC_SYNTHESIZE(cocos2d::Vec2, _indPos, IndPos);
	// 隣接するパネル座標群
	CC_SYNTHESIZE(cocos2d::Vector<PanelCoordinate *>, _nextCoordinate, NextCoordinate);
	// パネル
	CC_SYNTHESIZE_RETAIN(NPanel*, _nPanel, NPanel);
	// 行番号、列番号を指定して、パネル座標を作成するメソッド
	static PanelCoordinate* createWithCoodinate(int longitude, int latitude);
};

#endif /* PANELCOORDINATE_H_ */
