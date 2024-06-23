//
// Created by Ricardo on 4/14/2021.
//

#include "OPIDFAScene.hpp"

using namespace opalib;

bool OPIDFAScene::init(std::unique_ptr<OPBaseSceneParams> params) {
    if (!OPBaseScene::init(std::move(params))){
        return false;
    }

    auto p = getParams<OPIDFASceneParams>();

    _view->getChild(p->confirmBtnId)->addClickListener([this](fairygui::EventContext*){
        onAdvance();
    });

    return true;
}

void OPIDFAScene::onAdvance() {

}
