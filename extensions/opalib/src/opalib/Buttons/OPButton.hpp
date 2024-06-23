#ifndef OPButton_hpp
#define OPButton_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class OPButton : public cocos2d::Node
{
public:
    
    typedef std::function<void(cocos2d::Ref*)> SEL_ClickHandler;

private:

    OPButton() {}

    bool soundEnabled {true};
    bool down {false};
    bool touchInside {false};
    bool containerPressed {false};
    bool clicksOnDown {false};
    bool delaysCallback {true};
    bool clickable {true};

    SEL_ClickHandler onClick {0};

    const int ContainerAnimationTag {1000};

    cocos2d::Node* container;
    cocos2d::Sprite* sprite {0};
    cocos2d::ui::Margin margin;

    void updateState();
    void registerClick();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:

    void click();

    virtual bool init() override;
    virtual bool initWithSprite(const std::string& imagePath);
    virtual bool initWithScale9Sprite(cocos2d::Rect capInsets, const std::string& imagePath);
    virtual bool initWithSpriteFrameName(const std::string& name);
    virtual bool initWithSpriteFrameName(const std::string& name, const cocos2d::Rect& capInsets);

    static OPButton* create();
    static OPButton* create(const std::string& path);
    static OPButton* create(cocos2d::Rect capInsets, const std::string& path);
    static OPButton* createWithSpriteFrameName(const std::string& name, cocos2d::Rect capInsets);
    static OPButton* createWithSpriteFrameName(const std::string& name);

    bool isSoundEnabled() { return soundEnabled; }
    void setSoundEnabled(bool sound) { soundEnabled = sound; }

    cocos2d::ui::Margin getMargin() { return margin; }
    void setMargin(const cocos2d::ui::Margin& margin) 
    { 
        this->margin = margin;
        updateContentSize();
    }

    virtual void setSpriteSize(const cocos2d::Size& size);
    
    void setClickable(bool clickable) { this->clickable = clickable; }
    bool isClickable() { return clickable; }

    void setClicksOnDown(bool clicks) { clicksOnDown = clicks; }
    bool getClicksOnDown() { return clicksOnDown; }

    void setDelaysCallback(bool delays) { delaysCallback = delays; }
    bool getDelaysCallback() { return delaysCallback; }
    
    virtual void addChildToContainer(cocos2d::Node *node);
    virtual void addChildToContainer(cocos2d::Node *node, int localZOrder);

    virtual void setContentSize(const cocos2d::Size& size) override;
    void updateContentSize();

    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override;
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled) override;

    cocos2d::Node* getContainer() { return container; }
    cocos2d::Sprite* getSprite() { return sprite; }

    void addClickEventListener(const SEL_ClickHandler& handler) {
        onClick = handler;
    }
};

#endif /* OPButton_hpp */