#include "OPButton.hpp"
#include "../Crash/OPCrashAnalytics.hpp"
#include "../Debug/OPDebug.hpp"
#include "../SoundEffects/OPSoundController.h"
#include "../Misc/OPTouch.hpp"

USING_NS_CC;

OPButton* OPButton::create()
{
    auto button = new(std::nothrow) OPButton();
    if (button && button->init())
    {
        button->autorelease();
        return button;
    }

    delete button;
    button = 0;

    return 0;
}

OPButton* OPButton::create(const std::string& path)
{
    auto button = new(std::nothrow) OPButton();
    if (button && button->initWithSprite(path))
    {
        button->autorelease();
        return button;
    }

    delete button;
    button = 0;

    return 0;
}

OPButton* OPButton::create(cocos2d::Rect capInsets, const std::string& path)
{
    auto button = new(std::nothrow) OPButton();
    if (button && button->initWithScale9Sprite(capInsets, path))
    {
        button->autorelease();
        return button;
    }

    delete button;
    button = 0;

    return 0;
}

OPButton* OPButton::createWithSpriteFrameName(const std::string& name, cocos2d::Rect capInsets)
{
    auto button = new(std::nothrow) OPButton();
    if (button && button->initWithSpriteFrameName(name, capInsets))
    {
        button->autorelease();
        return button;
    }

    delete button;
    button = 0;

    return 0;
}

OPButton* OPButton::createWithSpriteFrameName(const std::string& name)
{
    auto button = new(std::nothrow) OPButton();
    if (button && button->initWithSpriteFrameName(name))
    {
        button->autorelease();
        return button;
    }

    delete button;
    button = 0;

    return 0;
}

bool OPButton::init()
{
    if (!Node::init()) {
        return false;
    }

    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ONE * 0.5);

    container = Node::create();
    container->setIgnoreAnchorPointForPosition(false);
    container->setAnchorPoint(Vec2::ONE * 0.5);
    addChild(container);

    auto touch = EventListenerTouchOneByOne::create();
    touch->setSwallowTouches(true);
    touch->onTouchBegan = CC_CALLBACK_2(OPButton::onTouchBegan, this);
    touch->onTouchMoved = CC_CALLBACK_2(OPButton::onTouchMoved, this);
    touch->onTouchEnded = CC_CALLBACK_2(OPButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);

    return true;
}

void OPButton::updateState()
{
    if (containerPressed == (down && touchInside)) {
        return;
    }

    containerPressed = !containerPressed;
    container->stopActionByTag(ContainerAnimationTag);
    
    ScaleTo* scale = 0;
    if (containerPressed) 
    {
        container->setScale(1);
        scale = ScaleTo::create(0.07, 0.93);
    }
    else 
    {
        container->setScale(0.93);
        scale = ScaleTo::create(0.15, 1);
    }

    auto action = EaseCubicActionOut::create(scale);
    action->setTag(ContainerAnimationTag);
    container->runAction(action);
}

void OPButton::registerClick()
{
    if (soundEnabled) {
        OPSoundController::shared()->play("button");
    }

    if (onClick) {
        onClick(this);
    }
}

void OPButton::click()
{
    if (!delaysCallback) {
        registerClick();
    }

    auto downAction = EaseCubicActionOut::create(ScaleTo::create(0.07, 0.93));
    auto callAction = CallFunc::create([this]() 
    {
        if (delaysCallback) {
            registerClick();
        }
    });
    auto upAction = EaseCubicActionOut::create(ScaleTo::create(0.15, 1));
    
    runAction(
        Sequence::create(
            downAction,
            callAction,
            upAction,
            nullptr
        )
    );
}

bool OPButton::onTouchBegan(Touch* touch, Event* event)
{
    if (!isVisible()) {
        return false;
    }

    auto contact = OPTouch::checkTouch(touch, event);
    if (contact && !clickable) {
        return true;
    }

    if (!contact) {
        return false;
    }

    if (clicksOnDown) 
    {
        click();
        return true;
    }

    down = contact;
    touchInside = down;
    updateState();

    return down;
}

void OPButton::onTouchMoved(Touch* touch, Event* event)
{
    if (clicksOnDown || !clickable) {
        return;
    }

    touchInside = OPTouch::checkTouch(touch, event);
    updateState();
}

void OPButton::onTouchEnded(Touch* touch, Event* event)
{
    if (clicksOnDown || !clickable) {
        return;
    }

    if (down && touchInside) {
        registerClick();
    }

    down = false;
    touchInside = false;

    updateState();
}

bool OPButton::initWithSprite(const std::string& path)
{
    if (!OPButton::init()) {
        return false;
    }

    sprite = Sprite::create(path);
    container->addChild(sprite);

    updateContentSize();

    return true;
}

bool OPButton::initWithScale9Sprite(cocos2d::Rect capInsets, const std::string& path)
{
    if (!OPButton::init()) {
        return false;
    }

    sprite = ui::Scale9Sprite::create(capInsets, path);
    container->addChild(sprite);

    updateContentSize();    

    return true;
}

bool OPButton::initWithSpriteFrameName(const std::string& name)
{
    if (!OPButton::init()) {
        return false;
    }

    sprite = ui::Scale9Sprite::createWithSpriteFrameName(name);
    container->addChild(sprite);

    updateContentSize();    

    return true;
}

bool OPButton::initWithSpriteFrameName(const std::string& name, const cocos2d::Rect& capInsets)
{
    if (!OPButton::init()) {
        return false;
    }

    sprite = ui::Scale9Sprite::createWithSpriteFrameName(name, capInsets);
    container->addChild(sprite);

    updateContentSize();    

    return true;
}

void OPButton::updateContentSize()
{
    if (!sprite) {
        return;
    }

    auto spriteSize = sprite->getContentSize();

    auto size = spriteSize;
    size.width += margin.left + margin.right;
    size.height += margin.top + margin.bottom;

    setContentSize(size);
}

void OPButton::setContentSize(const cocos2d::Size& size)
{
    Node::setContentSize(size);
  
    if (sprite)
    {
        auto spriteSize = sprite->getContentSize();
        container->setPosition(spriteSize.width * 0.5 + margin.left, spriteSize.height * 0.5 + margin.bottom);
    }
    else {
        container->setPosition(size * 0.5);
    }
}

void OPButton::addChildToContainer(Node* node)
{
    container->addChild(node);
}

void OPButton::addChildToContainer(Node* node, int localZOrder)
{
    container->addChild(node, localZOrder);
}

void OPButton::setSpriteSize(const cocos2d::Size& size)
{
    if (sprite) 
    {
        sprite->setContentSize(size);
        updateContentSize();
    }
    else {
        OPDebug::warn("OPButton", "setSpriteSize is only valid when you create an OPButton with a sprite.");
    }
}

void OPButton::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    Node::setCascadeOpacityEnabled(cascadeOpacityEnabled);
    
    container->setCascadeOpacityEnabled(cascadeOpacityEnabled);

    if (sprite) {
        sprite->setCascadeOpacityEnabled(cascadeOpacityEnabled);
    }
}

void OPButton::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    Node::setCascadeColorEnabled(cascadeColorEnabled);
    
    container->setCascadeColorEnabled(cascadeColorEnabled);

    if (sprite) {
        sprite->setCascadeColorEnabled(cascadeColorEnabled);
    }
}
