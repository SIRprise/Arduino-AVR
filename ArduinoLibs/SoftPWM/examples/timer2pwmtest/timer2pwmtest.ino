#include <SoftPWM.h>

#define SAMPLE_RATE 7812
#define V_SR (SAMPLE_RATE/100)
#define SIN_SIZE 256

uint8_t sintable[SIN_SIZE];
float cnt=0;

void setup() {
  //sinus-tabelle berechnen
  for(uint16_t i=0;i<SIN_SIZE;i++){
    sintable[i]=(uint8_t)trunc(sin((i/(float)SIN_SIZE)*2*PI)*127.5+127.5);
  }
  
  SoftPWM::init(13);
  SoftPWM::start();
  while(1)
  {
    SoftPWM::setDuty(sintable[(char)cnt]);
    cnt+=1.5;
    if (cnt>255)
      cnt=0;
  }
}

void loop() {
  
}
