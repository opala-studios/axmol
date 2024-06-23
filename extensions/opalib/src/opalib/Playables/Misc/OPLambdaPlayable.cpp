#include <Playables/Misc/OPLambdaPlayable.hpp>
#include <utility>
OPLambdaPlayable::OPLambdaPlayable(std::function<void(OPFrameTimes)> lambda) :
    _lambda(std::move(lambda)) {}

void OPLambdaPlayable::evaluate(OPFrameTimes times) {
    _lambda(times);
}
