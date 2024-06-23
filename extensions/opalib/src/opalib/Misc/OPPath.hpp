//
//  OPPath.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 02/08/19.
//

#ifndef OPPath_hpp
#define OPPath_hpp

#include <iostream>

class OPPath
{
public:
    
    static std::string join(const std::string& path, const std::string& component);
    static std::string removeLastComponent(const std::string& path);
    
    static std::string lastComponent(const std::string& path);
    
    static std::string fileNameWithoutExtension(const std::string& path);
};

#endif /* OPPath_hpp */
