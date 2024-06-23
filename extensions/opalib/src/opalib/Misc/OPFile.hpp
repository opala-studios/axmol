//
//  OPFile.hpp
//  Opalib
//
//  Created by Adilson Tavares on 24/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPFile_hpp
#define OPFile_hpp

#include <iostream>

class OPFile
{
public:
    
    static std::string getWritablePath(const std::string& relativePath);

    static void remove(const std::string& path);

    static void copy(const std::string& fromPath, const std::string& toPath);
    static void move(const std::string& fromPath, const std::string& toPath);

    static std::string readString(const std::string& filePath);
    
    static bool exists(const std::string& path);
    static void writeString(const std::string& content, const std::string& filePath);
};

#endif /* OPFile_hpp */

