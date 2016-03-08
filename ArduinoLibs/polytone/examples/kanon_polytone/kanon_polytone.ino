#include <polytone.h>

//Kanon Polytone-Demo
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
#define B4 466.16
#define H4 493.88
#define C5 523.25
#define CS5 555.37 //korrigiert
#define D5 (2*D4) //korrigiert
#define DS5 622.25
#define E5 659.26
#define F5 698.46
#define FS5 739.99
#define G5 783.99
#define GS5 830.61
#define A5 880.0
#define B5 (B4*2)
#define H5 (H4*2)


float kanonMelodie[8]={D5/2,A3,H3,FS4/2,G4/2,D4/2,G4/2,A3};
float kanonBegleit[8]={FS5,E5,D5,CS5,H4,A4,H4,CS5};

void kanon()
{
  for(int i=0;i<7;i++)
  {
    float F[2]={2*kanonMelodie[i],2*kanonBegleit[i]};
    float vols[2]={1,0.2};
    polytone::play(F,vols,2,1500);
  }
}

void setup()
{
  polytone::init();
}

void loop()
{
  
  float F[2]={2*kanonMelodie[7],2*kanonBegleit[7]};
  float vols[2]={1,0.2};
  polytone::play(F,vols,2,2500);
  delay(5000);
}

