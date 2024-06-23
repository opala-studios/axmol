//
// Created by Talles Borges  on 29/07/20.
//

#include <opalib/Debug/OPDebug.hpp>
#include <opalib/UserData/OPUserData.hpp>
#include <opalib/Ads/OPAds.hpp>
#include <opalib/Adjust/OPAdjust.hpp>
#include <opalib/Scene/OPSplashScene.hpp>
#include "OPAppManager.hpp"

#include "../Misc/OPEventBus.hpp"

OPAppManager* OPAppManager::_sharedInstance = nullptr;

bool OPAppManager::init()
{
    initializeFetchParser();
    initializePathResolver();
    initializeDB();
    initializeAPI();

    return true;
}

void OPAppManager::updateKits()
{
    _api->fetchKits(_database->getFetchTimestamp(), [this](OPFetchResponse* fetchResponse) {
        OP_LOG_VERB("BEGIN - fetchKits callback");
        if (fetchResponse == nullptr){
            OP_LOG_VERB("END - fetchResponse was nullptr");
            return;
        }
        if (fetchResponse->timestamp != _database->getFetchTimestamp()) {
            OP_LOG_VERB("fetchResponse->timestamp != _database->getFetchTimestamp()");
            updateDatabase(fetchResponse);
            OPEventBus::shared()->notify(opalib::Event::DbUpdated());
            saveDatabase();
        }
        OP_LOG_VERB("END - fetchKits callback");
    });
}
void OPAppManager::updateDatabase(OPFetchResponse* fetchResponse)
{
    _database->updateWithResponse(fetchResponse);
}

OPAPI* OPAppManager::getApi() const
{
    return _api.get();
}

OPDBManager* OPAppManager::getDatabase() const
{
    return _database;
}

OPPathResolver* OPAppManager::getPathResolver() const
{
    return _pathResolver;
}

void OPAppManager::saveDatabase()
{
	OP_LOG_VERB("BEGIN");
    auto obj = _fetchParser->createObject();
    _database->save(*obj);

    rapidjson::Document document = OPJson::document();
    _fetchParser->toJson(obj, document);
    OPJson::save(document, _pathResolver->getDBPath());
    OP_LOG_VERB("END");
}

OPFetchResponseJSONParser* OPAppManager::getFetchParser() const
{
    return _fetchParser.get();
}

bool OPAppManager::userConsent() {
    return OPUserData::shared()->getBool(UserConsentId, true);
}

bool OPAppManager::wasGDPRAlreadyShowed() {
    return OPUserData::shared()->hasKey(UserConsentId);
}

void OPAppManager::setUserConsent(bool consent) {
    OPUserData::shared()->setBool(UserConsentId, consent);
    OPAds::shared()->initialize(consent);
//    OPAdjust::shared()->initialize(consent);
}

bool OPAppManager::shouldCheckGDPRRequirement() {
    return OPUserData::shared()->getBool(RequestGDPRId, true);
}

void OPAppManager::setShouldCheckGDPRRequirement(bool shouldRequest) {
    OPUserData::shared()->setBool(RequestGDPRId, shouldRequest);
}

bool OPAppManager::isGDPRRequired() {
    return OPUserData::shared()->getBool("is_gdpr_required");
}

void OPAppManager::setGDPRRequired(bool value) {
    OPUserData::shared()->setBool("is_gdpr_required", value);
}

cocos2d::Scene *OPAppManager::getOpalaSplashScene() {
    auto splashParams = std::unique_ptr<opalib::OPSplashSceneParams>(new opalib::OPSplashSceneParams());
    splashParams->packagePath = "opalib/fairy/";
    splashParams->packageName = "OpalaSplash";
    return opalib::OPSplashScene::create(std::move(splashParams));
}

