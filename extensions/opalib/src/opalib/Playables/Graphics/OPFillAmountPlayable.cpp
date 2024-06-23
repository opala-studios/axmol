//
// Created by bruno on 17/06/2021.
//

#include "OPFillAmountPlayable.hpp"
void OPFillAmountPlayable::evaluate(OPFrameTimes times) {
    float p;
    if (_fillAmountCurve.isDefined()) {
        p = _fillAmountCurve.evaluate(times.progress);
    } else {
        p = times.progress;
    }
    _image->setFillAmount(p);
}
OPFillAmountPlayable::OPFillAmountPlayable(
    fairygui::GImage* image,
    const OPCurve<float>& fillAmountCurve
) :
    _image(image),
    _fillAmountCurve(fillAmountCurve) {
    if (image == nullptr) {
        throw std::runtime_error("It's not possible to create an OPFillAmountPlayable with a null image.");
    }
}
OPFillAmountPlayable::OPFillAmountPlayable(fairygui::GImage* image) :
    OPFillAmountPlayable(image, OPCurve<float>()) {

}
