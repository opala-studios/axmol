#include "OPDebugNode.hpp"
#include "../Debug/OPDebug.hpp"

USING_NS_CC;

bool OPDebugNode::init()
{
    if (!DrawNode::init()) {
        return false;
    }

    setAnchorPoint(Vec2::ZERO);
    setIgnoreAnchorPointForPosition(false);

    strokeColor = Color4F(1, 0, 0, 0.7);
    fillColor = Color4F(1, 0, 0, 0.43);

    update(0);
    scheduleUpdate();

    return true;
}

void OPDebugNode::update(float dt)
{
    DrawNode::update(dt);

    auto parent = getParent();
    if (!parent) {
        return;
    }

    auto size = parent->getContentSize();

    clear();
    drawSolidRect(Vec2::ZERO, size, fillColor);
    drawRect(Vec2::ZERO, size, strokeColor);

    setContentSize(size);
    setPosition(Vec2::ZERO);
}
