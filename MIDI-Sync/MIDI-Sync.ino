/*
MIT License

Copyright (c) 2024 Kenny Polte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



-- Eurorack MIDI Clock & Run Sync --

A DIY project built specifically to sync Pam's Pro Workout with my DAW's clock.
Accepts standard MIDI messages via USB and outputs a 24ppqn CLOCK and a steady 
RUN gate during playback/recording.

This sketch is written for the Arduino Micro and uses direct addressing to
update the outputs. Make sure you update the port register values in this code when
using a different device or changing pins.

NOTE THE PHYSICAL PIN NUMBERS:
  clock output - D2;
  run output - D8;

Outputs should be buffered. In my design, I am using opamp-based comparators to
boost the voltage to Eurorack level (+10V). See the included schematic.

*/

#include "MIDIUSB.h"

int clockState = LOW;
const int clockPulseLength = 5; // milliseconds
int clockPulseTimer = 0;
unsigned long startMillis = 0;

void readMIDI() {
  midiEventPacket_t rx;

  // Read all incoming MIDI messages
  // Process clock, start/continue, and stop signals
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.byte1 == 0xF8) {
        PORTD = B0000010;
        clockState = HIGH;
        startMillis = millis();
      } else if (rx.byte1 == 0xFA || rx.byte1 == 0xFB) {
        PORTB = B00010000;
      } else if (rx.byte1 == 0xFC) {
        PORTB = B00000000;
      }
    }
  } while (rx.header != 0);
}

void updateClockOutput() {

  // Update the clock gate timer
  // Turn the gate to low if the set amount of time has elapsed
  if (clockState == HIGH) {
    const long delta = millis() - startMillis;
    if (delta >= clockPulseLength) {
      PORTD = B00000000;
      clockState = LOW;
    }
  }
}

void setup() {
  Serial.begin(115200);
  DDRD = B11111111;
  DDRB = B11111111;
}

void loop() {
  readMIDI();
  updateClockOutput();
}
