//
//  ComponentPool.hpp
//  opalib-ios
//
//  Created by Opala Studios on 27/08/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef ComponentPool_hpp
#define ComponentPool_hpp

#include <FairyGUI.h>
#include "../Scene/OPBaseScene.hpp"
#include "OPFairyUtils.hpp"

namespace opalib{

template<typename T>
class ComponentPool {
public:
	ComponentPool(const std::string& poolBaseName, int poolSize, fairygui::GComponent* owner) {
		createPool(poolBaseName, poolSize, owner);
	}
	
	ComponentPool(OPBaseScene* scene, const std::string& componentName, const std::string& poolBaseName, int poolSize, std::function<void(int, T*)> handler = nullptr) {
		createPoolFromPackage(scene->getPackageName(), scene->getView(), componentName, poolBaseName, poolSize, handler);
	}
	
	ComponentPool(const std::string& packageName, fairygui::GComponent* target, const std::string& componentName, const std::string& poolBaseName, int poolSize, std::function<void(int, T*)> handler = nullptr) {
		createPoolFromPackage(packageName, target, componentName, poolBaseName, poolSize, handler);
	}
	
	~ComponentPool() {
		_pool.clear();
	}
	
	T* getObject(int idx) {
		if(idx < _pool.size())
			return _pool.at(idx);

		return nullptr;
	}
	std::vector<T*>& getPool() { return _pool; }
	
	void clear() { _pool.clear(); }
	
	int getSize() { return (int)_pool.size(); }
	
private:
	std::vector<T*> _pool{nullptr};
	
	void createPool(const std::string& poolBaseName, int poolSize, fairygui::GComponent* owner) {
		_pool.clear();
		for(int i = 0; i < poolSize; i++)
		{
			auto obj = owner->getChild(poolBaseName + std::to_string(i))->as<T>();
			if(obj)
				_pool.push_back(obj);
		}
	}
	
	void createPoolFromPackage(const std::string& packageName, fairygui::GComponent* target, const std::string& componentName, const std::string& poolBaseName, int poolSize, std::function<void(int, T*)> handler = nullptr) {
		_pool.clear();
		
		if (!packageName.empty()) {
			for(int i = 0; i < poolSize; i++) {
				std::string name = poolBaseName + std::to_string(i);
				T* component = fairygui::UIPackage::createObject(packageName, componentName)->template as<T>();
				component->name = name;
				component->setVisible(false);
				
				target->addChild(component);
				_pool.push_back(component);
				
				if(handler)
					handler(i, component);
			}
		}
	}
	
};

class ComponentRect
{
public:
	static cocos2d::Rect getRect(fairygui::GObject& obj) {
		cocos2d::Rect rect;
		cocos2d::Vec2 pos = OPFairyUtils::getScreenPosition(&obj);

		rect.setRect(pos.x, pos.y, obj.getSize().width, obj.getSize().height);
		return rect;
	}
	
};

}

#endif /* ComponentPool_hpp */
