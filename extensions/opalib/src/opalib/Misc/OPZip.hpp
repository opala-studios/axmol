#ifndef OPZip_hpp
#define OPZip_hpp

#include <iostream>
#include <vector>

class OPZip
{
public:

    static bool zip(const std::string& directory, const std::string& zipPath);
    static bool zip(const std::vector<std::string>& files, const std::string& zipPath, std::string root = "");
    static bool zip(const std::vector<std::pair<std::string,std::string>>& filesMap, const std::string& zipPath);
    static bool unzip(const std::string& zipPath, const std::string& directory);
    
protected:
    
    static std::string findRootDirectory(const std::vector<std::string>& paths);
    static std::string directoryFromPrefix(const std::string& prefix);
};

#endif /* OPZip_hpp */
