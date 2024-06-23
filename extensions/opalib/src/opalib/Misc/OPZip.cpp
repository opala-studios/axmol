#include "OPZip.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Helpers/OPString.hpp"
#include "../Misc/OPPath.hpp"
#include "../Misc/OPFile.hpp"
#include "../Misc/OPDirectory.hpp"

#include <cocos2d.h>
#include <fstream>
#include <miniz/zip_file.hpp>
//#include <minizip/zip.h>

USING_NS_CC;

std::string OPZip::findRootDirectory(const std::vector<std::string>& paths)
{
    if (paths.empty()) {
        return "";
    }
    
    auto prefix = paths.front();
    if (paths.size() == 1) {
        return directoryFromPrefix(prefix);
    }
    
    for (size_t i = 1; i < paths.size(); ++i)
    {
        size_t size = std::min(prefix.size(), paths[i].size());
        for (size_t j = 0; j < size; ++j) {
            if (paths[i][j] != prefix[j]) {
                prefix = prefix.substr(0, j);
                break;
            }
        }
    }
    
    return directoryFromPrefix(prefix);
}

std::string OPZip::directoryFromPrefix(const std::string& prefix)
{
    auto index = prefix.find_last_of('/');
    if (index != std::string::npos) {
        return prefix.substr(0, index + 1);
    }
    return "";
}

bool OPZip::zip(const std::string& directory, const std::string& zipPath)
{
    std::vector<std::string> filesToZip;
    std::vector<std::string> files;

    FileUtils::getInstance()->listFilesRecursively(directory, &files);
    for (const std::string& file : files) {
        if (!OPString::endsWith(file, "/"))
            filesToZip.push_back(file);
    }

    return zip(filesToZip, zipPath);
}

bool OPZip::zip(const std::vector<std::string> &files, const std::string &zipPath, std::string root)
{
    std::remove(zipPath.c_str());

    if (root.empty()) {
        root = findRootDirectory(files);
    }

    miniz_cpp::zip_file zip;
    
    for (const auto& originalPath : files)
    {
        auto pathOnZip = originalPath.substr(root.size());
        zip.write(originalPath, pathOnZip);
    }
    
    zip.save(zipPath);
    return true;
}

bool OPZip::zip(const std::vector<std::pair<std::string, std::string>>& filesMap, const std::string& zipPath)
{
    std::remove(zipPath.c_str());

    miniz_cpp::zip_file zip;

    for (const auto& fileMap : filesMap)
    {
        zip.write(fileMap.first, fileMap.second);
    }

    zip.save(zipPath);
    return true;
}
bool OPZip::unzip(const std::string& zipPath, const std::string& directory)
{
    OPDebug::log("OPZip", "Start unzipping file at: " + zipPath);

    auto data = FileUtils::getInstance()->getDataFromFile(zipPath);

    OPDebug::log("OPZip", "Creating buffer from zip.");
    auto zip = ZipFile::createWithBuffer(data.getBytes(), data.getSize());

    if (!zip)
    {
        OPDebug::error("OPZip", "Could not create cocos2d::Zip.");
        return false;
    }

    std::string fileName = zip->getFirstFilename();
    if (fileName.size() == 0) {
        return false;
    }

    std::vector<std::string> files;
    files.push_back(fileName);

    while ((fileName = zip->getNextFilename()).size() > 0) {
        files.push_back(fileName);
    }

    std::vector<std::string> filesToExtract;
    std::copy_if(files.begin(), files.end(), std::back_inserter(filesToExtract), [](const std::string& name) {
        return !OPString::startsWith(name, "__MACOSX") && !OPString::endsWith(name, ".DS_Store") && !OPString::endsWith(name, "/");
    });

    if (!FileUtils::getInstance()->isDirectoryExist(directory)) {
        FileUtils::getInstance()->createDirectory(directory);
    }

    OPDebug::log("OPZip", "Unzipping file at " + zipPath);
    ssize_t dataSize;
    auto success = true;

    for (const auto& file : filesToExtract)
    {
        std::string extractedPath = OPPath::join(directory, file);
        OPDebug::log("OPZip", "Extracting file " + file + " to " + extractedPath);

        auto directory = OPPath::removeLastComponent(extractedPath);
        if (!FileUtils::getInstance()->isDirectoryExist(directory)) {
            FileUtils::getInstance()->createDirectory(directory);
        }

        auto data = zip->getFileData(file, &dataSize);
        if (dataSize <= 0)
        {
            OPDebug::error("OPZip", "Error extracting " + file);
            success = false;
        }
        else
        {
            OPDebug::log("OPZip", std::to_string(dataSize) + " bytes extracted.");

            std::ofstream file(extractedPath);
            file.write((char*)data, dataSize);
            file.close();

            delete data;
        }
    }

    CC_SAFE_DELETE(zip);

    if (success) {
        OPDebug::log("OPZip", "Zip extraction finished successfully!");
    }
    else {
        OPDebug::error("OPZip", "Failed to extract some files.");
    }
//
    return success;
}
