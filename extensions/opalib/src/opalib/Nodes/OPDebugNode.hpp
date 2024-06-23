#ifndef OPDebugNode_hpp
#define OPDebugNode_hpp

#include <cocos2d.h>

class OPDebugNode : public cocos2d::DrawNode
{
private:

    cocos2d::Color4F strokeColor;
    cocos2d::Color4F fillColor;

public:

    cocos2d::Color4F getStrokeColor() { return strokeColor; }
    void setStrokeColor(const cocos2d::Color4F& color) { strokeColor = color; }

    cocos2d::Color4F getFillColor() { return fillColor; }
    void setFillColor(const cocos2d::Color4F& color) { fillColor = color; }

    virtual bool init() override;
    virtual void update(float dt) override;
    
    CREATE_FUNC(OPDebugNode);
};

#endif /* OPDebugNode_hpp */