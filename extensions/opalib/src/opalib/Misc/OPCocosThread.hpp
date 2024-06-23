//
//  OPCocosThread.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 01/07/19.
//

#ifndef OPCocosThread_hpp
#define OPCocosThread_hpp

#include <stdio.h>
#include <vector>
#include <mutex>
#include <cocos2d.h>

class OPCocosThread
{
public: typedef std::function<void()> SEL_CallbackHandler;
    
private:
    
    struct Invokation
    {
        std::string name;
        long id;
        float delay;
        SEL_CallbackHandler callback;
    };
    
    long _currentId {1};
    
    std::mutex _mtx;
    std::vector<std::shared_ptr<Invokation>> _invokations;
    
    static OPCocosThread *getInstance();
    
public:
    
    static void initialize();
    void update(float dt);
    
    static long invoke(std::string name, SEL_CallbackHandler callback);
    static long invoke(std::string name, float delay, SEL_CallbackHandler callback);
    static bool cancelInvoke(long id);
};

//Warning: this does not make it safe to schedule on another thread
//Another thread could be scheduling at the same time that the main thread
//is destroying this object, and there is no synchronization workaround that can solve this
class OPSafeScheduler
{
public:
    void schedule(const std::string& name, float delay, const std::function<void()>& callback)
    {
        _invocations.insert(OPCocosThread::invoke(name, delay, callback));
    }
    
    void schedule(const std::string& name, const std::function<void()>& callback)
    {
        _invocations.insert(OPCocosThread::invoke(name, callback));
    }
    
    virtual ~OPSafeScheduler() {
        for (auto id : _invocations) {
            OPCocosThread::cancelInvoke(id);
        }
    }

protected:
    std::set<long> _invocations;
    
};

#endif /* OPCocosThread_hpp */
