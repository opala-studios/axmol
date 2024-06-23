//
//  OPDirectory.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 06/08/19.
//

#include "OPDirectory.hpp"
#include "OPPath.hpp"
#include "../Helpers/OPString.hpp"
#include <cocos2d.h>

USING_NS_CC;

#include <dirent.h>

std::string OPDirectory::getTemp()
{
    auto writable = FileUtils::getInstance()->getWritablePath();
    auto temp = OPPath::join(writable, "temp");

    return temp;
}

bool OPDirectory::create(const std::string& path)
{
    return FileUtils::getInstance()->createDirectory(path);
}

std::string OPDirectory::getTemp(const std::string& relativePath)
{
    auto temp = getTemp();
    return OPPath::join(temp, relativePath);
}

bool OPDirectory::exists(const std::string& path)
{
    return FileUtils::getInstance()->isDirectoryExist(path);
}

void OPDirectory::remove(const std::string& path)
{
    FileUtils::getInstance()->removeDirectory(path);
}

std::string OPDirectory::getWritablePath()
{
    return FileUtils::getInstance()->getWritablePath();
}

std::string OPDirectory::getWritablePath(const std::string& relativePath, bool shouldCreateRelativeDir)
{
    auto path = OPPath::join(getWritablePath(), relativePath);
    
    if(shouldCreateRelativeDir)
    {
        auto dirPath = OPPath::removeLastComponent(path);
        if(!FileUtils::getInstance()->isDirectoryExist(dirPath))
        {
            FileUtils::getInstance()->createDirectory(dirPath);
        }
    }
    
    return path;
}

std::vector<std::string> OPDirectory::getFileNames(const std::string& directory)
{
    DIR* dir;
    struct dirent* ent;
    
    std::vector<std::string> files;
    
    if ((dir = opendir(directory.c_str())))
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string fileName = ent->d_name;
            if (fileName != "." && fileName != "..") {
                files.push_back(fileName);
            }
        }
        
        closedir(dir);
    }
    
    return files;
}

//TODO:: copyFromTo isn't working on Android - Need fix it
void OPDirectory::copyFromTo(const std::string& origDirectoryPath,const std::string& destDirectoryPath,const std::vector<std::string>& extensions)
{
    auto fm = FileUtils::getInstance();
    std::vector<std::string> files = fm->listFiles(origDirectoryPath);
    
    if(!fm->isDirectoryExist(destDirectoryPath))
    {
       fm->createDirectory(destDirectoryPath);
    }
    
    for(std::string f : files)
    {
        std::string ext = fm->getFileExtension(f);
        bool isExtensionAsked = false;
        
        for(auto extension : extensions)
        {
            isExtensionAsked = extension == ext;
            if(isExtensionAsked) break;
        }
        
        if(isExtensionAsked)
        {
            std::vector<std::string> vec = OPString::split(f, '/');
            std::string fileName = vec.at(vec.size()-1);
            std::string destFilePath =  OPPath::join(destDirectoryPath, fileName);
            if(!fm->isFileExist(destFilePath))
            {
                fm->writeDataToFile(fm->getDataFromFile(f), destFilePath);
            }
        }
    }
}

bool OPDirectory::isAssetsFolder(const std::string& path)
{
    auto vec = OPString::split(path, '/');
    if(!vec.size()) return false;
    
    auto rootFolder = vec.at(0);
    return rootFolder == "assets";
}

std::string OPDirectory::getFullPathForFileName(const std::string& fileName) {
    return cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
}

