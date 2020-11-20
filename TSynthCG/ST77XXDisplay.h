#define sclk 27
#define mosi 26
#define cs 2
#define dc 3
#define rst 9
#define DISPLAYTIMEOUT 700

#include <Adafruit_GFX.h>
#include "ST77XX_t3.h" // Local copy from TD1.48 that works for 0.96" IPS 160x80 display

#include "Fonts/Org_01.h"
#include "Yeysk16pt7b.h"
#include "Fonts/FreeSansBold18pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansOblique24pt7b.h"
#include "Fonts/FreeSansBoldOblique24pt7b.h"

#define PULSE 1
#define VAR_TRI 2
#define FILTER_ENV 3
#define AMP_ENV 4
/*
TSynth Display Options
Select ST77XXDisplayType as :-
1  Default ST7735 130 x80 as ElectroTechnique Build standard 
2  ST7789 240 x 240 Display PIMORONI PM476 1.3inch as DH TSynth DHmod1 build
3  ST7789 240 x 240 Display ADA fruit 1.4inc display 
4  ST7789 240 x 320 Display ADA fruit module  ,portrait mode
*/
#define ST77XXDisplayType 2


ST7735_t3 tft = ST7735_t3(cs, dc, mosi, sclk, rst);

String currentParameter = "";
String currentValue = "";
float currentFloatValue = 0.0;
String currentPgmNum = "";
String currentPatchName = "";
String newPatchName = "";
char * currentSettingsOption = "";
char * currentSettingsValue = "";
uint32_t currentSettingsPart = SETTINGS;
uint32_t paramType = PARAMETER;

boolean MIDIClkSignal = false;
uint32_t peakCount = 0;
uint16_t prevLen = 0;

uint32_t colour[NO_OF_VOICES] = {ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE, ST7735_BLUE};

unsigned long timer = 0;

void startTimer() {
  if (state == PARAMETER)
  {
    timer = millis();
  }
}

FLASHMEM void renderBootUpPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(42, 30, 46, 11, ST7735_WHITE);
  tft.fillRect(88, 30, 61, 11, ST7735_WHITE);
  tft.setCursor(45, 31);
  tft.setFont(&Org_01);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.println("ELECTRO");
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(91, 37);
  tft.println("TECHNIQUE");
  tft.setTextColor(ST7735_YELLOW);
  tft.setFont(&Yeysk16pt7b);
  tft.setCursor(5, 70);
  tft.setTextSize(1);
  tft.println("TSynth");
  tft.setTextColor(ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(110, 95);
  tft.println(VERSION);
  if(ST77XXDisplayType == 2){
	//20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(5, 145);
  tft.println("20201120CG");
  
  tft.drawLine(5, 170, 235 , 170, ST77XX_ORANGE);
  
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);  
  }
  

}

