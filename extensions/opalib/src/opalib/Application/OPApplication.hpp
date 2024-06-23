//
//  OPApplication.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPApplication_hpp
#define OPApplication_hpp

#include <cocos2d.h>
#include <opalib/Audio/Juce/OPJuceEngine.hpp>
#include <Utils/Validator.h>
#include <Utils/Vibrator.hpp>

class OPApplication : public cocos2d::Application
{
public:
    
    static OPApplication* shared();
    
    OPApplication();
    virtual ~OPApplication();

    virtual void createAppManager() = 0;

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
	
	virtual void applicationWillResignActive();
	virtual void applicationDidBecomeActive();
    
    OPAudioEngine* getAudioEngine();
    opalib::Validator* getDirectorValidator();
    opalib::Vibrator* getVibrator();


protected:
    std::unique_ptr<OPAudioEngine> _audioEngine { nullptr };
    std::unique_ptr<opalib::Validator> _directorValidator { nullptr };
    std::unique_ptr<opalib::Vibrator> _vibrator { nullptr };

    virtual void prepareCustomInitializers();
};

namespace opalib {
namespace Event {

struct ApplicationWillTerminate
{};
} // namespace Event
} // namespace opalib

#endif /* OPApplication_hpp */
