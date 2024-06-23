#ifndef OPCrashAnalytics_Android_hpp
#define OPCrashAnalytics_Android_hpp

#include "./OPCrashAnalytics.hpp"

class OPCrashAnalytics_Android : public OPCrashAnalytics
{
protected:

    virtual void setup() override;

public:
    void verb(const std::string& message) override;
    void log(const std::string& message) override;
    void warn(const std::string& message) override;
    void error(const std::string& message) override;
    void recordException(const std::string &message) override;
};

#endif
