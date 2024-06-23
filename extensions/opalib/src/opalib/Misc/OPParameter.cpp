//
//  OPParameter.cpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPParameter.hpp"
#include "../Debug/OPDebug.hpp"

OPParameter::OPParameter()
{
    type = UNKNOWN;
}

void OPParameter::copyFrom(const OPParameter& other)
{
    switch (other.type)
    {
        case Type::UNKNOWN:
            break;
            
        case Type::BOOL:
            setBool(other.value.boolValue);
            break;
            
        case Type::INT:
            setInt(other.value.intValue);
            break;
            
        case Type::FLOAT:
            setFloat(other.value.floatValue);
            break;
            
        case Type::DOUBLE:
            setDouble(other.value.doubleValue);
            break;
            
        case Type::STRING:
            setString(other.value.stringValue, other.stringLength);
            break;
            
        default:
            // Copy constructor for this type (other.type) is not implemented.
            CC_ASSERT(false);
            break;
    }
}

OPParameter::OPParameter(const OPParameter& other)
{
    copyFrom(other);
}

OPParameter::OPParameter(OPParameter&& other)
{
    copyFrom(other);
}

OPParameter& OPParameter::operator=(OPParameter&& other)
{
    copyFrom(other);
    return *this;
}

OPParameter::OPParameter(bool boolValue)
{
    setBool(boolValue);
}

OPParameter::OPParameter(int intValue)
{
    setInt(intValue);
}

OPParameter::OPParameter(float floatValue)
{
    setFloat(floatValue);
}

OPParameter::OPParameter(double doubleValue)
{
    setDouble(doubleValue);
}

OPParameter::OPParameter(const char* stringValue)
{
    setString(stringValue);
}

OPParameter::OPParameter(const char* stringValue, size_t size)
{
    setString(stringValue, size);
}

OPParameter::OPParameter(const std::string& stringValue)
{
    setString(stringValue.c_str(), stringValue.size());
}

void OPParameter::setBool(bool boolValue)
{
    changeValueAssert();
    
    type = Type::BOOL;
    value.boolValue = boolValue;
}

void OPParameter::setInt(int intValue)
{
    changeValueAssert();
    
    type = Type::INT;
    value.intValue = intValue;
}

void OPParameter::setFloat(float floatValue)
{
    changeValueAssert();
    
    type = Type::FLOAT;
    value.doubleValue = floatValue;
}

void OPParameter::setDouble(double doubleValue)
{
    changeValueAssert();
    
    type = Type::DOUBLE;
    value.doubleValue = doubleValue;
}

void OPParameter::setString(const char *stringValue)
{
    setString(stringValue, strlen(stringValue));
}

void OPParameter::setString(const char *stringValue, size_t size)
{
    changeValueAssert();
    
    if (type == Type::STRING) {
        delete value.stringValue;
    }
    else {
        type = Type::STRING;
    }
    
    value.stringValue = new char[size + 1];
    memcpy(value.stringValue, stringValue, size);
    value.stringValue[size] = 0;
    
    stringLength = size;
}

void OPParameter::setString(const std::string &stringValue)
{
    setString(stringValue.c_str(), stringValue.size());
}

void OPParameter::changeValueAssert()
{
    OPDebug::ensure(type == UNKNOWN, "Could not set a value of parameter more than once.");
}

OPParameter::~OPParameter()
{
    if (type == Type::STRING) {
        delete value.stringValue;
    }
}
