#include <cocos2d.h>

#include "OPCurve.hpp"

float findFloatBezierPosition(float p, float tangent, float magnitude, float dx) {
    return p + std::tan(tangent) * magnitude * dx;
}
float cubicFloats(
    float p0,
    float p3,
    float rightTangent,
    float rightTangentMagnitude,
    float leftTangent,
    float leftTangentMagnitude,
    float dx,
    float t
) {
    float p1 = findFloatBezierPosition(p0, rightTangent, rightTangentMagnitude, dx);
    float p2 = findFloatBezierPosition(p3, leftTangent, leftTangentMagnitude, dx);
    return OPCurve<float>::cubic(t, p0, p1, p2, p3);
}
template<>
float OPCurve<float>::cubic(float t, OPKeyframe first, OPKeyframe second) {

    float dx = second.time - first.time;
    return cubicFloats(
        first.value,
        second.value,
        first.rightTangent,
        first.rightTangentMagnitude,
        second.leftTangent,
        second.leftTangentMagnitude,
        dx, t
    );
}

template<>
cocos2d::Vec2 OPCurve<cocos2d::Vec2>::cubic(float t, OPKeyframe first, OPKeyframe second) {
    cocos2d::Vec2 p0 = first.value;
    cocos2d::Vec2 p3 = second.value;
    float dx = second.time - first.time;
    float p0x = p0.x;
    float p0y = p0.y;
    float p3x = p3.x;
    float p3y = p3.y;
    float x = cubicFloats(
        p0x, p3x,
        first.rightTangent,
        first.rightTangentMagnitude,
        second.leftTangent,
        second.leftTangentMagnitude,
        dx, t
    );
    float y = cubicFloats(
        p0y, p3y,
        first.rightTangent,
        first.rightTangentMagnitude,
        second.leftTangent,
        second.leftTangentMagnitude,
        dx, t
    );
    return cocos2d::Vec2(x, y);

}
