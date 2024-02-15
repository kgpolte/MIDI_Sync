#include "MIDIUSB.h"

const int clockPin = 2;
const int runPin = 3;
int clockState = LOW;
const int clockPulseLength = 5; // milliseconds
int clockPulseTimer = 0;
unsigned long previousMillis = 0;

void readMIDI() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      if (rx.byte1 == 0xF8) {
        digitalWrite(clockPin, HIGH);
        clockState = HIGH;
        previousMillis = millis();
      } else if (rx.byte1 == 0xFA || rx.byte1 == 0xFB) {
        digitalWrite(runPin, HIGH);
      } else if (rx.byte1 == 0xFC) {
        digitalWrite(runPin, LOW);
      }
    }
  } while (rx.header != 0);
}

void updateClockOutput() {
  if (clockState == HIGH) {
    const long delta = millis() - previousMillis;
    if (delta >= clockPulseLength) {
      digitalWrite(clockPin, LOW);
      clockState = LOW;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, clockState);
}

void loop() {
  readMIDI();
  updateClockOutput();
}
