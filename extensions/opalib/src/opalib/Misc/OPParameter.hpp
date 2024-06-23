//
//  OPParameter.hpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPParameter_hpp
#define OPParameter_hpp

#include <iostream>

class OPParameter
{
public:
    
    union Value
    {
        int intValue;
        bool boolValue;
        float floatValue;
        double doubleValue;
        char* stringValue;
    };
    
    enum Type
    {
        UNKNOWN,
        BOOL,
        INT,
        FLOAT,
        DOUBLE,
        STRING
    };
    
private:
    
    Type type {UNKNOWN};
    Value value {0};
    
    size_t stringLength {0};
    
    void changeValueAssert();
    
public:
    
    OPParameter();
    OPParameter(const OPParameter& other);
    OPParameter(OPParameter&& other);
    
    OPParameter& operator=(OPParameter&& other);
    
    ~OPParameter();
    
    OPParameter(bool boolValue);
    OPParameter(int intValue);
    OPParameter(float floatValue);
    OPParameter(double doubleValue);
    OPParameter(const char* stringValue);
    OPParameter(const char* stringValue, size_t size);
    OPParameter(const std::string& stringValue);
    
    Type getType() { return type; }
    
    void setBool(bool boolValue);
    void setInt(int intValue);
    void setFloat(float floatValue);
    void setDouble(double doubleValue);
    void setString(const char* stringValue);
    void setString(const char* stringValue, size_t size);
    void setString(const std::string& stringValue);
    
    bool getBool() { return value.boolValue; }
    int getInt() { return value.intValue; }
    float getFloat() { return value.floatValue; }
    double getDouble() { return value.doubleValue; }
    char* getCString() { return value.stringValue; }
    std::string getString() { return std::string(value.stringValue, stringLength); }
    
protected:
    
    void copyFrom(const OPParameter& other);
};

#endif /* OPParameter_hpp */
