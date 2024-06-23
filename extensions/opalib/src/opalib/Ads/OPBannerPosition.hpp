//
//  OPBannerPosition.h
//  DrumsCocos
//
//  Created by Adilson Tavares on 02/07/19.
//

#ifndef OPBannerPosition_h
#define OPBannerPosition_h

#include <iostream>
#include "../Helpers/OPString.hpp"
#include <json/document-wrapper.h>

struct OPBannerPosition
{
public:
    
    enum Horizontal
    {
        Left,
        Center,
        Right
    };
    
    enum Vertical
    {
        Top,
        Bottom
    };


    Horizontal horizontal;
    Vertical vertical;
    
    static const OPBannerPosition TopLeft;
    static const OPBannerPosition TopCenter;
    static const OPBannerPosition TopRight;
    static const OPBannerPosition BottomLeft;
    static const OPBannerPosition BottomCenter;
    static const OPBannerPosition BottomRight;
    
    bool operator==(const OPBannerPosition& other)
    {
        return this->horizontal == other.horizontal && this->vertical == other.vertical;
    }
    
    bool operator!=(const OPBannerPosition& other)
    {
        return this->horizontal != other.horizontal || this->vertical != other.vertical;
    }
    
    OPBannerPosition()
    {
        horizontal = Horizontal::Center;
        vertical = Vertical::Bottom;
    }

    OPBannerPosition(const rapidjson::Value& root)
    {
        vertical = Vertical::Bottom;
        horizontal = Horizontal::Center;

        if (root.HasMember("bannerPosition")) {
            auto positionValue = std::string(root["bannerPosition"].GetString());
            auto positions = {TopLeft, TopCenter, TopRight, BottomLeft, BottomCenter, BottomRight};
            for (auto position : positions) {
                if (position.getName() == positionValue) {
                    horizontal = position.horizontal;
                    vertical = position.vertical;
                    break;
                }
            }
        }
    }


    std::string getName()
    {
        std::string name = "";
        
        switch (vertical)
        {
            case Vertical::Bottom:      name = "bottom";    break;
            case Vertical::Top:         name = "top";       break;
            default:                    name = "unknown";   break;
        }
        
        name += " ";
        
        switch (horizontal)
        {
            case Horizontal::Left:      name += "left";     break;
            case Horizontal::Center:    name += "center";   break;
            case Horizontal::Right:     name += "right";    break;
            default:                    name += "unknown";  break;
        }
        
        return name;
    }
    
private:
    
    OPBannerPosition(Horizontal horizontal, Vertical vertical);
};

#endif /* OPBannerPosition_h */
