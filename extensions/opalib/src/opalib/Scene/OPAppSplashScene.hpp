//
// Created by Ricardo on 4/13/2021.
//

#ifndef OP_OPAPPSPLASHSCENE_HPP
#define OP_OPAPPSPLASHSCENE_HPP

#include <Scene/OPBaseScene.hpp>
#include <Misc/OPMacros.hpp>

namespace opalib {

    struct OPAppSplashSceneParams : public OPBaseSceneParams {

    };

    class OPAppSplashScene : public OPBaseScene {
    public:
        CREATE_FUNC_WITH_ARGS(OPAppSplashScene)

        virtual void onEnter() override;

    protected:
        void onSplashAnimationEnded();
        virtual void advanceToNextScene();

    protected:
        bool _isGdprRequired = false;
    };

}

#endif //OP_OPAPPSPLASHSCENE_HPP
