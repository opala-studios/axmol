//
// Created by Talles Borges  on 29/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPAPPMANAGER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPAPPMANAGER_HPP_

#include "../API/OPAPI.hpp"
#include "../Resolvers/OPPathResolver.hpp"
#include "OPDBManager.hpp"

namespace opalib {
namespace Event {
struct DbUpdated{};
struct OnAppReady{};
} // namespace Event
} // namespace opalib

//TODO: Why this class is a cocos2d::Ref, is it an UI component ?
class OPAppManager : public cocos2d::Ref
{
public:
    static OPAppManager* shared() { return _sharedInstance; };

    virtual bool init();

    // API
    virtual OPAPI* getApi() const;
    virtual void updateKits();

    // DB
    virtual OPDBManager* getDatabase() const;

    // Path Resolver
    virtual OPPathResolver* getPathResolver() const;

    // Parers
    virtual OPFetchResponseJSONParser* getFetchParser() const;

    virtual cocos2d::Scene* getGDPRScene() = 0;
    virtual cocos2d::Scene* getIDFAScene() = 0;
    virtual cocos2d::Scene* getInitialScene() = 0;
    virtual cocos2d::Scene* getSplashScene() = 0;
    virtual cocos2d::Scene* getOpalaSplashScene();

    bool wasGDPRAlreadyShowed();
    void setUserConsent(bool consent);
    bool userConsent();
    bool shouldCheckGDPRRequirement();
    void setShouldCheckGDPRRequirement(bool shouldRequest);
    void setGDPRRequired(bool value);
    bool isGDPRRequired();

protected:
    static OPAppManager* _sharedInstance;
    // managers
    std::unique_ptr<OPAPI> _api;
    OPDBManager* _database;
    OPPathResolver* _pathResolver;

    std::string UserConsentId;
    std::string RequestGDPRId;

    // Parsers
    std::unique_ptr<OPFetchResponseJSONParser> _fetchParser;

    virtual void initializeAPI() = 0;
    virtual void initializeDB() = 0;
    virtual void initializePathResolver() = 0;
    virtual void initializeFetchParser() = 0;

    virtual void updateDatabase(OPFetchResponse* fetchResponse);
    virtual void saveDatabase();
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPAPPMANAGER_HPP_
