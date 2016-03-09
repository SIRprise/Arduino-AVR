#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <DebouncedInput.h>
#include <polytone.h>

LiquidCrystal_I2C lcd(0x27,20,4);

/*
 * Arduino Nano
 * A4=SDA (LCD)
 * A5=SCL (LCD)
 * D11 = Speaker
 * D2 = Button Enter (INT0)
 * D3 = Button DOWN (INT1)
 * D4 = Button UP   <-- UP / DOWN maybe switched -> change in getButton()
 */

 //TODO: echter Mischer per timer2 und sinustabelle, um Schwebung zu hören (max. 8kHz sampling)

//Stimmung: Halbtonfaktor = 2^(1/12)
#define WURZEL(n,x) pow(x, 1.0/n)
#define HALBTONFAKTOR WURZEL(12,2)
//#define HALBTONFAKTOR 1.059463094359295264561825294946341700779204317494185628559

#define BTN_ENTER 2
#define BTN_UP 4
#define BTN_DOWN 3

DebouncedInput di;

void soundfrq(double frq);
void playsnd(double frq,int ms);
int mainmnu();
void training(double percent);
void tuner();
void metronom();

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Geh\357rtrainer");

  pinMode(11, OUTPUT);
  int inputPins[] = {BTN_ENTER,BTN_UP,BTN_DOWN};
  di.init(inputPins, 3);
  Timer1.initialize(1000000/50); //50 Hz

  randomSeed(analogRead(0));
}

int speakerState = LOW;

void speaker(void)
{
  if (speakerState == LOW) {
    speakerState = HIGH;
  } else {
    speakerState = LOW;
  }
  digitalWrite(11, speakerState);
}

void loop() {
  int option = mainmnu();
  //int option=6;
  switch(option) {
    case 0: polytone::init();
            while(1){
              polytone::kanon();
			  delay(3000);
            }
            break;
    case 1: training(5);
            break;
    case 2: training(4);
            break;
    case 3: training(3);
            break;
    case 4: training(2);
            break;
    case 5: training(1);
            break;
    case 6: training(0.5);
            break;
    case 7: tuner();
            break;
    case 8: metronom();
            break;
  }
}

void soundfrq(double frq) {
  if(frq < 50) {
    //Timer1.disablePwm(11); //Pin11 is no PWM pin of Timer1
    Timer1.detachInterrupt();
    digitalWrite(11, LOW);
  }else{
    Timer1.initialize(1000000/(2*frq));
    Timer1.attachInterrupt(speaker);
    //Timer1.pwm(11,512,1000000/frq); //Pin11 is no PWM pin of Timer1
  }
}

void playsnd(double frq,int ms) {
  soundfrq(frq);
  delay(ms);
  soundfrq(0);
}

int mainmnu() {
  lcd.setCursor(0,0);
  lcd.print("Geh\357rtrainer");
  int modestate=0;
  int buttonstate;

  do{
    buttonstate=0;
    lcd.setCursor(0,1);
    switch(modestate) {
      case 0: lcd.print("Polytone-Demo    ");
              break;
      case 1: lcd.print("Training 5\45     ");
              break;
      case 2: lcd.print("Training 4\45     ");
              break;
      case 3: lcd.print("Training 3\45     ");
              break;
      case 4: lcd.print("Training 2\45     ");
              break;
      case 5: lcd.print("Training 1\45     ");
              break;
      case 6: lcd.print("Training 0.5\45   ");
              break;
      case 7: lcd.print("Tonerzeugung     ");
              break;
      case 8: lcd.print("Metronom         ");
              break;
      case 9: lcd.print("-----------------");
              break;
    }
    
    buttonstate=di.getButtonBusy();
      
    if(buttonstate==BTN_DOWN){
      if(modestate==9){
        modestate=0;
        }
      else{
        modestate++;
        }
    }
    if(buttonstate==BTN_UP){
      if(modestate==0){
        modestate=9;
        }
      else{
        modestate--;
        }
    }
  }while(buttonstate!=BTN_ENTER);
  
  return modestate;
}

