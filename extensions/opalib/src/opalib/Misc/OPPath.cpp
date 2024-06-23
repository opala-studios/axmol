//
//  OPPath.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 02/08/19.
//

#include "OPPath.hpp"
#include "../Helpers/OPString.hpp"

std::string OPPath::lastComponent(const std::string& path)
{
    size_t slashPos = path.find_last_of('/');
    
    if (slashPos != std::string::npos) {
        return path.substr(slashPos + 1);
    }
    
    return path;
}

std::string OPPath::join(const std::string& path, const std::string& component)
{
    std::string newPath = path;
    std::string newComponent = component;
    
    if (!OPString::endsWith(newPath, "/")) {
        newPath += "/";
    }

    if (OPString::startsWith(component, "/")) {
        newComponent = newComponent.substr(1);
    }
    
    return newPath + newComponent;
}

std::string OPPath::removeLastComponent(const std::string& path)
{
    auto index = path.find_last_of('/');
    if (index == std::string::npos) {
        return "";
    }

    return path.substr(0, index);
}

std::string OPPath::fileNameWithoutExtension(const std::string& path)
{
    auto str = path;
    auto slashIndex = path.find_last_of("/");
    auto extensionIndex = path.find_last_of(".");
    
    if (extensionIndex != std::string::npos) {
        str = str.substr(0, extensionIndex);
    }
    
    if (slashIndex != std::string::npos) {
        str = str.substr(slashIndex + 1);
    }
    
    return str;
}
