#ifndef OPALIB_CINEMATICS_OPLAMBDAPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPLAMBDAPLAYABLE_HPP_
#include <Playables/OPPlayable.hpp>

class OPLambdaPlayable : public OPPlayable {
private:
    std::function<void(OPFrameTimes)> _lambda;
public:
    OPLambdaPlayable(std::function<void(OPFrameTimes)>  lambda);
    void evaluate(OPFrameTimes times) override;
};

#endif