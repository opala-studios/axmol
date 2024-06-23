//
// Created by Talles Borges  on 29/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPDBMANAGER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPDBMANAGER_HPP_

#include <cocos2d.h>

#include "../Models/OPCategory.hpp"
#include "../Models/OPGroupCategory.hpp"
#include "../Models/OPKit.hpp"
#include "../Models/OPKitCategory.hpp"
#include "../Models/Responses/OPFetchResponse.hpp"

class OPDBManager : public cocos2d::Ref
{
public:
    CREATE_FUNC(OPDBManager);

    virtual bool init();

    // Kits methods
    virtual void addOrUpdateKit(OPKit* kit);
    OPKit* getKit(const std::string& id);
    cocos2d::Vector<OPKit*> getKits() const;
    cocos2d::Vector<OPKit*> getKits(OPCategory* category) const;
    cocos2d::Vector<OPKit*> getDownloadedKits() const;

    cocos2d::Vector<OPKit*> filter(const std::string& filter);
    cocos2d::Vector<OPKit*> filterDownloaded(const std::string& filter);

    // Category
    virtual void addOrUpdateCategory(OPCategory* category);
    OPCategory* getCategory(const std::string& id) const;
    cocos2d::Vector<OPCategory*> getFeedCategories();
    cocos2d::Vector<OPCategory*> getCategories(OPCategory* group);

    // group category
    virtual void addOrUpdateGroupCategory(OPGroupCategory* groupCategory);

    // kit category
    virtual void addOrUpdateKitCategory(OPKitCategory* kitCategory);

    // misc
    const cocos2d::Map<std::string, OPKit*>& getKitsTable() const;
    const cocos2d::Map<std::string, OPCategory*>& getCategoriesTable() const;
    const cocos2d::Map<std::string, OPGroupCategory*>& getGroupCategoriesTable() const;
    const cocos2d::Map<std::string, OPKitCategory*>& getKitCategoriesTable() const;

    // timestamp
    int64_t getFetchTimestamp() const;
    void setFetchTimestamp(int64_t fetchTimestamp);

    // misc
    virtual void updateWithResponse(OPFetchResponse* fetchResponse);
    
    virtual void save(OPFetchResponse& obj);

protected:
    int64_t _fetchTimestamp {0};
    cocos2d::Map<std::string, OPKit*> _kits;
    cocos2d::Map<std::string, OPCategory*> _categories;
    cocos2d::Map<std::string, OPGroupCategory*> _groupCategories;
    cocos2d::Map<std::string, OPKitCategory*> _kitCategories;

    virtual void updateKit(OPKit* kit, OPKit* withKit);
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_MANAGERS_OPDBMANAGER_HPP_
