//
// Created by Talles Borges  on 29/07/20.
//

#include "OPDBManager.hpp"

#include <JuceHeader.h>

#include "Debug/OPDebug.hpp"
#include "Helpers/OPString.hpp"
#include "Managers/OPAppManager.hpp"

bool OPDBManager::init()
{
    return true;
}

void OPDBManager::addOrUpdateKit(OPKit* kit)
{
    OPKit* dbKit = _kits.at(kit->getId());
    if (dbKit) {
        updateKit(dbKit, kit);
        return;
    }

    if (!kit->isDeleted())
        _kits.insert(kit->getId(), kit);
}

void OPDBManager::updateKit(OPKit* dbKit, OPKit* kit)
{
    dbKit->setName(kit->getName());
    dbKit->setAuthorName(kit->getAuthorName());
    dbKit->setMusicName(kit->getMusicName());
    dbKit->setIsDeleted(kit->isDeleted());
}

void OPDBManager::addOrUpdateCategory(OPCategory* category)
{
    OPCategory* dbCategory = _categories.at(category->getId());
    if (dbCategory) {
        dbCategory->setName(category->getName());
        dbCategory->setOrder(category->getOrder());
        dbCategory->setIsAcendingOrder(category->getIsAcendingOrder());
        dbCategory->setType(category->getType());
        dbCategory->setReferencedId(category->getReferencedId());
        dbCategory->setShowInFeed(category->getShowInFeed());
        dbCategory->setIsDeleted(category->getIsDeleted());
        dbCategory->setURLBackground(category->getURLBackground());
        dbCategory->setURLThumb(category->getURLThumb());
        return;
    }

    _categories.insert(category->getId(), category);
}

void OPDBManager::addOrUpdateGroupCategory(OPGroupCategory* groupCategory)
{
    OPCategory* group = _categories.at(groupCategory->getGroupId());
    OPCategory* category = _categories.at(groupCategory->getCategoryId());

    groupCategory->setCategory(category);
    groupCategory->setGroup(group);

    OPGroupCategory* dbGroupCategory = _groupCategories.at(groupCategory->getId());
    if (dbGroupCategory) {
        dbGroupCategory->setCategory(category);
        dbGroupCategory->setCategoryId(groupCategory->getCategoryId());
        dbGroupCategory->setGroup(group);
        dbGroupCategory->setGroupId(groupCategory->getGroupId());
        dbGroupCategory->setOrder(groupCategory->getOrder());
        dbGroupCategory->setIsDeleted(groupCategory->getIsDeleted());
        return;
    }

    _groupCategories.insert(groupCategory->getId(), groupCategory);
}

void OPDBManager::addOrUpdateKitCategory(OPKitCategory* kitCategory)
{
    OPKit* kit = _kits.at(kitCategory->getKitId());
    OPCategory* category = _categories.at(kitCategory->getCategoryId());

    kitCategory->setCategory(category);
    kitCategory->setKit(kit);

    OPKitCategory* dbKitCategory = _kitCategories.at(kitCategory->getId());
    if (dbKitCategory) {
        dbKitCategory->setCategory(category);
        dbKitCategory->setCategoryId(kitCategory->getCategoryId());
        dbKitCategory->setKit(kit);
        dbKitCategory->setKitId(kitCategory->getKitId());
        dbKitCategory->setOrder(kitCategory->getOrder());
        dbKitCategory->setIsDeleted(kitCategory->getIsDeleted());
        return;
    }

    _kitCategories.insert(kitCategory->getId(), kitCategory);
}

OPKit* OPDBManager::getKit(const std::string& id)
{
    return _kits.at(id);
}

cocos2d::Vector<OPKit*> OPDBManager::getKits() const
{
    cocos2d::Vector<OPKit*> kits;

    for (auto& pair : _kits) {
        if (!pair.second->isDeleted())
            kits.pushBack(pair.second);
    }

    return kits;
}

cocos2d::Vector<OPKit*> OPDBManager::getDownloadedKits() const
{
    auto kits = cocos2d::Vector<OPKit*>();
    for (auto& kit : _kits) {
        if (kit.second->isDownloaded()) {
            kits.pushBack(kit.second);
        }
    }

    auto pathResolver = OPAppManager::shared()->getPathResolver();

    std::sort(kits.begin(), kits.end(), [pathResolver](OPKit* a, OPKit* b) {
        auto fileA = juce::File(pathResolver->getKitPath(a));
        auto fileB = juce::File(pathResolver->getKitPath(b));
        return fileA.getCreationTime() > fileB.getCreationTime();
    });

    return kits;
}

cocos2d::Vector<OPKit*> OPDBManager::filter(const std::string& filter)
{
    if (filter.empty()) {
        return getKits();
    }

    auto filterKits = cocos2d::Vector<OPKit*>();
    auto lowerFilter = OPString::lowercased(filter);

    for (auto& pair : _kits) {
        auto kit = pair.second;
        auto kitName = OPString::lowercased(kit->getName());
        auto authorName = OPString::lowercased(kit->getAuthorName());
        auto musicTitle = OPString::lowercased(kit->getMusicName());

        if (kitName.find(lowerFilter) != std::string::npos || authorName.find(lowerFilter) != std::string::npos
            || musicTitle.find(lowerFilter) != std::string::npos) {
            filterKits.pushBack(kit);
        }
    }

    return filterKits;
}

