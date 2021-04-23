
#include <LiquidCrystal.h> // include the LCD library
const int rs = PA6, en = PA5, d4 = PA4, d5 = PA3, d6 = PA2, d7 = PA1; //STM32 Pins to which LCD is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD
unsigned long previousMillis = 0; 
unsigned long currentMillis = 0;
float average = 0;
int Delay= 200;
int counter2;
int val;
float setvolt=0;
float setcur=0;
double test;
float val1;
float amps;                   //pin decleration
float Value;
float calfactor=0;
float calfactor1=-0.3; //current calibration
float calfactor2=-0.04;
float calfactor3=-0.24;
float calfactor4=-0.66;
float calfactor5=0;
float calfactor6=0.018;
float alog;
int btn1;
int btn2;
int counter;
int pwm_value;
float average1;
float average2;
float Start;
HardwareTimer timer(1);  //select timer
void setup() {
  Serial.begin(115200);
pinMode(PA8, PWM);
pinMode(PB1, INPUT_ANALOG);
pinMode(PB0, INPUT_ANALOG);
pinMode(PA4, INPUT_ANALOG);
pinMode(PA0, INPUT_ANALOG);     //pin setting
pinMode(PA7, INPUT_ANALOG);
pinMode(PB11,INPUT_PULLUP);
pinMode(PB7,OUTPUT);
pinMode(PB6,OUTPUT);
pinMode(PB14,OUTPUT);
pinMode(PB8,OUTPUT);
attachInterrupt(digitalPinToInterrupt(PB11),btnA,FALLING);
pinMode(PB10,INPUT_PULLUP);                                   //setting interupts
attachInterrupt(digitalPinToInterrupt(PB10),btnB,FALLING);
timer.setPrescaleFactor(1);
timer.setOverflow(1020);             //setting pwm timer
digitalWrite(PB8,LOW);
lcd.begin(16, 2);//Defining 16*2 LCD

 digitalWrite(PB14,LOW);
 Start=0;
}

void loop() {
alog=analogRead(PB1);    //read analog values

if(setvolt<=20 && setvolt>13){
  calfactor=calfactor2;
}
if(setvolt>20 && setvolt<30){
  calfactor=calfactor3;
}
if(setvolt>30){
  calfactor=calfactor4;
}
if(setvolt<=13 && setvolt>=9.5){
  calfactor=calfactor5;
}
if(setvolt<9.5){
  calfactor=calfactor6;
}
alog=alog*0.00080566406*(98/3.88);
alog=alog+calfactor;


for(int j=0; j < 1000; j++)
  {
    val1 = analogRead(PB0) * 0.0008059;
    amps = amps + (2.45-val1)/0.100; 
  }
  amps = amps/1000;
  amps=amps-calfactor1;
  
if(amps>12){
   pwmWrite(PA8,0);
  currentMillis = millis();
  if(currentMillis - previousMillis >= Delay){  
    previousMillis += Delay;
    lcd.clear();
  lcd.print("OUTPUT SHORT!");
  }
}

 if(counter2==1){

    digitalWrite(PB8,LOW);
     setvolt=analogRead(PA0);
 setcur =analogRead(PA7);    //read analog values
 setvolt=setvolt/51.188;     //convert analog value to voltage
 setcur=setcur/409.5;
    digitalWrite(PB6,LOW);
    digitalWrite(PB7,HIGH);
     digitalWrite(PB14,LOW);
    currentMillis = millis();
  if(currentMillis - previousMillis >= Delay){  
    previousMillis += Delay;
    lcd.clear();
Serial.println(calfactor);
  lcd.print("SET- ");
 
  lcd.print(setvolt);
  lcd.print("V ");
  
  lcd.print(setcur);
  lcd.print("A");
lcd.setCursor(0, 1);
  lcd.print("! OUTPUT IS ON ");
  
}
 
 }

if(Start<50){
  setvolt=analogRead(PA0);
 setcur =analogRead(PA7);    //read analog values
 setvolt=setvolt/51.188;     //convert analog value to voltage
 setcur=setcur/409.5;
  Start ++;
}


if(amps<setcur){
  if(setvolt>80){
    setvolt=80;
  }
if(setvolt < alog)
    {
      pwm_value = pwm_value - 1;                  //When we decrease PWM width, the volage gets higher at the output. 
      pwm_value = constrain(pwm_value, 0, 938);
       digitalWrite(PB14,LOW);
    }
    //If set voltage is lower than real value from feedback, we increase PWM width till we get same value
    if(setvolt > alog)
    {
      pwm_value = pwm_value + 1;                  //When we increase PWM width, the volage gets lower at the output. 
      pwm_value = constrain(pwm_value, 0, 938);
       digitalWrite(PB14,LOW);
    }  

}
if(amps>setcur){
   digitalWrite(PB14,HIGH);
   pwm_value = pwm_value - 1;                  //When we increase PWM width, the volage gets lower at the output. 
      pwm_value = constrain(pwm_value, 0, 938);
      
    }  
pwmWrite(PA8,pwm_value);

 if(setcur<0){
  setcur=0;
 }
 // if(amps<0){
 // amps=0;
 //}

  if(counter2==0){
      digitalWrite(PB6,HIGH);
    digitalWrite(PB7,LOW);
    digitalWrite(PB8,HIGH);
       
  currentMillis = millis();
  if(currentMillis - previousMillis >= Delay){  
    previousMillis += Delay;
    Serial.println(alog);
  lcd.clear();
  lcd.print("    OUTPUT ");
  lcd.setCursor(0, 1);
  lcd.print(setvolt);
  lcd.print("V ");
  lcd.print(amps);
  lcd.print("A");
}
}



if(counter2>1){
  counter2=0;
}
if(counter>2){
  counter=0;
}
}
void btnA(){
btn1=digitalRead(PB11);        //this set of code works when pb11 is pressed
if(btn1==LOW){
 counter ++;
 delay(3);
}
}
void btnB(){
  digitalWrite(PB6,HIGH);
  btn1=digitalRead(PB10);        //this set of code works when pb6 is pressed
  Serial.println(btn1);
if(btn1==LOW){
  counter2 ++;
  delay(3);
}
}
