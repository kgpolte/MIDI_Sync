# MIDI_Sync
 
## Recent Changes:
Switched from using the Adafruit MIDIUSB library to Lathoub's [Arduino-USBMIDI](https://github.com/lathoub/Arduino-USBMIDI/tree/master), which required a complete rewriting of the code. This was necessary to fix issues with inconsistent clock pulses. The 24ppqn output is much smoother now, and perfectly usable, but could still probably be improved.
	
I've been waffling on using digitalWrite() vs. direct port manipulation but have decided to stick with the latter for now as Pam's Pro workout seems to freak out a lot less, even though the clock traces look more or less the same either way, interestingly.
 
## Description:
 
A super-simple MIDI clock/run sync utility for Eurorack.

Designed to sync Pam's Pro Workout with a DAW over USB using an Arduino. Outputs a 24ppqn clock signal and a gate that stays high during playback and recording.

I designed this layout and code specifically for the Arduino Micro, so make sure you make the appropriate changes to the code and schematic when using different hardware or output pins!

## TODO:

1. Implement averaging for the timing between clock pulses.

2. Verify the hardware schematic and perfboard layout.