FLASHMEM void renderPeak() {
  int Vuy =223;
  if (vuMeter && peak.available()) {
    uint16_t len = 0;
    if (peakCount > 1) {
      len = (int)(peak.read() * 75.0f);
      prevLen = len;
      peakCount = 0;
    } else {
      len = prevLen;
      peakCount++;
    }
	if(ST77XXDisplayType == 1){
		int Vuy = 20;
	}
	else if(ST77XXDisplayType == 2){
		int Vuy = 223;
	}
	else if(ST77XXDisplayType == 3){
		int Vuy = 223;
	}
	else if(ST77XXDisplayType == 4){
		int Vuy = 300;
	}
    // DH relocating VU meter, Vuy sets location
    // DH set 3 colour VU level Green-yellow-red
    
    int Redlev = 42;
    int Yellowlev = 22;
    int yellowlen=0;
    int redlen = 0;
    if(len > Redlev){
     redlen = len - Redlev;
    
    tft.drawFastVLine(158, Vuy - len ,redlen , ST7735_RED);
    tft.drawFastVLine(158, Vuy - Redlev, Yellowlev , ST7735_YELLOW);
    tft.drawFastVLine(158, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    if(ST77XXDisplayType > 1){
      tft.drawFastVLine(159, Vuy - len ,redlen , ST7735_RED);
    tft.drawFastVLine(159, Vuy - Redlev, Yellowlev , ST7735_YELLOW);
    tft.drawFastVLine(159, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    tft.drawFastVLine(160, Vuy - len ,redlen , ST7735_RED);
    tft.drawFastVLine(160, Vuy - Redlev, Yellowlev , ST7735_YELLOW);
    tft.drawFastVLine(160, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    tft.drawFastVLine(161, Vuy - len ,redlen , ST7735_RED);
    tft.drawFastVLine(161, Vuy - Redlev, Yellowlev , ST7735_YELLOW);
    tft.drawFastVLine(161, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    tft.drawFastVLine(162, Vuy - len ,redlen , ST7735_RED);
    tft.drawFastVLine(162, Vuy - Redlev, Yellowlev , ST7735_YELLOW);
    tft.drawFastVLine(162, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    }
    
    
    
    }
    else if(len >Yellowlev){
      yellowlen = len - Yellowlev;
      
      tft.drawFastVLine(158, Vuy - len ,yellowlen , ST7735_YELLOW);
      tft.drawFastVLine(158, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
      if(ST77XXDisplayType > 1){
        tft.drawFastVLine(159, Vuy - len ,yellowlen , ST7735_YELLOW);
        tft.drawFastVLine(159, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    
        tft.drawFastVLine(160, Vuy - len ,yellowlen , ST7735_YELLOW);
        tft.drawFastVLine(160, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
    
        tft.drawFastVLine(161, Vuy - len ,yellowlen , ST7735_YELLOW);
        tft.drawFastVLine(161, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
        tft.drawFastVLine(162, Vuy - len ,yellowlen , ST7735_YELLOW);
        tft.drawFastVLine(162, Vuy - Yellowlev, Yellowlev , ST7735_GREEN);
        }
    
    
    }
    
    else {
      tft.drawFastVLine(158, Vuy - len, len ,  len > Redlev ? ST7735_RED : ST7735_GREEN);
      if(ST77XXDisplayType > 1){
        tft.drawFastVLine(159, Vuy - len, len ,  len > Redlev ? ST7735_RED : ST7735_GREEN);
        tft.drawFastVLine(160, Vuy - len, len ,  len > Redlev ? ST7735_RED : ST7735_GREEN);
        tft.drawFastVLine(161, Vuy - len, len ,  len > Redlev ? ST7735_RED : ST7735_GREEN);
        tft.drawFastVLine(162, Vuy - len, len ,  len > Redlev ? ST7735_RED : ST7735_GREEN);
      }
    }
    

    
  }
}

FLASHMEM void renderCurrentPatchPage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println(currentPgmNum);

  tft.setTextColor(ST7735_BLACK);
  tft.setFont(&Org_01);

  if (MIDIClkSignal) {
    tft.fillRect(100, 27, 14, 7, ST77XX_ORANGE);
    tft.setCursor(101, 32);
    tft.println("CK");
  }
  renderPeak();

  //      1 2 3 4 5 6 7 8 9 10 11 12
  //    1 B B B B B B B B B B B B
  //    2 B B B B B B Y Y Y Y Y Y
  //    3 B B B B O O O O Y Y Y Y
  //    4 B B B R O O O R R Y Y Y

  //V4
  if (voices[3].voiceOn == 1 && unison && notesOn == 4) {
    colour[3] = ST77XX_MAGENTA;
  } else if (voices[3].voiceOn == 1 && unison && notesOn > 1 && colour[3] != ST77XX_MAGENTA) {
    colour[3] = ST7735_BLUE;
  } else if (voices[3].voiceOn == 0) {
    colour[3] = ST7735_BLUE;
  }

  //V5-6
  if (voices[4].voiceOn == 1 && unison && notesOn > 2) {
    colour[4] = ST77XX_ORANGE;
    colour[5] = ST77XX_ORANGE;
  } else if (voices[4].voiceOn == 0) {
    colour[4] = ST7735_BLUE;
    colour[5] = ST7735_BLUE;
  }

  //V7
  if (voices[6].voiceOn == 1 && unison && notesOn > 2) {
    colour[6] = ST77XX_ORANGE;
  } else if (voices[6].voiceOn == 1 && unison && notesOn == 2 && colour[6] != ST77XX_ORANGE) {
    colour[6] = ST7735_YELLOW;
  } else if (voices[6].voiceOn == 0) {
    colour[6] = ST7735_BLUE;
  }

  //V8
  if (voices[7].voiceOn == 1 && unison && notesOn == 4) {
    colour[7] = ST77XX_MAGENTA;
  } else if (voices[7].voiceOn == 1 && unison && notesOn == 3 && colour[7] != ST77XX_MAGENTA) {
    colour[7] = ST77XX_ORANGE;
  } else if (voices[7].voiceOn == 1 && unison && notesOn == 2 && colour[7] != ST77XX_MAGENTA && colour[7] != ST77XX_ORANGE) {
    colour[7] = ST7735_YELLOW;
  } else if (voices[7].voiceOn == 0) {
    colour[7] = ST7735_BLUE;
  }

  //V9
  if (voices[8].voiceOn == 1 && unison && notesOn == 4) {
    colour[8] = ST77XX_MAGENTA;
  } else if (voices[8].voiceOn == 1 && unison && (notesOn == 2 || notesOn == 3) && colour[8] != ST77XX_MAGENTA) {
    colour[8] = ST7735_YELLOW;
  } else if (voices[8].voiceOn == 0) {
    colour[8] = ST7735_BLUE;
  }

  //V10-12
  if (voices[9].voiceOn == 1 && unison && notesOn > 1) {
    colour[9] = ST7735_YELLOW;
    colour[10] = ST7735_YELLOW;
    colour[11] = ST7735_YELLOW;
  } else if (voices[9].voiceOn == 0) {
    colour[9] = ST7735_BLUE;
    colour[10] = ST7735_BLUE;
    colour[11] = ST7735_BLUE;
  }

  if (voices[0].voiceOn == 1)   tft.fillRect(117, 27, 8, 8, ST7735_BLUE); else tft.drawRect(117, 27, 8, 8, ST7735_BLUE);
  if (voices[1].voiceOn == 1)   tft.fillRect(127, 27, 8, 8, ST7735_BLUE); else tft.drawRect(127, 27, 8, 8, ST7735_BLUE);
  if (voices[2].voiceOn == 1)   tft.fillRect(137, 27, 8, 8, ST7735_BLUE); else tft.drawRect(137, 27, 8, 8, ST7735_BLUE);

  if (voices[3].voiceOn == 1)   tft.fillRect(147, 27, 8, 8, colour[3]); else tft.drawRect(147, 27, 8, 8, ST7735_BLUE);
  if (voices[4].voiceOn == 1)   tft.fillRect(117, 37, 8, 8, colour[4]); else tft.drawRect(117, 37, 8, 8, ST7735_BLUE);
  if (voices[5].voiceOn == 1)   tft.fillRect(127, 37, 8, 8, colour[5]); else tft.drawRect(127, 37, 8, 8, ST7735_BLUE);

  if (voices[6].voiceOn == 1)   tft.fillRect(137, 37, 8, 8, colour[6]); else tft.drawRect(137, 37, 8, 8, ST7735_BLUE);
  if (voices[7].voiceOn == 1)   tft.fillRect(147, 37, 8, 8, colour[7]); else tft.drawRect(147, 37, 8, 8, ST7735_BLUE);
  if (voices[8].voiceOn == 1)   tft.fillRect(117, 47, 8, 8, colour[8]); else tft.drawRect(117, 47, 8, 8, ST7735_BLUE);

  if (voices[9].voiceOn == 1)   tft.fillRect(127, 47, 8, 8, colour[9]); else tft.drawRect(127, 47, 8, 8, ST7735_BLUE);
  if (voices[10].voiceOn == 1)  tft.fillRect(137, 47, 8, 8, colour[10]); else tft.drawRect(137, 47, 8, 8, ST7735_BLUE);
  if (voices[11].voiceOn == 1)  tft.fillRect(147, 47, 8, 8, colour[11]); else tft.drawRect(147, 47, 8, 8, ST7735_BLUE);

  tft.drawFastHLine(10, 63, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(1, 90);
  tft.setTextColor(ST7735_WHITE);
  tft.println(currentPatchName);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(10, 120, 235 , 120, ST7735_CYAN);
  /*
  tft.drawLine(10, 130, 235 , 130, ST7735_BLUE);
  tft.drawLine(10, 140, 235 , 140, ST7735_RED);
  tft.drawLine(10, 150, 235 , 150, ST7735_GREEN);
  tft.drawLine(10, 160, 235 , 160, ST7735_CYAN);
  tft.drawLine(10, 170, 235 , 170, ST7735_MAGENTA);
  tft.drawLine(10, 180, 235 , 180, ST7735_YELLOW);
  tft.drawLine(10, 190, 235 , 190, ST7735_WHITE);
  tft.drawLine(10, 200, 235 , 200, ST77XX_MAGENTA);
  tft.drawLine(10, 210, 235 , 210, ST77XX_ORANGE);
  */
  tft.drawLine(10, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderPulseWidth(float value) {
  tft.drawFastHLine(108, 74, 15 + (value * 13), ST7735_CYAN);
  tft.drawFastVLine(123 + (value * 13), 74, 20, ST7735_CYAN);
  tft.drawFastHLine(123 + (value * 13), 94, 16 - (value * 13), ST7735_CYAN);
  if (value < 0) {
    tft.drawFastVLine(108, 74, 21, ST7735_CYAN);
  } else {
    tft.drawFastVLine(138, 74, 21, ST7735_CYAN);
  }
}

FLASHMEM void renderVarTriangle(float value) {
  tft.drawLine(110, 94, 123 + (value * 13), 74, ST7735_CYAN);
  tft.drawLine(123 + (value * 13), 74, 136, 94, ST7735_CYAN);
}

FLASHMEM void renderEnv(float att, float dec, float sus, float rel) {
  tft.drawLine(100, 94, 100 + (att * 15), 74, ST7735_CYAN);
  tft.drawLine(100 + (att * 15), 74.0, 100 + ((att + dec) * 15), 94 - (sus * 20), ST7735_CYAN);
  tft.drawFastHLine(100 + ((att + dec) * 15), 94 - (sus * 20), 40 - ((att + dec) * 15), ST7735_CYAN);
  tft.drawLine(139, 94 - (sus * 20), 139 + (rel * 13), 94, ST7735_CYAN);
}

FLASHMEM void renderCurrentParameterPage() {
  switch (state) {
    case PARAMETER:
      tft.fillScreen(ST7735_BLACK);
      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(0, 53);
      tft.setTextColor(ST7735_YELLOW);
      tft.setTextSize(1);
      tft.println(currentParameter);
      if(ST77XXDisplayType > 1){
      //20201023 DH using extended Screen space of ST7789 240x240
      tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
      tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
      }

// Not a necessary feature perhaps
//      if (midiOutCh > 0) {
//        tft.setTextColor(ST77XX_ORANGE);
//        tft.setFont(&Org_01);
//        tft.setTextSize(2);
//        tft.setCursor(140, 35);
//        tft.println(midiOutCh);
//        tft.setFont(&FreeSans12pt7b);
//        tft.setTextSize(1);
//      }
      renderPeak();
      tft.drawFastHLine(10, 63, tft.width() - 20, ST7735_RED);
      tft.setCursor(1, 90);
      tft.setTextColor(ST7735_WHITE);
      tft.println(currentValue);
      if (pickUpActive) {
        tft.fillCircle(150, 70, 5, ST77XX_CYAN);
        tft.drawFastHLine(146, 70, 4, ST7735_WHITE);
      }
      switch (paramType) {
        case PULSE:
          renderPulseWidth(currentFloatValue);
          break;
        case VAR_TRI:
          renderVarTriangle(currentFloatValue);
          break;
        case FILTER_ENV:
          renderEnv(filterAttack * 0.0001f, filterDecay * 0.0001f, filterSustain, filterRelease * 0.0001f);
          break;
        case AMP_ENV:
          renderEnv(ampAttack * 0.0001f, ampDecay * 0.0001f, ampSustain, ampRelease * 0.0001f);
          break;
      }
      break;
  }
}

FLASHMEM void renderDeletePatchPage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Delete?");
  tft.drawFastHLine(10, 60, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 78);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 78);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);
  tft.fillRect(0, 85, tft.width(), 23, ST77XX_MAGENTA);
  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.first().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.first().patchName);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderDeleteMessagePage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setCursor(2, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Renumbering");
  tft.setCursor(10, 90);
  tft.println("SD Card");
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderSavePage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Save?");
  tft.drawFastHLine(10, 60, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 78);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches[patches.size() - 2].patchNo);
  tft.setCursor(35, 78);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches[patches.size() - 2].patchName);
  tft.fillRect(0, 85, tft.width(), 23, ST77XX_MAGENTA);
  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderReinitialisePage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(5, 53);
  tft.println("Initialise to");
  tft.setCursor(5, 90);
  tft.println("panel setting");
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderPatchNamingPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 53);
  tft.println("Rename Patch");
  tft.drawFastHLine(10, 63, tft.width() - 20, ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 90);
  tft.println(newPatchName);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void renderRecallPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 45);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 45);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);

  tft.fillRect(0, 56, tft.width(), 23, 0xA000);
  tft.setCursor(0, 72);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.first().patchNo);
  tft.setCursor(35, 72);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.first().patchName);

  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  patches.size() > 1 ? tft.println(patches[1].patchNo) : tft.println(patches.last().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  patches.size() > 1 ? tft.println(patches[1].patchName) : tft.println(patches.last().patchName);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void showRenamingPage(String newName) {
  newPatchName = newName;
}

FLASHMEM void renderUpDown(uint16_t  x, uint16_t  y, uint16_t  colour) {
  //Produces up/down indicator glyph at x,y
  tft.setCursor(x, y);
  tft.fillTriangle(x, y, x + 8, y - 8, x + 16, y, colour);
  tft.fillTriangle(x, y + 4, x + 8, y + 12, x + 16, y + 4, colour);
}

FLASHMEM void renderSettingsPage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 53);
  tft.println(currentSettingsOption);
  if (currentSettingsPart == SETTINGS) renderUpDown(140, 42, ST7735_YELLOW);
  tft.drawFastHLine(10, 63, tft.width() - 20, ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 90);
  tft.println(currentSettingsValue);
  if (currentSettingsPart == SETTINGSVALUE) renderUpDown(140, 80, ST7735_WHITE);
  if(ST77XXDisplayType > 1){
  //20201023 DH using extended Screen space of ST7789 240x240
  tft.drawLine(5, 120, 235 , 120, ST7735_CYAN);
  tft.drawLine(5, 235, 235, 235, ST7735_CYAN);
  }
}

