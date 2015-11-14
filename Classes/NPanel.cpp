/*
 * NPanel.cpp
 *
 *  Created on: 2015/10/01
 *      Author: 和仁
 */

#include "NPanel.h"
#include "GameScene.h"

USING_NS_CC;

// パネル数字の最大値
const int NUM_MAX = 5;

NPanel::NPanel()
: _numLabel(NULL)
, _num(0)
{
	// TODO 自動生成されたコンストラクター・スタブ

}

NPanel::~NPanel() {
	// TODO Auto-generated destructor stub
	CC_SAFE_RELEASE_NULL(_numLabel);
}

bool NPanel::init()
{
	if (!Sprite::initWithFile("NPanel.png")){
			return false;
		}

	auto winSize = Director::getInstance()->getWinSize();

	auto frameSize = Size(this->getContentSize().width / NUM_MAX, this->getContentSize().height);

	// パネル用の数字生成
	auto num = rand() % NUM_MAX + 1;
	this->setNum(num);

	// 数字ごとに切り取る画像を設定
	this->setTextureRect(Rect(frameSize.width * (_num - 1), 0, frameSize.width, frameSize.height));

	// 透明度を同期する
	this->setCascadeOpacityEnabled(true);

	return true;
}

void NPanel::update(float dt)
{

}
/**
void GameScene::panelPack()
{
	for (i = 0; i < PANEL_IND_WIDTH; i++) {

		_nPanelVector.pushBack(nPanel);

	}
}
**/
