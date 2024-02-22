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
*/

#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

unsigned long t0 = millis();
int clockState = LOW;
int clockLength = 3;
byte pinStates = B00000000;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

static void OnClock()
{
  clockState = HIGH;
  pinStates += B00000001;
  PORTF = pinStates;
  t0 = millis();
}

static void OnStart()
{
  pinStates += B00000010;
  PORTF = pinStates;
}

static void OnContinue()
{
  pinStates += B00000010;
  PORTF = pinStates;
}

static void OnStop()
{
  pinStates -= B00000010;
  PORTF = pinStates;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void updateClock() 
{
  // Set clock output LOW after if the set trigger time has elapsed
  if (clockState == HIGH) {
    if (millis() - t0 >= clockLength) {
      clockState = LOW;
      pinStates -= B00000001;
      PORTF = pinStates;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  // Set PD6 and PD7 to outputs
  // PD6: pin D12, clock
  // PD7: pin D6, run
  DDRF = B00000011;

  MIDI.begin();
  MIDI.setHandleClock(OnClock);
  MIDI.setHandleStart(OnStart);
  MIDI.setHandleContinue(OnContinue);
  MIDI.setHandleStop(OnStop);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();
  updateClock();
}