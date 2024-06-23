//
//  OPCocosThread.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 01/07/19.
//

#include "../Crash/OPCrashAnalytics.hpp"
#include "OPCocosThread.hpp"
#include "../Debug/OPDebug.hpp"

OPCocosThread* OPCocosThread::getInstance()
{
    static OPCocosThread *instance = new OPCocosThread();
    return instance;
}

void OPCocosThread::initialize()
{
    auto instance = getInstance();
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(instance, 0, false);
}

void OPCocosThread::update(float dt)
{
    _mtx.lock();
    
    std::vector<std::pair<std::string, OPCocosThread::SEL_CallbackHandler>> callbacks;
    
    for (int i = 0; i < _invokations.size(); ++i)
    {
        auto invokation = _invokations[i];
        
        if (invokation->delay <= 0)
        {

//            OPDebug::log("OPCocosThread", "Calling invokation " + invokation->name);

            callbacks.emplace_back(invokation->name, invokation->callback);
            _invokations.erase(_invokations.begin() + i);
            
            i--;
        }
        else {
            invokation->delay -= dt;
        }
    }
    
    _mtx.unlock();
    
    for (auto& callback : callbacks)
    {
        try
        {
            callback.second();
        } catch (const std::exception& e)
        {
            OPCrashAnalytics::shared()->recordException("Opalib - CocosThread - " + callback.first, e);
        }
    }
}

long OPCocosThread::invoke(std::string name, SEL_CallbackHandler callback)
{
    return invoke(name, 0, callback);
}

long OPCocosThread::invoke(std::string name, float delay, SEL_CallbackHandler callback)
{
//    OPDebug::log("OPCocosThread", "Invoke " + name + " after " + std::to_string(delay) + "s");

    auto instance = getInstance();
    instance->_mtx.lock();
    
    auto invokation = std::make_shared<Invokation>();
    invokation->name = name;
    invokation->delay = delay;
    invokation->callback = callback;
    
    invokation->id = instance->_currentId;
    instance->_currentId++;
    
    instance->_invokations.push_back(invokation);
    instance->_mtx.unlock();
    
    return invokation->id;
}

bool OPCocosThread::cancelInvoke(long id)
{
    if (id <= 0) {
        return false;
    }

    auto instance = getInstance();
    instance->_mtx.lock();
    
    auto it = std::find_if(instance->_invokations.begin(), instance->_invokations.end(), [&](const std::shared_ptr<Invokation>& x) { return x->id == id; });
    
    if (it == instance->_invokations.end())
    {
        instance->_mtx.unlock();
        return false;
    }

//    OPDebug::log("OPCocosThread", "Cancel invoke " + (*it)->name);
    
    instance->_invokations.erase(it);
    instance->_mtx.unlock();
    
    return true;
}
