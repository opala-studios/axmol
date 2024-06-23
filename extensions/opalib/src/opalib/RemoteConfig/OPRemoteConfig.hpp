#ifndef OPRemoteConfig_hpp
#define OPRemoteConfig_hpp

#include <cocos2d.h>

enum OPFetchStatus
{
    None,
    Initializing,
    Initialized,
    FailedToInitialize
};

class OPRemoteConfig
{
public:

    typedef std::function<void(bool success)> SEL_FetchHandler;

protected:

    virtual void setup(const std::string& defaultsPath) = 0;
    virtual void _requestFetch() = 0;

    OPFetchStatus _fetchStatus = None;
    SEL_FetchHandler fetchCompletion {0};

public:

    static OPRemoteConfig* shared();

    void fetchWasCompleted(bool success);

    void initialize();
    void requestFetch(const SEL_FetchHandler& handler);

    OPFetchStatus getFetchStatus() const;

    virtual bool getBool(const std::string& name) = 0;
    virtual long getLong(const std::string& name) = 0;
    virtual double getDouble(const std::string& name) = 0;
    virtual std::string getString(const std::string& name) = 0;

    virtual int getInt(const std::string& name) { return (int)getLong(name); }
    virtual float getFloat(const std::string& name) { return getDouble(name); }
};

namespace opalib {
    namespace events {
        struct OnRemoteConfigFetched {
            bool success;
        };
    }
}

#endif /* OPRemoteConfig_hpp */
