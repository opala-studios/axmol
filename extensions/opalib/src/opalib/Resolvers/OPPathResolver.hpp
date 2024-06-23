//
// Created by Talles Borges  on 29/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_RESOLVERS_OPPATHRESOLVER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_RESOLVERS_OPPATHRESOLVER_HPP_

#include <cocos2d.h>

#include "../Models/OPKit.hpp"

class OPPathResolver : public cocos2d::Ref
{
public:
    virtual std::string getDBPath() = 0;
    virtual std::string getKitPath(OPKit* kit) = 0;

    // URLs
    // The base url: http://api.superpads.opalastudios.com
    virtual std::string getApiUrl() = 0;
    // Just the path: /api/fetch?&version=x&timstamp=1
    virtual std::string getFetchUrlPath(int64_t timestamp, int apiVersion);;
    virtual std::string getDownloadUrl(OPKit* kit) = 0;
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_RESOLVERS_OPPATHRESOLVER_HPP_
