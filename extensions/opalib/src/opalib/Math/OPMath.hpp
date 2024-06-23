#ifndef OPALIBTEMPLATE_OPMATH_HPP
#define OPALIBTEMPLATE_OPMATH_HPP
class OPMath {
public:
    template<typename T>
    static constexpr T lerp(T x, T y, float t) {
        return (x + (y - x) * t);
    }
    template<typename T>
    static float inv_lerp(T x, T y, T value) {
        if (x < 0) {
            y -= x;
            value -= x;
            x = 0;
        }
        return (value - x) / (y - x);
    }

    static bool approximately(float a, float b);
    
    template<typename T>
    static T clamp(T value, T min, T max) {
        if (value > max) {
            return max;
        }
        if (min > value) {
            return min;
        }
        return value;
    }
    
    static float smoothstep(float edge0, float edge1, float x) {
      x = clamp<float>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      return x * x * (3 - 2 * x);
    }

    template<typename T>
    static T clamp01(T value) {
        return clamp<T>(value, 0, 1);
    }
};
#endif
