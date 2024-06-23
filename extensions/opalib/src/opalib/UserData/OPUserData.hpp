//
//  OPUserData.hpp
//  Opalib
//
//  Created by Adilson Tavares on 17/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPUserData_hpp
#define OPUserData_hpp

#include <iostream>
#include <map>
#include <json/document-wrapper.h>

struct OPUserDataItem;

class OPUserData
{
private:
    
    rapidjson::Document document;
    
    std::map<std::string, OPUserDataItem> items;
    
    void onSetValue(bool autoSave);
    
    ~OPUserData();
    
public:
    
    static OPUserData* shared();
    
    void printItemsForDebug();
    
    bool hasKey(const std::string& name);
    void remove(const std::string& name);
    
    void setBool(const std::string& name, bool value, bool autoSave = true);
    void setInt(const std::string& name, int value, bool autoSave = true);
    void setFloat(const std::string& name, float value, bool autoSave = true);
    void setInt64(const std::string& name, int64_t value, bool autoSave = true);
    void setString(const std::string& name, const std::string& value, bool autoSave = true);
    
    bool getBool(const std::string& name, bool defaultValue = false);
    int getInt(const std::string& name, int defaultValue = 0);
    float getFloat(const std::string& name, float defaultValue = 0.0);
    int64_t getInt64(const std::string& name, int64_t defaultValue = 0);
    std::string getString(const std::string& name, const std::string& defaultValue = "");
    
    std::string getSavePath();

    std::string getContentAsJson();

    void initialize();
    void restore();
    void restore(const std::string& jsonContent);
    void save();
    void clear();
};

#endif /* OPUserData_hpp */
