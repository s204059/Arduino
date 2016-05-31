#include "TimerOne.h"

int read1[3];

void input_manager();

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);

  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  
  Timer1.initialize(5000000);         // initialize timer1, and set a 1/2 second period
  //Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(input_manager);  // attaches callback() as a timer overflow interrupt
  
 }

void loop() {
  // put your main code here, to run repeatedly:
  for(;;)
  {
    if(4*read1[2]+2*read1[1]+1*read1[0]> 2) digitalWrite(5,LOW);
    else digitalWrite(5,HIGH);
  }
}

void input_manager(void) {
  read1[0]=digitalRead(11);
  read1[1]=digitalRead(12);
  read1[2]=digitalRead(13);
  
  digitalWrite(2,read1[0]);
  digitalWrite(3,read1[1]);
  digitalWrite(4,read1[2]);
  
  digitalWrite(6,digitalRead(6) ^ 1);
}

