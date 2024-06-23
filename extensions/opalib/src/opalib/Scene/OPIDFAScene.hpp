//
// Created by Ricardo on 4/14/2021.
//

#ifndef OPIDFASCENE_HPP
#define OPIDFASCENE_HPP

#include "OPBaseScene.hpp"
#include <Misc/OPMacros.hpp>

namespace opalib {

    struct OPIDFASceneParams : OPBaseSceneParams {
        std::string confirmBtnId;
    };

    class OPIDFAScene : public OPBaseScene {
    public:
        CREATE_FUNC_WITH_ARGS(OPIDFAScene)

        bool init(std::unique_ptr<OPBaseSceneParams> params) override;

    protected:
        void onAdvance();


    };

}

#endif //OPIDFASCENE_HPP
