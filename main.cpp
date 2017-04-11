#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

byte phareAuto = LOW;
volatile byte state = LOW;
volatile byte state1 = LOW;
volatile long t;
volatile long t1;
volatile byte boulboul = HIGH;

//fonction réaliser à l'interruption
void interruption(){
  state1 = !state1;
}

void setup() {
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  //interruption sur front montant du pin 4
  attachInterrupt(digitalPinToInterrupt(3), interruption, RISING);
  interrupts();
  t = millis();
  t1 = millis();
  t1 = 100000;
}

void loop() {
  digitalWrite(10,state1);
  int sensorVal=analogRead(A0);
  Serial.println(sensorVal);
   //gestion du nombre de clignotement lancé par pression des boutons :
  if(digitalRead(2)==HIGH){ //bouton gauche
    //gestion du clignotement, changement d'état toute les 500 millisconde :
    if((millis()-t) >= 500){
      tone(11,330,20);
      state = !state;
      t = millis();
    }
    digitalWrite(8,state);
  }else if(digitalRead(4)==HIGH){ //bouton droit
    if((millis()-t) >= 500){
      tone(11,330,20);
      state = !state;
      t = millis();
    }
    digitalWrite(9,state);
  }else{
    state = HIGH;
    digitalWrite(9,LOW);
    digitalWrite(8,LOW);
    t = millis();
  }

  //allume phare dans l'obscurité
  if(sensorVal<400 && boulboul){
    t1 = millis();
    boulboul = LOW;
    phareAuto = HIGH;
  }else if(sensorVal>400 && !boulboul){
    t1 = millis();
    boulboul = HIGH;
    phareAuto = HIGH;
  }
  if(phareAuto){
    if((millis() - t1) >= 5000){
      phareAuto = LOW;
      if(boulboul){
        state1 = LOW;
      }else if(!boulboul){
        state1 = HIGH;
      }
    }
  }
}
