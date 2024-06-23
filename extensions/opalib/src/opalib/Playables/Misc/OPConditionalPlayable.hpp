//
// Created by bruno on 23/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPCONDITIONALPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPCONDITIONALPLAYABLE_HPP_

#include <Playables/OPPlayable.hpp>

class OPConditionalPlayable : public OPPlayable {
public:
    typedef std::function<bool()> OPPredicate;
private:
    OPPredicate _predicate;
    std::vector<std::shared_ptr<OPPlayable>> _delegates;
    bool allowed();
public:
    OPConditionalPlayable(
        OPPredicate predicate,
        const std::initializer_list<std::shared_ptr<OPPlayable>>& delegates = {}
    );
    void begin(OPFrameTimes times) override;
    void end(OPFrameTimes times) override;
    void evaluate(OPFrameTimes times) override;
    template<typename T, typename ...Args>
    std::shared_ptr<T> addDelegate(Args&& ...args) {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        _delegates.push_back(ptr);
        return ptr;
    }
};

#endif //OPALIB_CINEMATICS_OPCONDITIONALPLAYABLE_HPP_
