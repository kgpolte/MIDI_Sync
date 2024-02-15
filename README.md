# MIDI_Sync
 
 ## v0.3 BETA
 
# Changes:
Switched from using the Adafruit MIDIUSB library to Lathoub's [Arduino-USBMIDI](https://github.com/lathoub/Arduino-USBMIDI/tree/master), which required a complete rewriting of the code. This was necessary to fix issues with inconsistent clock pulses. The 24ppqn output is much smoother now, but could still probably be improved.
	
Went back to using digitalWrite() to update the output pins. This allows the code to remain a bit more modular in anticipation of future updates including additional outputs and controls.
 
# Description:
 
A super-simple MIDI clock/run sync utility for Eurorack.

Designed to sync Pam's Pro Workout with a DAW.

Outputs a 24ppqn clock signal and a gate that stays high during playback and recording.
