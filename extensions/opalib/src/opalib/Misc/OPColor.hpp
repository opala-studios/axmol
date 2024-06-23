#ifndef OPColor_hpp
#define OPColor_hpp

#include <cocos2d.h>

class OPColor
{
private:

    OPColor() {} 

    static std::string colorComponentToHex(uint8_t comp);

public:

    static cocos2d::Color3B parse3B(const std::string& str);
    
    static std::string toString(const cocos2d::Color3B& color);

    static std::string toHex(cocos2d::Color3B color) {
        return toHex(color.r, color.g, color.b);
    }

    static std::string toHex(cocos2d::Color4B color) {
        return toHex(color.r, color.g, color.b, color.a);
    }

    static std::string toHex(uint8_t r, uint8_t g, uint8_t b);
    static std::string toHex(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    
    static cocos2d::Color3B parse3BFromStrHex(std::string str);
};

#endif