cocos2d::Vector<OPKit*> OPDBManager::filterDownloaded(const std::string& filter)
{
    auto downloadedKits = getDownloadedKits();

    auto filterKits = cocos2d::Vector<OPKit*>();
    auto lowerFilter = OPString::lowercased(filter);

    for (auto& kit : downloadedKits) {
        auto kitName = OPString::lowercased(kit->getName());
        auto authorName = OPString::lowercased(kit->getAuthorName());
        auto musicTitle = OPString::lowercased(kit->getMusicName());

        if (kitName.find(lowerFilter) != std::string::npos || authorName.find(lowerFilter) != std::string::npos
            || musicTitle.find(lowerFilter) != std::string::npos) {
            filterKits.pushBack(kit);
        }
    }

    return filterKits;
}

OPCategory* OPDBManager::getCategory(const std::string& id) const
{
    return _categories.at(id);
}

cocos2d::Vector<OPKit*> OPDBManager::getKits(OPCategory* category) const
{
    auto kitCategories = cocos2d::Vector<OPKitCategory*>();

    for (const auto& pair : _kitCategories) {
        if (pair.second->getCategory() == category) {
            kitCategories.pushBack(pair.second);
        }
    }

    std::sort(kitCategories.begin(), kitCategories.end(), [category](OPKitCategory* a, OPKitCategory* b) {
        if (category->getIsAcendingOrder()) {
            return a->getOrder() < b->getOrder();
        } else {
            return a->getOrder() > b->getOrder();
        }
    });

    auto kits = cocos2d::Vector<OPKit*>();
    for (auto groupCategory : kitCategories) {
        if (groupCategory->getKit() != nullptr && !groupCategory->getKit()->isDeleted()) {
            kits.pushBack(groupCategory->getKit());
        }
    }

    return kits;
}

const cocos2d::Map<std::string, OPCategory*>& OPDBManager::getCategoriesTable() const
{
    return _categories;
}

const cocos2d::Map<std::string, OPGroupCategory*>& OPDBManager::getGroupCategoriesTable() const
{
    return _groupCategories;
}

const cocos2d::Map<std::string, OPKitCategory*>& OPDBManager::getKitCategoriesTable() const
{
    return _kitCategories;
}

const cocos2d::Map<std::string, OPKit*>& OPDBManager::getKitsTable() const
{
    return _kits;
}

cocos2d::Vector<OPCategory*> OPDBManager::getFeedCategories()
{
    cocos2d::Vector<OPCategory*> feedCategories;
    for (const auto& item : _categories) {
        if (item.second->getShowInFeed()) {
            feedCategories.pushBack(item.second);
        }
    }

    std::sort(feedCategories.begin(), feedCategories.end(), [](OPCategory* a, OPCategory* b) {
        return a->getOrder() < b->getOrder();
    });

    return feedCategories;
}

cocos2d::Vector<OPCategory*> OPDBManager::getCategories(OPCategory* group)
{
    auto groupCategories = cocos2d::Vector<OPGroupCategory*>();

    for (const auto& pair : _groupCategories) {
        if (pair.second->getGroup() == group) {
            if (!pair.second->getIsDeleted()) {
                groupCategories.pushBack(pair.second);
            }
        }
    }

    std::sort(groupCategories.begin(), groupCategories.end(), [group](OPGroupCategory* a, OPGroupCategory* b) {
        if (group->getIsAcendingOrder()) {
            return a->getOrder() < b->getOrder();
        } else {
            return a->getOrder() > b->getOrder();
        }
    });

    auto categories = cocos2d::Vector<OPCategory*>();
    for (auto groupCategory : groupCategories) {
        categories.pushBack(groupCategory->getCategory());
    }

    return categories;
}

int64_t OPDBManager::getFetchTimestamp() const
{
    return _fetchTimestamp;
}

void OPDBManager::setFetchTimestamp(int64_t fetchTimestamp)
{
    _fetchTimestamp = fetchTimestamp;
}

void OPDBManager::updateWithResponse(OPFetchResponse* fetchResponse)
{
    OP_LOG_VERB("BEGIN");
    // setup kits
    for (auto& pair : fetchResponse->kits) {
        addOrUpdateKit(pair.second);
    }

    // setup categories
    for (auto& pair : fetchResponse->categories) {
        addOrUpdateCategory(pair.second);
    }

    // setup categoryKits
    for (auto& kitCategory : fetchResponse->kitCategories) {
        addOrUpdateKitCategory(kitCategory.second);
    }

    // setup groupCategories
    for (auto& groupCategory : fetchResponse->groupCategories) {
        addOrUpdateGroupCategory(groupCategory.second);
    }

    setFetchTimestamp(fetchResponse->timestamp);
    OP_LOG_VERB("END");
}

void OPDBManager::save(OPFetchResponse& obj)
{
    obj.kits = getKitsTable();
    obj.categories = getCategoriesTable();
    obj.groupCategories = getGroupCategoriesTable();
    obj.kitCategories = getKitCategoriesTable();
    obj.timestamp = getFetchTimestamp();
}
