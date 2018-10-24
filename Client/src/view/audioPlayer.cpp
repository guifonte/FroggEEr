#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstring>
#include <random>
#include <thread>
#include <chrono>

#include "../../inc/view/audioPlayer.hpp"

using namespace Audio;
using namespace std::chrono;

Sample::Sample() {

}

Sample::~Sample() {

}

bool Sample::finished() {
  if ( (this->position) >= (this->data.size())) return true;
  else return false;
}

void Sample::load(const char *filename) {
  std::string buffer;
  float fdata;
  std::ifstream input_file;
  unsigned int count = 0;

  input_file.open(filename, std::ios_base::in);
  if (!input_file) {
    std::cerr << "Arquivo " << filename << " nao encontrado" << std::endl;
    return;
  }

  while (std::getline(input_file, buffer) ) {
    std::stringstream(buffer) >> fdata;
    (this->data).push_back(fdata);
    count ++;
  }
  this->position = 0;

}

unsigned int Sample::get_position() {
  return this->position;
}

void Sample::set_position(unsigned int pos) {
  this->position = pos;
}

std::vector<float> Sample::get_data() {
  return this->data;
}


SoundPlayer::SoundPlayer() {
  this->playing = false;
  this->audio_sample = NULL;
}

void SoundPlayer::pause() {
  this->playing = false;
}

SoundPlayer::~SoundPlayer() {

}

Sample *SoundPlayer::get_data() {
  return this->audio_sample;
}


int mix_and_play (const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData )

{
  SoundPlayer *soundPlayer = (SoundPlayer*) userData;
  float *buffer = (float *) outputBuffer;
  Sample *s;
  s = soundPlayer->get_data();
  if (s != NULL) {
    std::vector<float> data = s->get_data();
    unsigned int pos = s->get_position();

    // Fill the buffer with samples!
    for (int i=0; (i<framesPerBuffer); i++) {
      if (pos < data.size())
        buffer[i] = data[pos];
      else
        buffer[i] = 0;
      pos=pos+1;
    }
    s->set_position(pos);
  }
  return 0;
}

void SoundPlayer::play(Sample *audiosample) {
  this->audio_sample = audiosample;
}

void SoundPlayer::init() {
  PaError err;

  err = Pa_Initialize();
  if( err != paNoError ) {
    std::cerr << "Error on Pa_Initialize()" << std::endl;
    return;
  }

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* Default output device. */
  if (outputParameters.device == paNoDevice) {
    std::cerr << "Error: No default output device on Pa_GetDefaultOutputDevice()" << std::endl;
    return;
  }

  outputParameters.channelCount = 1;                     /* Mono output. */
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  err = Pa_OpenStream( &stream,
                         NULL,      /* No input. */
                         &outputParameters,
                         44100,
                         64,       /* Frames per buffer. */
                         paClipOff, /* We won't output out of range samples so don't bother clipping them. */
                         mix_and_play,
                         this );

  if( err != paNoError ) {
    std::cerr << "Error on Pa_OpenStream()" << std::endl;
    return;
  }

  err = Pa_StartStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StartStream()" << std::endl;
    return;
  }


}

void SoundPlayer::stop() {
  PaError err;
  err = Pa_StopStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StopStream()" << std::endl;
    return;
  }

  err = Pa_CloseStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StopStream()" << std::endl;
    return;
  }

  Pa_Terminate();

}

uint64_t SoundManager::get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

SoundManager::SoundManager(std::string resDir) {
  this->soundPlayer = new SoundPlayer();
  this->killSound = new Sample();
  this->levelupSound = new Sample();
  this->killSound->load(resDir.append("blip.dat").c_str());
  this->levelupSound->load(resDir.append("levelup.dat").c_str());
}

void SoundManager::playKillSound(uint64_t t0) {
  uint64_t t1;
  this->killSound->set_position(0);
  this->soundPlayer->init();
  this->soundPlayer->play(this->killSound);
  t0 = SoundManager::get_now_ms();
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = SoundManager::get_now_ms();

    if (t1-t0 > 500) break;
  }
  this->soundPlayer->stop();
}

void SoundManager::playLevelUpSound(uint64_t t0) {
  uint64_t t1;
  this->levelupSound->set_position(0);
  this->soundPlayer->init();
  this->soundPlayer->play(this->levelupSound);
  t0 = SoundManager::get_now_ms();
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = SoundManager::get_now_ms();

    if (t1-t0 > 1500) break;
  }
  this->soundPlayer->stop();
}



