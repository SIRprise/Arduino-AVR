#include <polytone.h>
//Speaker = Pin11, uses Timer 2 for PWM and Timer 1 for Timing

#define A3 220.00
#define B3 233.08
#define H3 246.94
#define C4 261.63
#define CS4 277.18
#define D4 293.66
#define DS4 311.13
#define E4 329.63
#define F4 349.23
#define FS4 369.99
#define G4 392.00
#define GS4 415.3
#define A4 440.0


void setup()
{
  polytone::init();
}

void loop()
{
  /*
  float F[2]={2*A3,2*C4};
  float vols[2]={0.3,1};
  polytone::play(F,vols,2,2500);
  */
  float F[2]={440,860};
  float vols[2]={1,0.5};
  polytone::play(F,vols,2,5000);
  //delay(5000);
  polytone::play(F,vols,1,5000);
  delay(5000);
}

