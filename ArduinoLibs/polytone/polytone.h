#ifndef polytone_h
#define polytone_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <math.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define _useTimer1
#define _useTimer2

namespace polytone {
  void init();
  void play(float* freqs, float* vols, uint8_t nb, uint16_t delayms);
  void playSolo(float freq, float vol, uint16_t delayms);
  void startPlayback();
  void stopPlayback();
  void kanon(); //demo
}

#endif
