/*
 This code will blink an LED attached to pin 13 on and off. 
 It will stay on for 0.25 seconds.
 It will stay off for 1 second.
 */
#include <Metro.h> //Include Metro library
#define LED1 22 
#define LED2 21
#define LED3 20
#define LED4 19

#define LED5 18 
#define LED6 17
#define LED7 16
#define LED8 15

#define LED9 8 
#define LED10 9
#define LED11 10
#define LED12 11

//Create a variable to hold theled's current state
int count = 0;

// Instanciate a metro object and set the interval to 250 milliseconds (0.25 seconds).
Metro ledMetro = Metro(250); 

void setup()
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  pinMode(LED3,OUTPUT); 
  pinMode(LED4,OUTPUT); 

  pinMode(LED5,OUTPUT);
  pinMode(LED6,OUTPUT); 
  pinMode(LED7,OUTPUT); 
  pinMode(LED8,OUTPUT);
}

void loop()
{
  if (ledMetro.check() == 1) {
    if (count == 0){
      digitalWrite(LED12,LOW);
      digitalWrite(LED1,HIGH);
    }
    if (count == 1){
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
    }
    if (count == 2){
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);
    }
    if (count == 3){
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,HIGH);
    }
    if (count == 4){
      digitalWrite(LED4,LOW);
      digitalWrite(LED5,HIGH);
    }
    if (count == 5){
      digitalWrite(LED5,LOW);
      digitalWrite(LED6,HIGH);
    }
    if (count == 6){
      digitalWrite(LED6,LOW);
      digitalWrite(LED7,HIGH);
    }
    if (count == 7){
      digitalWrite(LED7,LOW);
      digitalWrite(LED8,HIGH);
    }
    if (count == 8){
      digitalWrite(LED8,LOW);
      digitalWrite(LED9,HIGH);
    }
    if (count == 9){
      digitalWrite(LED9,LOW);
      digitalWrite(LED10,HIGH);
    }
    if (count == 10){
      digitalWrite(LED10,LOW);
      digitalWrite(LED11,HIGH);
    }
    if (count == 11){
      digitalWrite(LED11,LOW);
      digitalWrite(LED12,HIGH);
    }
    count++;
    if (count == 12){
      count = 0;
    }
  }
}

