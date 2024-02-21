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

const int clockPin = 7;
const int runPin = 6;
unsigned long t0 = millis();
int clockState = LOW;
int clockLength = 5;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnClock()
{
  clockState = HIGH;
  digitalWrite(clockPin, HIGH);
  t0 = millis();
}

static void OnStart()
{
  digitalWrite(runPin, HIGH);
}

static void OnContinue()
{
  digitalWrite(runPin, HIGH);
}

static void OnStop()
{
  digitalWrite(runPin, LOW);
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
      digitalWrite(clockPin, LOW);
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

  pinMode(clockPin, OUTPUT);
  pinMode(runPin, OUTPUT);

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