//
// Created by Ricardo on 24/11/20.
//

#ifndef OP_DIRECTORVALIDATOR_H
#define OP_DIRECTORVALIDATOR_H

#include <cocos2d.h>

namespace opalib {

    class Validator {
    public:
        typedef std::function<void()> Callback;
        typedef std::function<bool()> Predicate;
    private:

        struct Invocation {
            Invocation(size_t id, Callback&& callback) : id(id), callback(std::move(callback)){}
            size_t id;
            Callback callback;
        };

    public:

        Validator(Predicate&& predicate) : _predicate(std::move(predicate)) {}
        virtual ~Validator() = default;

        void flush();
        size_t push(Validator::Callback&& callback);
        void erase(size_t id);

    private:
        Predicate _predicate;
        std::mutex _mutex;
        std::vector<Invocation> _invocations;
        size_t _invocationIdCounter = 1;
    };

}

#endif //OP_DIRECTORVALIDATOR_H
