#include "OPRect.hpp"

OPRect::OPRect(float xMin, float xMax, float yMin, float yMax) :
    xMin(xMin),
    xMax(xMax),
    yMin(yMin),
    yMax(yMax) {}

float OPRect::getHeight() const {
    return yMax - yMin;
}

float OPRect::getWidth() const {
    return xMax - xMin;
}
