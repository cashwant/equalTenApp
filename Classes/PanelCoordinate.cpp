/*
 * PanelCoordinate.cpp
 *
 *  Created on: 2015/10/02
 *      Author: 和仁
 */

#include "PanelCoordinate.h"

USING_NS_CC;

// パネル座標の幅と高さ
const int COORDINATE_SIZE_WIDTH = 50;
const int COORDINATE_SIZE_HEIGHT = 50;
// 最下部のパネル座標の位置
const int COORDINATE_OFFSET = 35;

PanelCoordinate::PanelCoordinate()
: _longitude(0)
, _latitude(0)
, _nPanel(nullptr)
{
	// TODO 自動生成されたコンストラクター・スタブ

}

PanelCoordinate::~PanelCoordinate() {
	// TODO Auto-generated destructor stub
}

bool PanelCoordinate::initWithCoordinate(int longitude, int latitude)
{
	if (!Sprite::initWithFile("panel_coordinate.png")) {
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();
	auto CoordinateSize = this->getContentSize();

	// 一番左下のパネル座標の絶対座標
	auto originPos = Vec2(winSize.width / 2 - COORDINATE_SIZE_WIDTH / 2 * (PANEL_IND_WIDTH - 1),
			COORDINATE_OFFSET + COORDINATE_SIZE_HEIGHT / 2);
	// 各パネル座標の絶対座標
	auto indPos = Vec2(originPos.x + COORDINATE_SIZE_WIDTH * longitude
			, originPos.y + COORDINATE_SIZE_HEIGHT * latitude);
	this->setIndPos(indPos);
	this->setPosition(_indPos);

	this->scheduleUpdate();

	return true;
}

PanelCoordinate* PanelCoordinate::createWithCoodinate(int longitude, int latitude)
{
	// コンストラクタを作成
	PanelCoordinate *ret = new PanelCoordinate();
	if (ret->initWithCoordinate(longitude, latitude))
	{// 初期化がうまくいけば
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void PanelCoordinate::update(float dt)
{

}

