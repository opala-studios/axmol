//
//  OPEvent.hpp
//  OpalaApp
//
//  Created by Adilson Tavares on 01/08/19.
//

#ifndef OPEvent_h
#define OPEvent_h

#include <iostream>
#include <vector>
#include <algorithm>

template<typename... T>
class OPEvent
{
private:
    
    std::vector<std::function<void(T...)>> callbacks;
    
    size_t getCallbackAddress(std::function<void(T...)> f)
    {
        typedef void(fnType)(T...);
        fnType ** fnPointer = f.template target<fnType*>();
        
        if (!fnPointer) {
            return 0;
        }
        
        return (size_t) *fnPointer;
    }
    
    bool callbacksAreEqual(std::function<void(T...)> a, std::function<void(T...)> b)
    {
        return getCallbackAddress(a) == getCallbackAddress(b);
    }
    
    void clearInvalidCallbacks()
    {
//        if (callbacks.size() == 0) {
//            return;
//        }
//        
//        auto it = callbacks.begin();
//        
//        while (it != callbacks.end())
//        {
//            if (!getCallbackAddress(*it)) {
//                callbacks.erase(it);
//            }
//            else {
//                it++;
//            }
//        }
    }
    
public:
    
    std::size_t size()
    {
        return callbacks.size();
    }
    
    void add(const std::function<void(T...)> callback) {
        callbacks.push_back(callback);
    }
    
    void remove(std::function<void(T...)> callback)
    {
        for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
        {
            if (callbacksAreEqual(*it, callback))
            {
                callbacks.erase(it);
                return;
            }
        }
    }
    
    void clear()
    {
        callbacks.clear();
    }
    
    void invoke(T... params)
    {
        clearInvalidCallbacks();
        
        for (std::function<void(T...)> callback : callbacks) {
            callback(params...);
        }
    }
};

typedef OPEvent<> OPEventVoid;

#endif /* OPEvent_h */
