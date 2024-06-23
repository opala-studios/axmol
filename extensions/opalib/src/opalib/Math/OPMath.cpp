#include <cmath>
#include <limits>
#include "OPMath.hpp"

bool OPMath::approximately(float a, float b)
{
    const float EPSILON = std::numeric_limits<float>::epsilon();
    return std::fabs(a - b) < EPSILON;
}
