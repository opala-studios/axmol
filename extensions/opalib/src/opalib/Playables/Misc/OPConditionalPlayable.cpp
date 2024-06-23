#include <Playables/Misc/OPConditionalPlayable.hpp>
#include <utility>
OPConditionalPlayable::OPConditionalPlayable(
    OPConditionalPlayable::OPPredicate predicate,
    const std::initializer_list<std::shared_ptr<OPPlayable>>& delegates
) :
    _predicate(std::move(predicate)),
    _delegates(delegates) {}
void OPConditionalPlayable::begin(OPFrameTimes times) {
    if (allowed()) {
        for (std::shared_ptr<OPPlayable>& delegate : _delegates) {
            delegate->begin(times);
        }
    }
}
void OPConditionalPlayable::end(OPFrameTimes times) {
    if (allowed()) {
        for (std::shared_ptr<OPPlayable>& delegate : _delegates) {
            delegate->end(times);
        }
    }
}
void OPConditionalPlayable::evaluate(OPFrameTimes times) {
    if (allowed()) {
        for (std::shared_ptr<OPPlayable>& delegate : _delegates) {
            delegate->evaluate(times);
        }
    }
}
bool OPConditionalPlayable::allowed() {
    return _predicate();
}
