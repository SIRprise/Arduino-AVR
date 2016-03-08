#include "Arduino.h"
#include "DebouncedInput.h"

DebouncedInput::DebouncedInput( )
{
  
}

void DebouncedInput::init(int* pin, int nbOfPins)
{
  _nbOfPins=nbOfPins;
  _pin=(int*)malloc(nbOfPins * sizeof(int));
  
  for(int i=0;i<nbOfPins;i++) {
    pinMode(pin[i], INPUT_PULLUP);
    _pin[i]=pin[i];
  }
}

int DebouncedInput::getButton() {
  for(int i=0;i<_nbOfPins;i++) {
    if (digitalRead(_pin[i]) == LOW){
      while(digitalRead(_pin[i])==LOW); //wait until button is unpressed
      return _pin[i];
    }
  }
  return 0;
}

int DebouncedInput::getButtonBusy() {
  int buttonstate=0;
  
  while(buttonstate==0)
      buttonstate=this->getButton();
  
  return buttonstate;
}

//Abfrageroutine die Zeitpunkt des Drückens statt Loslassens nimmt
unsigned long DebouncedInput::metrGetButtonBusyTime() {
  bool pressed=false;
  unsigned long temptime;
  
  while(pressed==false){
    for(int i=0;i<_nbOfPins;i++) {
    if (digitalRead(_pin[i]) == LOW){
      pressed=true; 
    } 
    }
  }
  
  temptime=millis();
  delay(10);

  //warten bis finger vom knopf ist 
  while(pressed==true){
    for(int i=0;i<_nbOfPins;i++) {
    if (digitalRead(_pin[i]) == HIGH) {
      pressed=false; 
    } else {
      pressed=true;
    }
    }
  }

  delay(10);
  return temptime;
}
