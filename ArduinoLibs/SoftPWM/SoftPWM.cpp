#include "Arduino.h"
#include "SoftPWM.h"

int SoftPWM::_pin;

void SoftPWM::init(int pin)
{
  _pin=pin;
}

void SoftPWM::start()
{
  pinMode(_pin, OUTPUT);
/*
  // set fast PWM mode
  TCCR2A |= ((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);
*/
  // set CTC mode
  TCCR2A |= (1<<WGM21);
  TCCR2A &= ~(1<<WGM20);
  TCCR2B &= ~(1<<WGM22);
  /*
  // Arduino pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  */

  // presc 8 -> 7,8kHz
  TCCR2B |= (1<<CS21);
  TCCR2B &= ~((1<<CS20)|(1<<CS22));
  //stopped = CS22 0 CS21 0 CS20 0
  //no presc= CS22 0 CS21 0 CS20 1
  //8 =       CS22 0 CS21 1 CS20 0
  //32 =      CS22 0 CS21 1 CS20 1
  //64 =      CS22 1 CS21 0 CS20 0
  //128=      CS22 1 CS21 0 CS20 1
  //256=      CS22 1 CS21 1 CS20 0
  //1024=     CS22 1 CS21 1 CS20 1

  // set initial pulse width
  OCR2A = 255; //OCR2A > OCR2B !
  OCR2B = 0; //init val

  //TIMSK2|=(1<<TOIE2);
  TIMSK2|=(1<<OCIE2A);
  TIMSK2|=(1<<OCIE2B);

  //sei();
}

void SoftPWM::stop()
{
  // Disable the PWM timer.
  TCCR2B &= ~((1<<CS20)|(1<<CS21)|(1<<CS22));
  TIMSK2=0;

  digitalWrite(_pin, LOW);
}

void SoftPWM::setDuty(unsigned char val)
{
  OCR2B = val;
}

/*
ISR(TIMER2_OVF_vect)
{
  if(SoftPWM::_state==true){
    digitalWrite(SoftPWM::_pin, LOW);
    SoftPWM::_state=false;
  }else{
    digitalWrite(SoftPWM::_pin, HIGH);
    SoftPWM::_state=true;
  }
}
*/

ISR(TIMER2_COMPA_vect)
{
    digitalWrite(SoftPWM::_pin, HIGH);
    //PORTB|=(1<<PB5); //pin13
}

ISR(TIMER2_COMPB_vect)
{
    digitalWrite(SoftPWM::_pin, LOW);
    //PORTB&=~(1<<PB5);
}


