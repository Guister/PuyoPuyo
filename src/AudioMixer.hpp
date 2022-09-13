#ifndef PUYOPUYO_AUDIOMIXER_HPP
#define PUYOPUYO_AUDIOMIXER_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/*
 * Responsable to deal with the game sound.
 * Singleton.
 */

class AudioMixer {
private:
    std::unordered_map<std::string_view , Mix_Chunk *> sounds;
    std::unordered_map<std::string_view , Mix_Music *> soundtrack;

    AudioMixer();
    static AudioMixer &getAudioMixer();
    friend class GameManager;
public:
    AudioMixer(const AudioMixer &) = delete;
    AudioMixer &operator=(const AudioMixer &) = delete;
    AudioMixer(AudioMixer &&) = delete;
    AudioMixer &operator=(AudioMixer &&) = delete;

    void playSound(std::string_view title, int loops = 0);
    void playMusic(std::string_view title, int loops = 0);
    void shutdown();
};


#endif//PUYOPUYO_AUDIOMIXER_HPP
