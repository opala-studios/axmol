//
// Created by Ricardo on 10/23/2020.
//

#ifndef OPALIB_OPSOUNDCONTROLLER_H
#define OPALIB_OPSOUNDCONTROLLER_H

#include <unordered_map>
#include "opalib/Audio/OPSynthesizer.hpp"

class OPSoundController {
public:

    typedef std::function<void(bool)> LoadSoundHandler;

    static OPSoundController* shared();

    void initialize();
    void play(const std::string& soundName);
    void playByPath(const std::string& soundName, const std::string& soundPath);
    void stop(const std::string& soundName);
    void stopAll();

    void addSound(const std::string &name, const std::string &filePath,
                  const LoadSoundHandler &onCompletion);

private:
    static std::unordered_map<std::string, std::shared_ptr<OPSynthItem>> load_json(const std::string& filepath);

private:
    std::unordered_map<std::string, std::shared_ptr<OPSynthItem>> _synthItems;
    std::unique_ptr<OPSynthesizer> _synthesizer;
};


#endif //OPALIB_OPSOUNDCONTROLLER_H
