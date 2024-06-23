//
//  OPPool.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 25/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPPool_hpp
#define OPPool_hpp

#include <queue>

template<typename T, typename ...Args>
class OPCocosRefPool
{
public:
    //removi o && por conta de um problema com ponteiros
    //https://stackoverflow.com/questions/38723515/c11-stdforward-a-pointer
    OPCocosRefPool(size_t size, Args ...args)
    {
        _size = size;
        for (int i = 0; i < size; ++i)
        {
            auto element = std::remove_pointer<T>::type::create(std::forward<Args>(args)...);
            element->retain();
            _queue.push(element);
        }
    }

    ~OPCocosRefPool()
    {
        while(!_queue.empty())
        {
            T element = _queue.front();
            _queue.pop();
            element->release();
        }
        _size = 0;
    }
    
    size_t size()
    {
        return _size;
    }

    T spawn()
    {
        T element = _queue.front();
        _queue.pop();
        _queue.push(element);
        return element;
    }

private:
    size_t _size = 0;
    std::queue<T> _queue;
};


#endif /* OPPool_hpp */
