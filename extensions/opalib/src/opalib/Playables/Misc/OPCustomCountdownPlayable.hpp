#ifndef OPALIB_CINEMATICS_OPCUSTOMCOUNTDOWNPLAYABLE_HPP_
#define OPALIB_CINEMATICS_OPCUSTOMCOUNTDOWNPLAYABLE_HPP_

#include <Playables/OPPlayable.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <GTextField.h>
class OPCustomCountdownPlayable : public OPPlayable {
private:
    std::vector<std::string> _messages;
    fairygui::GTextField* _field;
public:
    OPCustomCountdownPlayable(fairygui::GTextField* field, const std::vector<std::string>& messages);
    void evaluate(OPFrameTimes times) override;
};

#endif