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
const unsigned long TIME_INTERVAL_MINS = 12;
const unsigned long MS_PER_MINUTE = 60000UL;
const unsigned long BLINK_INTERVAL_MS = 2000UL;

#define NUM_LEDS  3
// Chained LEDs on pins 6 and 7
ChainableLED leds(6, 7, NUM_LEDS);

// Speaker Pin
const int SPK = 8;

void setup() {
  leds.init();
  if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    Chime(SPK);
    BlinkRGB(i);
  }
  // All stages done, sound the continuous alarm
  Alarm(SPK);
}

// Non-blocking RGB blinking function
void BlinkRGB(byte led) {
  if (DEBUG) {
    Serial.println("in: BlinkRGB");
  }

  unsigned long intervalDuration = TIME_INTERVAL_MINS * MS_PER_MINUTE;
  unsigned long stageStartTime = millis();
  unsigned long lastBlinkTime = 0;
  bool toggleColor = false;

  // Run loop until the full interval duration has elapsed safely
  while (millis() - stageStartTime < intervalDuration) {
    
    // Check if it's time to toggle the blinking color (every 2000ms)
    if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
      lastBlinkTime = millis();
      toggleColor = !toggleColor;

      if (toggleColor) {
        leds.setColorRGB(led, 0, 0, 255);  // Blue
        if (DEBUG) Serial.println("Blink: Blue");
      } else {
        leds.setColorRGB(led, 0, 255, 0);  // Green
        if (DEBUG) Serial.println("Blink: Green");
      }
    }
    
    // Tiny delay prevents hammering the CPU unnecessarily but keeps it highly responsive
    delay(50); 
  }

  // Interval complete - Turn LED Red
  leds.setColorRGB(led, 255, 0, 0);
}

void Alarm(int spk_pin) {
  if (DEBUG) {
    Serial.println("in: Alarm");
  }
  while (true) {
    tone(spk_pin, NOTE_C5, 500);
    delay(500);
    tone(spk_pin, NOTE_G5, 500);
    delay(500);
  }
}

void Chime(int spk_pin) {
  if (DEBUG) {
    Serial.println("in: Chime");
  }
  tone(spk_pin, NOTE_C7, 500);
  delay(500);
  tone(spk_pin, NOTE_G7, 500);
  delay(500);
}



