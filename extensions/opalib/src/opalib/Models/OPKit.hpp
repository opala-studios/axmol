//
// Created by Talles Borges  on 24/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_OPKIT_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_OPKIT_HPP_

#include <cocos2d.h>

#include <string>

class OPKit : public cocos2d::Ref
{
public:
    CREATE_FUNC(OPKit);

    bool init() { return true; }

    const std::string& getId() const;
    void setId(const std::string& id);

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getAuthorName() const;
    void setAuthorName(const std::string& authorName);

    const std::string& getMusicName() const;
    void setMusicName(const std::string& musicName);

    bool isDownloaded() const;
    void setIsDownloaded(bool isDownloaded);

    bool isDeleted() const;
    void setIsDeleted(bool isDeleted);

protected:
    std::string _id;
    std::string _name;
    std::string _authorName;
    std::string _musicName;

    bool _isDownloaded;
    bool _isDeleted;
};

namespace opalib {
namespace Event {

struct KitSelected
{
    OPKit* kit;
    std::string categoryName;
    std::string location;
};

struct OpenKit
{
    OPKit* kit;
};

} // namespace Event
} // namespace opalib

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_OPKIT_HPP_
