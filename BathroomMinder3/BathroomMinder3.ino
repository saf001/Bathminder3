#include <ChainableLED.h>
#include "pitches.h"
#define DEBUG  false

/*
  Bathroom Minder - Shawn Freeman
  https://www.instructables.com/member/shawnf2/
  https://github.com/saf001/Bathminder

  The idea is fairly straight-forward: When the bathroom door is closed, the alarm is armed.
  Once armed a countdown begins.

  System is unarmed, regardless of current operating state, whenever the door is open.

  Since this is very task specific, the countdown times are hard-coded, but can be changed, and the code recompiled.
  It is not my intent to make them a variable input to the device.

  This example code is in the public domain, and uses code and lessons learned from these other projects:

  http://www.arduino.cc/en/Tutorial/Blink
  http://www.arduino.cc/en/Tutorial/Tone

  The countdown occurs in stages. Each stage having the same amount of time (the same time interval).
  The total time for the entire countdown can be determined by multiplying the time interval by the number of stages.
  For example, with an interval of 5 minutes and 4 stages, the total time would be 5 * 4 = 20 minutes.

  Note that when the reed switch closes, the Arduino is powered up and immediately begins running the program.
*/

// Time step interval in minutes
// Knowing that I was going to have 3 stages, I opted for 12 minutes to give the occupant a total of 36 minutes before the alarm would sound.
unsigned long TimeInt = 12;

// The number of LEDs in the chain determines the number of stages (e.g. 4 LEDS = 4 stages, 1 LED = 1 stage).
// Here, I am using 3 chained LEDs (total time = 12 * 3 = 36 minutes)
#define NUM_LEDS  3
// target pins to use for LEDs (6 and 7 on the Mega)
ChainableLED leds(6, 7, NUM_LEDS);

// target pins to use for speaker
int SPK = 8;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize LEDs.
  leds.init();
  // setup up serial output for debugging
  if (DEBUG) {
    Serial.begin(9600);
  }
}

// the loop function runs over and over again forever
void loop() {
  // for each one of the LEDs in the chain...
  for (byte i = 0; i < NUM_LEDS; i++) {
    // sound a chime at the start of the countdown...
    Chime(SPK);
    // and blink it blue and green until the timer (for this interval) runs out, then turn red.
    BlinkRGB(i);
  }
  // after looping through the whole chain, all the intervals are done and the time is at zero - sound the alarm.
  Alarm(SPK);
}

// function to blink one LED in the chain
void BlinkRGB(byte led)
{
  if (DEBUG) {
    Serial.println("in: BlinkRGB");
  }
  // convert the time interval from minutes to milliseconds
  unsigned long dly = (TimeInt) * 60000;
  // get the start time for this interval (I tried using a routine based on delay, but that didn't work nearly as well)
  unsigned long now = millis();
  // set the end of this internal as dly milliseconds from the start of the interval
  unsigned long later = now + dly;

  // prototype for the setColorRGB function - useful to know
  // void setColorRGB(byte led, byte red, byte green, byte blue)

  // while now is less than later, blink the LED
  while (now < later) {
    if (DEBUG) {
      Serial.println("blink blue and green");
    }
    // turn the LED blue
    leds.setColorRGB(led, 0, 0, 255); //blue
    delay(2000);
    // turn the LED green
    leds.setColorRGB(led, 0, 255, 0); //green
    delay(2000);
    // update now to the now current time
    now = millis();
  }
  //now finally caught up to later, meaning that this interval is done - turn the LED red
  leds.setColorRGB(led, 255, 0, 0); //red
}

// this is the alarm function; I wanted it to have that annoying British police car sound to be most effective
void Alarm(int spk_pin)
{
  if (DEBUG) {
    Serial.println("in: Alarm");
  }

  // we're just going to place this forever, at least as long as the Arduino has power
  while (true) {
    tone(spk_pin, NOTE_C5, 500);
    delay(500);
    tone(spk_pin, NOTE_G5, 500);
    delay(500);
  }
}

// this is the chime function that is played at the start of each interval - just a simple ding-dong style tone
void Chime(int spk_pin) {
  if (DEBUG) {
    Serial.println("in: Chime");
  }
  tone(spk_pin, NOTE_C7, 500);
  delay(500);
  tone(spk_pin, NOTE_G7, 500);
  delay(500);
}



