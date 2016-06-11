#include "TimerOne.h"

#define photocell_window 1
/* #define photocell_door 2 */
#define Vcc 7
#define fan_1 13
#define fan_2 14

#define waiting_for_sensor 10  /* HIGH if you want the fan to run */

boolean something_open = false;        /* 0 -> closed, 1 -> open */
int fans_status = 0;                   /* 1 if the fans are currently working */
int fans_timeout = 60;
boolean immediate_on = false;          /* if true the fans will start running imediately */
boolean immediate_off = false;         /* if true the system will shut down */ 

int on_off = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(photocell_window,INPUT);
  /* pinMode(photocell_door,INPUT); */

  pinMode(Vcc,OUTPUT);
  
  pinMode(fan_1,OUTPUT);
  pinMode(fan_2,OUTPUT);

  pinMode(waiting_for_sensor,INPUT);

  Timer1.initialize(5000000);           /* initialize timer1, and set a 5 seconds period */
  Timer1.attachInterrupt(sensing);      /* set sensing() as the timer overflow interrupt ISR */

  digitalWrite(photocell_window,HIGH);  /* digitalWrite(x,HIGH) enables pull-up resistor on input x */
  /* digitalWrite(photocell_door,HIGH); */

  digitalWrite(Vcc,HIGH);

  digitalWrite(fan_1,LOW);              /* digitalWrite(y,LOW) sets to 0V (ground) the voltage on ouput y */
  digitalWrite(fan_2,LOW);

  digitalWrite(waiting_for_sensor,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

  if( on_off == 1 and something_open == false ){
    if( fans_status == 0 ){
      digitalWrite(fan_1,HIGH);
      digitalWrite(fan_2,HIGH);
      
      fans_status = 1;
    }
  } // if on_off is 1, no windows or doors are open and the fans were not running already, they will now start
  
  if( something_open == true ){
    digitalWrite(fan_1,LOW);
    digitalWrite(fan_2,LOW);

    fans_status = 0;
    fans_timeout = 60;
  } // The fans won't run anymore if windows or doors are open, the timeout is returned to defualt value

  if( fans_timeout == 0 ){
    digitalWrite(fan_1,LOW);
    digitalWrite(fan_2,LOW);

    fans_status = 0;
    fans_timeout = 60;
  } // this code segment is used to turn off the fans after approximately 5 minutes of continous operation

  if( immediate_on == true and something_open == false ){
    if( fans_status == 0 ){
      digitalWrite(fan_1,HIGH);
      digitalWrite(fan_2,HIGH);

      fans_status = 1;
    }
  } // This code segments immediately turns on the fans (if not already on) when an immediate action is required 

  if( immediate_off == true )
    Timer1.detachInterrupt();
  else
    Timer1.attachInterrupt(sensing);
}

void sensing(){
  // put the code of Timer1 ISR here:

  // since there's a pull-up resistor, on status corresponds to logic zero provided to the pin:
  if( digitalRead(waiting_for_sensor) == LOW )
    on_off = 1;
  else
    on_off = 0;

  // given that photocells return a logic 1 when windows and doors are closed:
  if( digitalRead(photocell_window) == LOW /* or digitalRead(photocell_door) == LOW */ )
    something_open = true;
    
  if (fans_status == 1) // aka the fans are running
    fans_timeout = fans_timeout - 1;
}

