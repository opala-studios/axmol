#ifndef OPCrashAnalytics_iOS_hpp
#define OPCrashAnalytics_iOS_hpp

#include "OPCrashAnalytics.hpp"
#include "../../boost/include/boost/algorithm/string.hpp"

class OPCrashAnalytics_iOS : public OPCrashAnalytics
{
protected:
        
    void* impl {0};
    
    void setup() override;
    
public:

    void verb(const std::string& message) override;
    void log(const std::string& message) override;
    void warn(const std::string& message) override;
    void error(const std::string& message) override;
    void recordException(const std::string &message) override;
};

#endif
