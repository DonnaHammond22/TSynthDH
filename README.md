# TSynthDH
Alternative Firmware and Hardware development for TSynth DIY Polyphonic Synthesizer from EletroTechnique.
Hi I am Donna Hammond aka leaFSMith.This GitHub repository is for my project to provide an alternative Firmware and hardware 
development for the TSynth DIY Polyphonic Music Synthesizer from ElectroTechnique which was released in 2020.
See link

Project start and goals
A]take the design info for Tsynth version for 4.1 Teensy build a prototype based on the original Schematic and verify
B]Implement a modified  Hardware and firmware to support an ST7789 based 240 x 240 Display [original was ST7735 160 x 80].-"TSynthDHmod1"
C]Implement and release a series of Firmware versions to support "TSynthDHmod1"

LOG Details
2020-11-19 first version "CF" uploaded, based on TSynth release 2.02 .verified on TSynth board version 1.2 for Teensy4.1 with Pimoroni PM476 1.3inch 240 x240 Display 
Note the Pimoroni display does not use the Reset signal. This release has also been verified with an Ada fruit 240 x240 display ST7789.The files altered where:-
ST7735_t3.cpp ,ST7735_t3.h and ST7735Display. The Oscilloscope trace was relocated to a clear display space.The VU meter was relocated in the display next to the Oscilloscope. The VU meter was given a thicker 3 colour (red,yellow green) representation.

2020-11-20 CG version uploaded,as CF but code tidied up with Display development supported as a generic set of ST77XX files with options selection of Display type screen size resolution using code variable
 ST77XXDisplayType as :-
1  Default ST7735 160 x80 as ElectroTechnique Build standard 
2  ST7789 240 x 240 Display PIMORONI PM476 1.3inch as DH TSynth DHmod1 build
3  ST7789 240 x 240 Display ADA fruit 1.4inc display 
4  ST7789 240 x 320 Display ADA fruit module  ,portrait mode
Also included new setting EFXSelEnable place holder no functionality yet ,but will follow

2020-11-09  DK version uploaded , based on TSynth release 2.03 added new Arbitrary waveforms for OSC A and OSC B based on 256 point values pof a single cycle
Folder includes example spreadsheet on how to create and import your own waveform files . Process was to take publically avaible downloadable  waveforms ,eg 
https://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/individual-folder-downloads-of-the-akwf-pack/
which are single cycles expressed as 600 point WAV format files, and process them in firstly Audacity to export as a readable data set , and secondly to re-map the data as a 256 integer value array scaled to range +/-4000.The new waveform data array was added to constants.h file, and the newwaveform implentation was supported by making changes to TSynth.ino file .TIP use you code text editor [I use Notepad++] serch for Parabolic, to find the places in the code where you need to add your new waveform.
