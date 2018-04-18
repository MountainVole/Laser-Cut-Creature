/*
 * This is the revised creature with tail servo for comic expo 2018
 * Tail is driven in a sin wave motion
 */
#include <Servo.h>
#include "FastLED.h"
#define heartOUT A0 //Where the neopixel jewel is connected
#define nerveB A3 //Where breath sensor is connected
#define nerveH A4 //Where heart sensor is connected
#define nerveT A5 //Where tail sensor is connected
#define fwd 6
#define bwd 11

CRGBArray<7> heart;

Servo tail;  // create servo object to control a servo
// twelve servo objects can be created on most boards

unsigned long prevMillis = 0;
float pos = 0;    // variable to store the servo position
float lastpos=0;
float rev;
long HeartRate = 90, interBeat = 33, hrState = 1; //Heartrate in BPM and the time between lub and dub as a percentage
long hrMillis, beatMillis, nextBeat;
int inB, inH, inT, preT;
float theta=0;

void setup() {
  tail.attach(13);  // attaches the servo on pin 13, last cmdr servo point
  LEDS.addLeds<NEOPIXEL, heartOUT>(heart, 7);
  hrMillis = 60000 / HeartRate; //Beats per Minute, divided my seconds per minute, times millis per second
  beatMillis = (hrMillis * interBeat) / 100; //hrMillis reduced to the percentage of interBeat. This is mostly for testing to get the right "Feel"
  prevMillis = nextBeat = 0;
  pinMode(fwd, OUTPUT);
  pinMode(bwd, OUTPUT);
  Serial.begin(9600);
  preT=1;
}

void loop() {
  //float theta = millis ()* 0.0018 ;
  //pos=90 + 80 * sin (theta);
  //tail.write (pos) ;
  //Heart();
  //analogWrite(bwd,200);*/ //This logic runs tail, heart, and lungs with no feedback
  inB=analogRead(nerveB);
  inH=analogRead(nerveH);
  inT=analogRead(nerveT);
  //scaled motor logic
  if (inB>10) inB=map(inB,10,1024,35,100);
  else inB=0;
  analogWrite(fwd,inB);
  //Scaled logic heart
  HeartRate=map(inH,0,1024,50,170);
  hrMillis = 60000 / HeartRate; //Beats per Minute, divided my seconds per minute, times millis per second
  beatMillis = (hrMillis * interBeat) / 100; //hrMillis reduced to the percentage of interBeat. This is mostly for testing to get the right "Feel"
  Heart();
  //Scaled logic tail
  inT=map(inT,0,1024,0,60);
  float interval=inT*0.0001;
  theta +=interval;
  pos=90 + 80 * sin (theta);
  tail.write (pos);
}

void Heart() {
  if ((millis() - prevMillis) > nextBeat)
  {
    //Time for a heartbeat
    for (int i = 0; i < 7; i++) {
      heart[i] = CRGB(255, 50, 5);
    }
    FastLED.show();
    prevMillis = millis();
    switch (hrState)
    {
      case 0:
        nextBeat = beatMillis;
        hrState = 1;
        break;
      case 1:
        nextBeat = hrMillis - beatMillis;
        hrState = 0;
        break;
      default:
        break;
    }
  }
  else if ((millis() % 10) == 0) {
    //Fade between heartbeats
    if (hrState == 0) {
      for (int i = 0; i < 7; i++)
      {
        heart[i].nscale8(235);//Dims all LEDs between beets
      }
    }
    else{
      for (int i = 0; i < 7; i++)
      {
        heart[i].nscale8(220);//Dims all LEDs between lub and dub
      }
    }
    FastLED.show();
  }
}

  
