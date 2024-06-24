//
//  RewardAnimationScene.cpp
//  OpalibTemplate-mobile
//
//  Created by Opala Studios on 26/08/21.
//


#include "RewardAnimationScene.hpp"
//#include "../Managers/FXManager.hpp"

USING_NS_CC;
using namespace fairygui;

OPPool::OPPool(const std::string& poolBaseName, int poolSize, fairygui::GComponent* owner)
{
	_pool.clear();
    for(int i = 0; i < poolSize; i++)
    {
        auto obj = owner->getChild(poolBaseName + std::to_string(i))->as<GComponent>();
        if(obj)
            _pool.push_back(obj);
    }
}

fairygui::GComponent* OPPool::getObject(int idx)
{
    if(idx < _pool.size())
        return _pool.at(idx);
    
    return nullptr;
}

OPPool::~OPPool(){
    _pool.clear();
}

cocos2d::Rect OPRect::getRect(const GObject& obj)
{
	cocos2d::Rect rect;
	rect.setRect(obj.getPosition().x, obj.getPosition().y,
				 obj.getSize().width, obj.getSize().height);
	return rect;
}

bool RewardAnimationScene::init()
{
    if (!OPBaseScene::init())
    {
        return false;
    }
	
//	FXManager::shared();
	
	_coinsPool = std::make_unique<OPPool>("coin", _coinsPoolSize, _view);
	_rStarsPool = std::make_unique<OPPool>("r-star", _starsPoolSize, _view);
	
	// MARK: COINS
	
	_coinsButton = _view->getChild("n1")->as<GButton>();
	_coinDest = _view->getChild("coin-dest");
	_coinsText = _view->getChild("numberCoins")->as<GTextField>();
	
	_coinsSpawner.setup(_coinsPoolSize, {OPRect::getRect(*_coinsButton)}, {OPRect::getRect(*_coinDest)});
	_coinsSpawner.setSpawnStyle(opalib::SpawnStyle::TO_LEFT, 200.0f);
	
	_coinsSpawner.setSpawnHandler([this](int index, cocos2d::Vec2 pos, float rotation, float alpha) {
		_coinsPool->getObject(index)->setPosition(pos.x, pos.y);
		_coinsPool->getObject(index)->setRotation(rotation);
		_coinsPool->getObject(index)->setVisible(true);
		
//		FXManager::shared()->play(FXManager::Effect::SPAWN_COIN);
	});
	
	_coinsSpawner.setUpdateHandler([this](int index, cocos2d::Vec2 pos, float rotation, float alpha) {
		_coinsPool->getObject(index)->setPosition(pos.x, pos.y);
		_coinsPool->getObject(index)->setRotation(rotation);
	});
	
	_coinsSpawner.setReachEndHandler([this](int index) {
		_coinsPool->getObject(index)->setVisible(false);
		_numberCoinsSpawned++;
		
		_coinsText->setText(std::to_string(_numberCoinsSpawned));
		
//		FXManager::shared()->play(FXManager::Effect::RECEIVE_COIN);
	});
	
	_coinsButton->addClickListener([this](const EventContext* ev) {
		_coinsSpawner.reset();
		_coinsSpawner.startSpawn(_coinsPoolSize, 2.0f,3.0f, 0.5f);
	});
	
	// MARK: STARS
	
	_starsButton = _view->getChild("n2")->as<GButton>();
	
	_starsPool = std::make_unique<OPPool>("star", _starsPoolSize, _view);
	
	_starsSpawner.setup(_starsPoolSize, {OPRect::getRect(*_starsButton)}, {
		OPRect::getRect(*_starsPool->getObject(0)),
		OPRect::getRect(*_starsPool->getObject(1)),
		OPRect::getRect(*_starsPool->getObject(2)),
		OPRect::getRect(*_starsPool->getObject(3)),
		OPRect::getRect(*_starsPool->getObject(4))
	});
	_starsSpawner.setDestinationIndexStyle(opalib::IndexStyle::FORWARD);
	_starsSpawner.setSpawnStyle(opalib::SpawnStyle::TO_TOP, 100.0f);
	
	_starsSpawner.setSpawnHandler([this](int index, cocos2d::Vec2 pos, float rotation, float alpha) {
		_rStarsPool->getObject(index)->setPosition(pos.x, pos.y);
		_rStarsPool->getObject(index)->setRotation(rotation);
		_rStarsPool->getObject(index)->setVisible(true);
		
//		FXManager::shared()->play(FXManager::Effect::SPAWN_STAR);
	});
	
	_starsSpawner.setUpdateHandler([this](int index, cocos2d::Vec2 pos, float rotation, float alpha) {
		_rStarsPool->getObject(index)->setPosition(pos.x, pos.y);
		_rStarsPool->getObject(index)->setRotation(rotation);
	});
	
	_starsSpawner.setReachEndHandler([this](int index) {
		_rStarsPool->getObject(index)->setVisible(false);
		
		_starsPool->getObject(index)->getController("color")->setSelectedIndex(1);
		
//		FXManager::shared()->play(FXManager::Effect::RECEIVE_STAR);
	});
	
	_starsButton->addClickListener([this](const EventContext* ev) {
		_starsSpawner.reset();
		
		for(int i = 0; i < _starsPoolSize; i++)
		{
			_starsPool->getObject(i)->getController("color")->setSelectedIndex(0);
		}
		
		_starsSpawner.startSpawn(_starsPoolSize, 0.5f,1.0f, 0.5f);
	});
	
	scheduleUpdate();

    return true;
}

void RewardAnimationScene::update(float deltaTime) {
	_coinsSpawner.update(deltaTime);
	_starsSpawner.update(deltaTime);
}
