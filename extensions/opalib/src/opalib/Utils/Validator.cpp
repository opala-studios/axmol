//
// Created by Ricardo on 24/11/20.
//

#include "Validator.h"


namespace opalib {

    size_t Validator::push(Validator::Callback&& callback) {
        std::lock_guard<std::mutex> lockGuard(_mutex);
        if (_predicate()){
            callback();
            return 0;
        }
        size_t id = _invocationIdCounter++;
        _invocations.emplace_back(id, std::move(callback));
        return id;
    }

    void Validator::erase(size_t id) {
        std::lock_guard<std::mutex> lockGuard(_mutex);
        auto it = std::find_if(_invocations.begin(), _invocations.end(), [id](const Invocation& invocation){
            return invocation.id == id;
        });
        if (it == _invocations.end()){
            return;
        }
        _invocations.erase(it);
    }

    void Validator::flush() {
        std::lock_guard<std::mutex> lockGuard(_mutex);
        for (auto& invocation : _invocations){
            invocation.callback();
        }
        _invocations.clear();
    }
}