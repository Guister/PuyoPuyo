#include "AudioMixer.hpp"
#include "utils/FileSystem.hpp"

AudioMixer::AudioMixer() {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        std::cout << "Could not open audio: " << Mix_GetError() << std::endl;
    }

    //CHANGED!
    auto bounce = filesystem::GetSourceFolder().string() + "/assets/sound/BounceYoFrankie.wav";
    auto pop = filesystem::GetSourceFolder().string() + "/assets/sound/pop.wav";
    auto bgMusic = filesystem::GetSourceFolder().string() + "/assets/sound/Casual 8-bit.wav";

    sounds = {
            {"bounce", Mix_LoadWAV(bounce.c_str())},
            {"pop", Mix_LoadWAV(pop.c_str())}
    };
    soundtrack = {
            {"bg", Mix_LoadMUS(bgMusic.c_str())}};

    Mix_VolumeMusic(10);
    playMusic("bg", -1);

}

AudioMixer &AudioMixer::getAudioMixer() {
    static AudioMixer onlyInstance;
    return onlyInstance;
}

void AudioMixer::playSound(std::string_view title, int loops) {
    if (Mix_PlayChannel(-1, sounds.at(title), loops) == -1) {
        std::cout << "Could not play sound: " << Mix_GetError() << std::endl;
    }
}

void AudioMixer::playMusic(std::string_view title, int loops) {
    if (Mix_PlayMusic(soundtrack.at(title), loops) == -1) {
        std::cout << "Could not open music: " << Mix_GetError() << std::endl;
    }
}

void AudioMixer::shutdown() { //CHANGED!
    for (auto &[name, sound]: sounds) {
        if (sound != nullptr) {
            Mix_FreeChunk(sound);
            sound = nullptr;
        }
    }
    for (auto &[name, music]: soundtrack) {
        if (music != nullptr) {
            Mix_FreeMusic(music);
            music = nullptr;
        }
    }
    Mix_CloseAudio();
    Mix_Quit();
}
