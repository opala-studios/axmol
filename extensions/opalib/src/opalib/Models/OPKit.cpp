//
// Created by Talles Borges  on 24/07/20.
//

#include "OPKit.hpp"

const std::string& OPKit::getId() const
{
    return _id;
}

void OPKit::setId(const std::string& id)
{
    _id = id;
}

const std::string& OPKit::getName() const
{
    return _name;
}

void OPKit::setName(const std::string& name)
{
    _name = name;
}

const std::string& OPKit::getAuthorName() const
{
    return _authorName;
}

void OPKit::setAuthorName(const std::string& authorName)
{
    _authorName = authorName;
}

const std::string& OPKit::getMusicName() const
{
    return _musicName;
}

void OPKit::setMusicName(const std::string& musicName)
{
    _musicName = musicName;
}

bool OPKit::isDeleted() const
{
    return _isDeleted;
}

void OPKit::setIsDeleted(bool isDeleted)
{
    _isDeleted = isDeleted;
}

bool OPKit::isDownloaded() const
{
    return _isDownloaded;
}

void OPKit::setIsDownloaded(bool isDownloaded)
{
    _isDownloaded = isDownloaded;
}
