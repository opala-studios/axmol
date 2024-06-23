#ifndef OPALIBTEMPLATE_OPRECT_HPP
#define OPALIBTEMPLATE_OPRECT_HPP

class OPRect {
public:
    float xMin, xMax, yMin, yMax;
    OPRect() = default;
    OPRect(
        float xMin,
        float xMax,
        float yMin,
        float yMax
    );
    float getHeight() const;
    float getWidth() const;
};

#endif