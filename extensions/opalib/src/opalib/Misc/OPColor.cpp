#include "OPColor.hpp"
#include "../Helpers/OPString.hpp"
#include <sstream>
#include <iomanip>

USING_NS_CC;

cocos2d::Color3B OPColor::parse3B(const std::string& str)
{
    auto result = OPString::split(str, ';');
    if (result.size() != 3) {
        return Color3B::BLACK;
    }

    auto r = atoi(result[0].c_str());
    auto g = atoi(result[1].c_str());
    auto b = atoi(result[2].c_str());

    return Color3B(r, g, b);
}

std::string OPColor::toString(const cocos2d::Color3B& color)
{
    std::string str;
    str += std::to_string(color.r) + ";";
    str += std::to_string(color.g) + ";";
    str += std::to_string(color.b);

    return str;
}

std::string OPColor::colorComponentToHex(uint8_t comp)
{
    std::stringstream ss;
    ss << std::setfill('0') 
        << std::setw(sizeof(uint8_t) * 2)
        << std::hex 
        << (int)comp;

    return ss.str();
}

std::string OPColor::toHex(uint8_t r, uint8_t g, uint8_t b)
{
    return "#" +
        OPColor::colorComponentToHex(r) + 
        OPColor::colorComponentToHex(g) + 
        OPColor::colorComponentToHex(b);
}

std::string OPColor::toHex(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return "#" +
        OPColor::colorComponentToHex(r) + 
        OPColor::colorComponentToHex(g) + 
        OPColor::colorComponentToHex(b) +
        OPColor::colorComponentToHex(a);
}

cocos2d::Color3B OPColor::parse3BFromStrHex(std::string str)
{
    OPString::replace(str, "#", "");
    if (str.size() < 6) {
        return Color3B::BLACK;
    }
    
    auto r = std::stoul(str.substr(0,2),nullptr,16);
    auto g = std::stoul(str.substr(2,2),nullptr,16);
    auto b = std::stoul(str.substr(4,2),nullptr,16);

    return Color3B(r, g, b);
}
