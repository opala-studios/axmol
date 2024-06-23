//
// Created by Talles Borges  on 02/03/20.
//

#ifndef OPBaseScene_hpp
#define OPBaseScene_hpp

#include <cocos2d.h>
#include <FairyGUI.h>
#include <vector>

#include "../Localization/OPLocalization.hpp"

typedef std::pair<std::string, fairygui::UIObjectFactory::GComponentCreator> SEL_ComponentCreator;

struct OPBaseSceneParams {
    std::string packagePath = "fairy/";
    std::string packageName;
    std::string viewName = "Main";
    std::string safeAreaViewName = "safeArea";
    std::string bannerAreaViewName = "adsBannerArea";
	std::vector<TranslatableItem> translations;

    virtual ~OPBaseSceneParams() = default;
};

struct OPBaseSceneButtonClickTestEvent {
	fairygui::GObject* component;
	const fairygui::EventContext* ev;
	int eventType;
	float posX;
	float posY;
};

struct OPBaseSceneChangeSceneEvent {
	std::string sceneName;
};

class OPBaseScene : public cocos2d::Scene
{
public:

    typedef std::function<void()> SEL_backPressedAction;
	typedef std::function<void(const OPBaseScene::SEL_backPressedAction&)> SEL_PushBackActionHandler;
	typedef std::function<void()> SEL_PopBackActionHandler;

    std::unique_ptr<OPBaseSceneParams> _params;
    fairygui::GObject* getBannerArea() const;
    fairygui::GObject* getSafeArea() const;
    fairygui::GComponent* getView() const;
    fairygui::GRoot* getGRoot() const;

    void setIsWaitingOnExit(bool isWaitingOnExit);
    void runOnceUntilExit(const std::function<void()>& action);

    template<typename T>
    T* getParams() { return static_cast<T*>(_params.get()); }

    virtual bool init(std::unique_ptr<OPBaseSceneParams> params = nullptr);
	~OPBaseScene() override;

	void bindClickListener(const std::string &path, const fairygui::EventCallback &buttonCallback, const fairygui::EventTag &tag = fairygui::EventTag::None) const;

	template <typename T>
    T *getChild(const std::string &path, bool required = true) const {
        auto *found = getView()->getChildByPath(path);
        if (required && found == nullptr) {
            std::stringstream sstream;
            sstream << "No object exists on path '" << path << "'";
            throw std::runtime_error(sstream.str());
        }
        T *casted = found->as<T>();
        if (required && casted == nullptr) {
            std::stringstream sstream;
            sstream << "Found required object on path '" << path << "' but it isn't of type '"
                    << typeid(T).name() << "'";
            throw std::runtime_error(sstream.str());
        }
        return casted;
    }

    template<typename T>
    bool findChild(const std::string &path, T **result) const {
        return (*result = getChild<T>(path, false)) != nullptr;
    }
	
	virtual std::string getPackageName() {
		return _params->packageName + _packageSufix;
	};
	
	virtual std::string getPackagePath() {
		return _params->packagePath;
	};

	virtual std::string getViewName() {
		return _params->viewName;
	};

	virtual std::string getSafeAreaViewName() {
		return _params->safeAreaViewName;
	}

	virtual std::string getBannerAreaViewName() {
		return _params->bannerAreaViewName;
	}
    
    virtual void onBackPressed();

protected:

    // Execute the block once, you can call it again after the scene is exited or setIsWaitingOnExit to false
    // Prevent showing scene twice, or popeing twice.
    //TODO: Refactor it to use Validator
	bool _isWaitingOnExit;

    virtual void onEnter() override;
    void onExitTransitionDidStart() override;
	void onEnterTransitionDidFinish() override;
    void onExit() override;

protected:
    void updateBannerArea();

    virtual bool requiresBanner() {
        return true;
    }

    cocos2d::EventListenerCustom* _bannerVisibilityListener;

	fairygui::GRoot *_gRoot;
	fairygui::GComponent *_view;


    std::vector<SEL_backPressedAction> _backActionStack;

	virtual std::vector<std::pair<std::string, std::string>> getCustomFonts() {
		return {};
	}

    virtual std::vector<std::string> getDependenciesPackagesNames() {
        return {};
    }

    virtual std::vector<SEL_ComponentCreator> getCustomComponents() {
        return {};
	}

	// If a scene is root then pressing the back button wont pop this scene, otherwise the current scene will be popped when pressing the back button
	virtual bool isRoot() { return false; }

	// Called when back action stack is empty and back action is pressed
	virtual void closeScene();

	// Pushes an action on the action stack
	void pushBackAction(const SEL_backPressedAction& action);

	// Clear the stack and pushes an action, resulting in one action on the stack.
	void registerBackAction(const SEL_backPressedAction& action);
	void popBackAction() { _backActionStack.pop_back(); }
	void clearBackAction() { _backActionStack.clear(); }
    
    virtual void setupScreenBranch();
private:
    fairygui::GObject* _safeArea;
    fairygui::GObject* _bannerArea;
    
    std::string _screenBranch{"Trunk"};
    
    std::string _packageSufix{""};


    void showBannerArea();
    void hideBannerArea();

    void updateSafeAreaBounds();
    
};

#endif //OPBaseScene_hpp
