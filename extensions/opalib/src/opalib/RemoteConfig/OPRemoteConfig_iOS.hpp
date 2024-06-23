#ifndef OPRemoteConfig_iOS_hpp
#define OPRemoteConfig_iOS_hpp

#include "./OPRemoteConfig.hpp"

class OPRemoteConfig_iOS : public OPRemoteConfig
{
protected:
    void* impl {0};
    virtual void setup(const std::string& defaultsPath) override;
    virtual void _requestFetch() override;
    
public:
    virtual bool getBool(const std::string& name) override;
    virtual long getLong(const std::string& name) override;
    virtual double getDouble(const std::string& name) override;
    virtual std::string getString(const std::string& name) override;
};

#endif
