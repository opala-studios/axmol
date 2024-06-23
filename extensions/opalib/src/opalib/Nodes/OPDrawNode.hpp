#ifndef OPDrawNode_hpp
#define OPDrawNode_hpp

#include <cocos2d.h>

class OPDrawNode : public cocos2d::DrawNode
{
public:

    void drawSolidArc(cocos2d::Vec2 origin, float radius, unsigned int segments, float startAngle, float endAngle, const cocos2d::Color4F& color);

    CREATE_FUNC(OPDrawNode);
};

#endif /* OPDrawNode_hpp */