FLASHMEM void showCurrentParameterPage( const char *param, float val, int pType) {
  currentParameter = param;
  currentValue = String(val);
  currentFloatValue = val;
  paramType = pType;
  startTimer();
}

FLASHMEM void showCurrentParameterPage(const char *param, String val, int pType) {
  if (state == SETTINGS || state == SETTINGSVALUE)state = PARAMETER;//Exit settings page if showing
  currentParameter = param;
  currentValue = val;
  paramType = pType;
  startTimer();
}

FLASHMEM void showCurrentParameterPage(const char *param, String val) {
  showCurrentParameterPage(param, val, PARAMETER);
}

FLASHMEM void showPatchPage(String number, String patchName) {
  currentPgmNum = number;
  currentPatchName = patchName;
}

FLASHMEM void showSettingsPage(char *  option, char * value, int settingsPart) {
  currentSettingsOption = option;
  currentSettingsValue = value;
  currentSettingsPart = settingsPart;
}

FLASHMEM void enableScope(boolean enable) {
  enable ? scope.ScreenSetup(&tft) : scope.ScreenSetup(NULL);
}

void displayThread() {
  threads.delay(2000); //Give bootup page chance to display
  while (1) {
    switch (state) {
      case PARAMETER:
        if ((millis() - timer) > DISPLAYTIMEOUT) {
          pickUpActive = false;
          renderCurrentPatchPage();
        } else {
          pickUpActive = pickUp;
          renderCurrentParameterPage();
        }
        break;
      case RECALL:
        renderRecallPage();
        break;
      case SAVE:
        renderSavePage();
        break;
      case REINITIALISE:
        renderReinitialisePage();
        tft.updateScreen(); //update before delay
        threads.delay(1000);
        state = PARAMETER;
        break;
      case PATCHNAMING:
        renderPatchNamingPage();
        break;
      case PATCH:
        renderCurrentPatchPage();
        break;
      case DELETE:
        renderDeletePatchPage();
        break;
      case DELETEMSG:
        renderDeleteMessagePage();
        break;
      case SETTINGS:
      case SETTINGSVALUE:
        renderSettingsPage();
        break;
    }
    tft.updateScreen();
  }
}

void setupDisplay() {
  tft.useFrameBuffer(true);
  if(ST77XXDisplayType== 1)
  {
   tft.initR(INITR_GREENTAB);
   tft.setRotation(3);
  }
  else if(ST77XXDisplayType == 2){
  //DH 7789 init
  tft.init(240,240,0);
  // DH set rotatio from 3 to 0 for Ada fruit 240x240 screen
  tft.setRotation(0);
  }
  else if(ST77XXDisplayType == 3){
  //DH 7789 init
  tft.init(240,240,0);
  // DH set rotatio from 3 to 0 for Ada fruit 240x240 screen
  tft.setRotation(0);
  }
  else if(ST77XXDisplayType == 4){
  //DH 7789 init
  tft.init(240,320,0);
  // DH set rotatio from 3 to 0 for Ada fruit 240x320 screen Only working in portrait mode
  tft.setRotation(2);
  }
  
  tft.invertDisplay(true);
  renderBootUpPage();
  tft.updateScreen();
  threads.addThread(displayThread);
}
