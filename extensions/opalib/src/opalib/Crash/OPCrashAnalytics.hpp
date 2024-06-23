#ifndef OPCrashAnalytics_hpp
#define OPCrashAnalytics_hpp

#include <cocos2d.h>

class OPCrashAnalytics
{
protected:
    
    virtual void setup() = 0;
    
public:
    
    static OPCrashAnalytics* shared();

    void verb(const std::string& context, const std::string& message);
    void log(const std::string& context, const std::string& message);
    void warn(const std::string& context, const std::string& message);
    void error(const std::string& context, const std::string& message);
    void recordException(const std::string& context, const std::exception& e);

    virtual void verb(const std::string& message) = 0;
    virtual void log(const std::string& message) = 0;
    virtual void warn(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
    virtual void recordException(const std::string& message) = 0;
};

#endif