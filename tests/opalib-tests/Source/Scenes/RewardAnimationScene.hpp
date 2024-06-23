//
//  RewardAnimationScene.hpp
//  OpalibTemplate-mobile
//
//  Created by Opala Studios on 26/08/21.
//

#ifndef RewardAnimationScene_hpp
#define RewardAnimationScene_hpp

#include "Opalib.h"

class OPPool {
public:
    OPPool(const std::string& poolBaseName, int poolSize, fairygui::GComponent* owner);
    ~OPPool();
    
    fairygui::GComponent* getObject(int idx);
            
private:
    std::vector<fairygui::GComponent*> _pool{nullptr};
};

class OPRect
{
public:
	static cocos2d::Rect getRect(const fairygui::GObject& obj);
};

class RewardAnimationScene : public OPBaseScene
{
public:
   
    virtual bool init() override;
    CREATE_FUNC(RewardAnimationScene);
    void update(float deltaTime) override;
    
protected:
    virtual std::string getPackageName() override { return "RewardAnimation"; }
    virtual std::string getViewName() override { return "Scene"; }
    
    virtual std::vector<std::pair<std::string, std::string>> getCustomFonts() override
    {
        return
        {
           {"UniSans-HeavyCAPS", "fonts/CAPS.otf"},
           {"UniSansRegular", "fonts/UniSans-Regular.ttf"}
        };
    }
	
	int _numberCoinsSpawned = 0;
    int _coinsPoolSize = 100;
    int _starsPoolSize = 5;

    std::unique_ptr<OPPool> _coinsPool{nullptr};
	std::unique_ptr<OPPool> _rStarsPool{nullptr};
	std::unique_ptr<OPPool> _starsPool{nullptr};
	
	opalib::SpawnerGObject _coinsSpawner;
	opalib::SpawnerGObject _starsSpawner;
	
	fairygui::GButton* _coinsButton;
	fairygui::GObject* _coinDest;
	
	fairygui::GTextField* _coinsText;
	fairygui::GButton* _starsButton;
	
};
#endif /* RewardAnimationScene_hpp */
