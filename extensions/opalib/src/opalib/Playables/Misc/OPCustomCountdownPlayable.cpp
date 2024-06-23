#include <Playables/Misc/OPCustomCountdownPlayable.hpp>
#include <GTextField.h>

OPCustomCountdownPlayable::OPCustomCountdownPlayable(fairygui::GTextField* field, const std::vector<std::string>& messages) :
    _messages(messages),
    _field(field) {}

void OPCustomCountdownPlayable::evaluate(OPFrameTimes times) {
    int index = (int) std::floor(_messages.size() * times.progress);
    _field->setText(_messages[index]);
}
