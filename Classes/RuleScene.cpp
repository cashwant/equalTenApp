/*
 * RuleScene.cpp
 *
 *  Created on: 2015/10/19
 *      Author: 和仁
 */

#include "RuleScene.h"
#include "StartScene.h"

USING_NS_CC;

const int HOME_BUTTON_POS_Y = 50;

RuleScene::RuleScene()
: _atmain(true)
{
	// TODO 自動生成されたコンストラクター・スタブ

}

RuleScene::~RuleScene() {
	// TODO Auto-generated destructor stub
}

Scene* RuleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RuleScene::create();
	scene->addChild(layer);
	return scene;
}

bool RuleScene::init()
{
	if(!Layer::init()){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();


	// ルール画面の生成
	auto ruleBackground = Sprite::create("rule_background.png");
	auto backSize = ruleBackground->getContentSize();
	ruleBackground->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(ruleBackground, 1);


	// ホームボタンの生成
	auto homeButton = MenuItemImage::create("home_button_rule.png", "home_button_rule_pressed.png",
			[this](Ref* ref){
		this->setAtmain(true);
		this->runAction(RemoveSelf::create());
	});

	auto menu = Menu::create(homeButton, NULL);
	ruleBackground->addChild(menu, 2);
	menu->setPosition(Vec2(backSize.width / 2, HOME_BUTTON_POS_Y));

	return true;
}

void RuleScene::update(float dt)
{

}
