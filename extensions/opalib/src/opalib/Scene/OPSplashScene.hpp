//
// Created by Ricardo on 4/12/2021.
//

#ifndef OP_OPSPLASH_HPP
#define OP_OPSPLASH_HPP

#include "OPBaseScene.hpp"
#include <Misc/OPMacros.hpp>

namespace opalib {

    struct OPSplashSceneParams : public OPBaseSceneParams {
        float waitTime = 3.0f;
    };

    class OPSplashScene : public OPBaseScene {
    public:
        CREATE_FUNC_WITH_ARGS(OPSplashScene);

        bool requiresBanner() override { return false; }

        void onEnterTransitionDidFinish() override;

    private:

        std::vector<std::string> getDependenciesPackagesNames() override {
            return {"Resources"};
        }
		
		void _callNextScene();

    };

}

#endif //PROJ_ANDROID_OPSPLASH_HPP
