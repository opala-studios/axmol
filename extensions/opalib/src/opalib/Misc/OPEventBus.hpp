//
// Created by Talles Borges  on 2020-04-03.
//

#ifndef OPALIBTEMPLATE_CLASSES_UTILS_OPEVENTBUS_HPP_
#define OPALIBTEMPLATE_CLASSES_UTILS_OPEVENTBUS_HPP_

#include "eventbus/EventBus.h"
#include "eventbus/TokenHolder.h"

class OPEventBus
{
public:
    static std::shared_ptr<Dexode::EventBus> shared()
    {
        static OPEventBus _sharedInstance;
        return _sharedInstance._eventBus;
    }

private:
    OPEventBus() { _eventBus = std::make_shared<Dexode::EventBus>(); };
    std::shared_ptr<Dexode::EventBus> _eventBus;
};

#endif // OPALIBTEMPLATE_CLASSES_UTILS_OPEVENTBUS_HPP_
