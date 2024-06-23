//
//  OPFile.cpp
//  Opalib
//
//  Created by Adilson Tavares on 24/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPFile.hpp"

#include <cocos2d.h>

#include <thread>

#include "OPPath.hpp"

USING_NS_CC;

std::string OPFile::getWritablePath(const std::string& relativePath)
{
    return OPPath::join(FileUtils::getInstance()->getWritablePath(), relativePath);
}

bool OPFile::exists(const std::string& path)
{
    return FileUtils::getInstance()->isFileExist(path);
}

void OPFile::remove(const std::string& path)
{
    FileUtils::getInstance()->removeFile(path);
}

std::string OPFile::readString(const std::string& filePath)
{
    return FileUtils::getInstance()->getStringFromFile(filePath);
}

void OPFile::writeString(const std::string& content, const std::string& filePath)
{
    FileUtils::getInstance()->writeStringToFile(content, filePath);
}

void OPFile::move(const std::string& fromPath, const std::string& toPath)
{
    if (fromPath == toPath)
        return;
    copy(fromPath, toPath);
    remove(fromPath);
}

void OPFile::copy(const std::string& fromPath, const std::string& toPath)
{
    Data data = FileUtils::getInstance()->getDataFromFile(fromPath);
    FileUtils::getInstance()->createDirectory(OPPath::removeLastComponent(toPath));
    FileUtils::getInstance()->writeDataToFile(data, toPath);
}
