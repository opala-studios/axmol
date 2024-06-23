//
//  OPDirectory.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 06/08/19.
//

#ifndef OPDirectory_hpp
#define OPDirectory_hpp

#include <iostream>
#include <vector>

class OPDirectory
{
public:

    static std::string getTemp();
    static std::string getTemp(const std::string& relativePath);

    static bool create(const std::string& path);
    static bool exists(const std::string& path);
    static void remove(const std::string& path);

    static std::string getWritablePath();
    static std::string getWritablePath(const std::string& relativePath, bool shouldCreateRelativeDir = false);

    static std::vector<std::string> getFileNames(const std::string& directory);
    
    static void copyFromTo(const std::string& origDirectoryPath,const std::string& destDirectoryPath,const std::vector<std::string>& extensions);
    
    static bool isAssetsFolder(const std::string& path);
    static std::string getFullPathForFileName(const std::string& fileName);
    
};

#endif /* OPDirectory_hpp */
