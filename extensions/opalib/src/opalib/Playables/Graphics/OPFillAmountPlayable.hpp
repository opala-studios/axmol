//
// Created by bruno on 17/06/2021.
//

#ifndef OPALIB_CINEMATICS_OPFILLAMOUNTPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPFILLAMOUNTPLAYABLE_HPP_
#include <Playables/OPPlayable.hpp>
#include <FairyGUI.h>
#include <Math/OPCurve.hpp>
class OPFillAmountPlayable : public OPPlayable {
private:
    fairygui::GImage* _image;
    OPCurve<float> _fillAmountCurve;
public:
    OPFillAmountPlayable(fairygui::GImage* image, const OPCurve<float>& fillAmountCurve);

    OPFillAmountPlayable(fairygui::GImage* image);
    void evaluate(OPFrameTimes times) override;

};

#endif //OPALIB_CINEMATICS_OPFILLAMOUNTPLAYABLE_HPP_
