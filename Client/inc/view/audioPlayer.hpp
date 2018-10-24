#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include "portaudio.h"
#include <vector>
#include <cstring>
#include <string>

namespace Audio {

class Sample {
  private:
    std::vector<float> data;
    unsigned int position;

  public:
    Sample();
    ~Sample();
    void load(const char *filename);
    std::vector<float> get_data();
    unsigned int get_position();
    void set_position(unsigned int pos);
    bool finished();

};

class SoundPlayer {
  private:
    Sample *audio_sample;
    bool playing;

    PaStreamParameters  outputParameters;
    PaStream*           stream;
    PaError             err;
    PaTime              streamOpened;


  public:
    SoundPlayer();
    ~SoundPlayer();

    void init();
    void pause();
    void stop();
    void play(Sample *audiosample);
    Sample *get_data();
};

class SoundManager {
  private:
    SoundPlayer *soundPlayer;
    Sample *killSound;
    Sample *levelupSound;
    uint64_t static get_now_ms();
  public:
    SoundManager(std::string resDir);
    void playKillSound(uint64_t t0);
    void playLevelUpSound(uint64_t t0);
};
}
#endif

