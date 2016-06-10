#include "TimerOne.h"

#define photocell_window 1
#define photocell_door 2
#define Vcc 7
#define fan_1 13
#define fan_2 14

#define waiting_for_sensor 10  /* HIGH if you want the fan to run */

int open_window = 0;        /* 0 -> closed, 1 -> open */
int open_door = 0;
int fans_status = 0;        /* 1 if the fans are currently working */
int fans_timeout = 60;
int immediate_on = 0;       /* if 1 the fans will start running imediately */

int on_off = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(photocell_window,INPUT);
  pinMode(photocell_door,INPUT);

  pinMode(Vcc,OUTPUT);
  
  pinMode(fan_1,OUTPUT);
  pinMode(fan_2,OUTPUT);

  pinMode(waiting_for_sensor,INPUT);

  Timer1.initialize(5000000);           /* initialize timer1, and set a 5 seconds period */
  Timer1.attachInterrupt(sensing);      /* set sensing() as the timer overflow interrupt ISR */

  digitalWrite(photocell_window,HIGH);
  digitalWrite(photocell_door,HIGH);

  digitalWrite(Vcc,HIGH);

  digitalWrite(fan_1,LOW);
  digitalWrite(fan_2,LOW);

  digitalWrite(waiting_for_sensor,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

  if(on_off == 1 ){
    if( fans_status == 0 ){
      digitalWrite(fan_1,HIGH);
      digitalWrite(fan_2,HIGH);
    }
  }
  
  if( open_window == 1 or open_door == 1 ){
    digitalWrite(fan_1,LOW);
    digitalWrite(fan_2,LOW);

    fans_status = 0;
    fans_timeout = 60;
  } // The fans won't run if windows or doors are open

  if( fans_timeout == 0 ){
    digitalWrite(fan_1,LOW);
    digitalWrite(fan_2,LOW);

    fans_status = 0;
    fans_timeout = 60;
  } // this code segment is used to turn off the fans after approximately 5 minutes of continous operation

  if( immediate_on == 1 ){
    if( fans_status == 0 ){
      digitalWrite(fan_1,HIGH);
      digitalWrite(fan_2,HIGH);
    }
    else
      fans_timeout == 60;
  } // This code segments immediately turns on the fans when an immediate action is required
}

void sensing(){
  // put the code of Timer1 ISR here:

  on_off = digitalRead(waiting_for_sensor);
  
  open_window = digitalRead(photocell_window);
  open_door = digitalRead(photocell_door);

  if (fans_status == 1)
    fans_timeout = fans_timeout - 1;
}

