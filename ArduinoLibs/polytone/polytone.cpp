#include "polytone.h"

#define SAMPLE_RATE 8000
#define V_SR (SAMPLE_RATE/100)
#define SIN_SIZE 256

int speakerPin = 11;
float* SW;
float* SWpos;
float* vol;

uint8_t NB_OF_FREQS;
uint8_t sintable[SIN_SIZE];

void polytone::init()
{
  //sinus-tabelle berechnen
  for(uint16_t i=0;i<SIN_SIZE;i++){
    sintable[i]=(uint8_t)trunc(sin((i/(float)SIN_SIZE)*2*PI)*127.5+127.5);
  } 
}

void polytone::startPlayback()
{
  pinMode(speakerPin, OUTPUT);

  // Set fast PWM mode (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);
  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  // No prescaler (p.158)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  // Set initial pulse width to the first sample.
  OCR2A = 128;
  // Set up Timer 1 to send a sample every interrupt.
  cli();
  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  // No prescaler (p.134)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  OCR1A = F_CPU / SAMPLE_RATE; // 16e6 / 8000 = 2000
  // Enable interrupt when TCNT1 == OCR1A (p.136)
  TIMSK1 |= _BV(OCIE1A);
  sei();
}

void polytone::stopPlayback()
{
  // Disable playback per-sample interrupt.
  TIMSK1 &= ~_BV(OCIE1A);
  // Disable the per-sample timer completely.
  TCCR1B &= ~_BV(CS10);
  // Disable the PWM timer.
  TCCR2B &= ~_BV(CS10);
  digitalWrite(speakerPin, LOW);
}

void polytone::play(float* freqs, float* vols, uint8_t nb, uint16_t delayms)
{
  NB_OF_FREQS=nb;

  SW=(float *) malloc(NB_OF_FREQS * sizeof(float));
  SWpos=(float *) malloc(NB_OF_FREQS * sizeof(float));
  vol=(float *) malloc(NB_OF_FREQS * sizeof(float));
  
  for(uint8_t i=0;i<NB_OF_FREQS;i++){
    SW[i]=(freqs[i])*((float)SIN_SIZE/V_SR);
    SW[i]/=100;
    SWpos[i]=0;
    vol[i]=vols[i];
  } 
  startPlayback();
  delay(delayms); // könnte auch über Sample-Timer gemacht werden,
                  // aber delay-timer(0) läuft sowieso
  stopPlayback();
  free(SW);
  free(SWpos);
  free(vol);
}

void polytone::playSolo(float freq, float vols, uint16_t delayms)
{
  NB_OF_FREQS=1;
  SW=(float *) malloc(sizeof(float));
  SWpos=(float *) malloc(sizeof(float));
  vol=(float *) malloc(sizeof(float));

  SW[0]=freq*((float)SIN_SIZE/V_SR);
  SW[0]/=100;
  SWpos[0]=0;
  vol[0]=vols;

  startPlayback();
  delay(delayms);
  stopPlayback();
  free(SW);
  free(SWpos);
  free(vol);
}

// wird SAMPLE_RATE x pro Sekunde aufgerufen
ISR(TIMER1_COMPA_vect) {
  int16_t res=0;
  
  //optimierte for-schleife (SampleRate max=8000 bei 2 Freqs)
  uint8_t i;
  i=NB_OF_FREQS;
  do
  {
    res+=(vol[i-1]*(sintable[(uint8_t)SWpos[i-1]]-128));
    SWpos[i-1]=fmod((SWpos[i-1]+SW[i-1]),SIN_SIZE);
  }while(--i);
  OCR2A=128+(res/NB_OF_FREQS);
  
  /*
  //optimierte routine, falls vol nicht gebraucht wird
  // (SampleRate max=16000 bei 2 Freqs)
  uint8_t i;
  i=NB_OF_FREQS;
  do
  {
    res+=(sintable[(uint8_t)SWpos[i-1]]);
    SWpos[i-1]=fmod((SWpos[i-1]+SW[i-1]),SIN_SIZE);
  }while(--i);
  OCR2A=(res/NB_OF_FREQS);
  */
  /*
  //optimierte Routine ohne vol und mit nur 2 Freqs
  OCR2A=(sintable[(uint8_t)SWpos[0]]+sintable[(uint8_t)SWpos[1]])>>1;
  SWpos[0]=fmod((SWpos[0]+SW[0]),SIN_SIZE);
  SWpos[1]=fmod((SWpos[1]+SW[1]),SIN_SIZE);
  */
}

//Kanon Polytone-Demo
//------------------------------------------------

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

void polytone::kanon()
{
  for(int i=0;i<7;i++)
  {
    float F[2]={2*kanonMelodie[i],2*kanonBegleit[i]};
    float vols[2]={1,0.2};
    polytone::play(F,vols,2,1500);
  }
}