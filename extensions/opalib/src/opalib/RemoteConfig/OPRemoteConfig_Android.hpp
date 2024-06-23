#ifndef OPRemoteConfig_Android_hpp
#define OPRemoteConfig_Android_hpp

#include "OPRemoteConfig.hpp"

class OPRemoteConfig_Android : public OPRemoteConfig
{
protected:

    void setup(const std::string& defaultsPath) override;
    void _requestFetch() override;

public:

    bool getBool(const std::string& name) override;
    long getLong(const std::string& name) override;
    double getDouble(const std::string& name) override;
    std::string getString(const std::string& name) override;
};

#endif /* OPRemoteConfig_Android_hpp */