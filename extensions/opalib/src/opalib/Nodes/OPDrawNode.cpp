#include "OPDrawNode.hpp"

USING_NS_CC;

void OPDrawNode::drawSolidArc(cocos2d::Vec2 origin, float radius, unsigned int segments, float startAngle, float endAngle, const cocos2d::Color4F& color)
{
    startAngle *= M_PI / 180;
    endAngle *= M_PI / 180;

    auto deltaAngle = (endAngle - startAngle) / segments;
    auto angle = startAngle;
    auto oldPosition = origin + Vec2(
        cos(startAngle) * radius,
        sin(startAngle) * radius
    );

    for (auto i = 1; i <= segments; ++i)
    {
        angle += deltaAngle;

        auto position = origin + Vec2(
            cos(angle) * radius,
            sin(angle) * radius
        );

        drawTriangle(origin, position, oldPosition, color);
        oldPosition = position;
    }
}