void training(double percent) {  
  while(1){
  int rndfrq=100+random(900);
  delay(10);
  int higher=(random(15)%2);
  int dur=700;
  int buttonstate;
  
  lcd.setCursor(0,1);
  lcd.print("Referenzton... ");
  playsnd(rndfrq,dur);
  lcd.setCursor(0,1);
  lcd.print("                  ");
  delay(dur);
  if(higher==1)
    playsnd(rndfrq+rndfrq*(percent/100),dur);
  else
    playsnd(rndfrq-rndfrq*(percent/100),dur);

  lcd.setCursor(0,1);
  lcd.print("h\357her/tiefer?");
  buttonstate=di.getButtonBusy();
  if((higher==1)&&(buttonstate==BTN_UP)){
    lcd.setCursor(0,1);
    lcd.print("korrekt!  :-)    ");
  }
  if((higher==0)&&(buttonstate==BTN_DOWN)){
    lcd.setCursor(0,1);
    lcd.print("korrekt!  :-)    ");
  }

  if((higher==1)&&(buttonstate==BTN_DOWN)){
    lcd.setCursor(0,1);
    lcd.print("falsch!  :-(    ");
  }

  if((higher==0)&&(buttonstate==BTN_UP)){
    lcd.setCursor(0,1);
    lcd.print("falsch!  :-(    ");
  }
  delay(2000);
  }
}

void tuner() {
  float tuning=440.0;
  float frq=tuning;
  int buttonstate;
  int tunerstate=0;
  int tuneoct=0;
  String tuningstr;
  
  lcd.setCursor(0,0);
  lcd.print("Tuner - Stimmung A");
  tuningstr = String(tuning,1);
  lcd.setCursor(0,1);
  lcd.print(tuningstr+" Hz    ");
  
  do{
    buttonstate=di.getButtonBusy();
    switch(buttonstate){
      case BTN_UP:    tuning=tuning+0.5;
                      break;
      case BTN_DOWN:  tuning=tuning-0.5;
                      break;
    }
    tuningstr = String(tuning,1);
    lcd.setCursor(0,1);
    lcd.print(tuningstr+" Hz    ");
  }while(buttonstate != BTN_ENTER);

  while(1){
    lcd.setCursor(0,0);
    
    switch(tunerstate) {
      case 0: lcd.print("Tuner           ");
              lcd.setCursor(0,1);
              lcd.print("Ton aus     ");
              soundfrq(0);
              break;
      case 1: lcd.print("Tuner a     ");
              lcd.setCursor(0,1);
              frq=tuning*pow(2,tuneoct);
              tuningstr = String(frq,1);
              lcd.print(tuningstr+" Hz     ");
              soundfrq(frq);
              break;
      case 2: lcd.print("Tuner b     ");
              lcd.setCursor(0,1);
              frq=tuning*HALBTONFAKTOR*pow(2,tuneoct);
              tuningstr = String(frq,1);
              lcd.print(tuningstr+" Hz     ");
              soundfrq(frq);
              break;
    }   
    
    buttonstate=di.getButtonBusy();
    //TODO Zustandszahl absichern:
    switch(buttonstate){
      case BTN_UP:    if(tunerstate!=0)
                      tuneoct++;
                      break;
      case BTN_DOWN:  if(tunerstate!=0)
                      tuneoct--;
                      break;
      case BTN_ENTER: tuneoct=0;
                      if(tunerstate<2)
                        tunerstate++;
                      else
                        tunerstate=0;
                      break;
    }
  }
}

void metronom() {
  unsigned long times[4];
  unsigned long ms;
  
  lcd.setCursor(0,0);
  lcd.print("Metronom - per ENTER");
  lcd.setCursor(0,1);
  lcd.print("4x vorgeben!");

  delay(1000);

  for(int i=0;i<4;i++){
    times[i]=di.metrGetButtonBusyTime();
  }

  for(int i=1;i<4;i++){
    ms += (times[i]-times[i-1]);
  }
  ms = ms/4;

  lcd.setCursor(0,0);
  lcd.print("Metronom           ");
  lcd.setCursor(0,1);
  lcd.print(String( (1000.0/ms)*60.0 ,1)+" bpm       ");
  while(1){
    delay(ms);
    playsnd(440,20);
  }
}

