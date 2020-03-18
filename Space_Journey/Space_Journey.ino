/* Copyright (c) 2020 LeRoy Miller
 *  
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
 */

/*
 * 
 * "Space Journey" based on the solo game by Tarcisio Lucas
 * TridentTD ESP TrueRandom code by TridentTD https://github.com/TridentTD/TridentTD_ESP_TrueRandom
 * 
 */
 
#include <M5Stack.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "TridentTD_ESP_TrueRandom.h"

int experience = 0;
int resistance = 10;
int days = 30;

String encounterList[7] = {"", "Pirate Ship", "Cosmic Entity", "Infected Ship", "Enemy Fleet", "Ancestral Creature", "Bounty Hunter"};
int encounterHits[7] = {0,3, 5, 4, 4, 5, 3};
int encounterModifiers[7] = {0,0,0,0,0,0,0};
String treasureTable[7] = {"","Weapon","Force Field", "Supplies", "Warp Hole", "Laser", "Aux Spaceship"};
//maybe should show who or what the treasure increases are for (?) IE: "", "Against Pirate", "against Cosmic Entity", "", "", "against Ancestral Creature", "against Bounty Hunter" 
String trapTable[7] = {"","Asteroid Circle", "Solar Storm", "Dimensional Portal", "Treasure Theft", "Damaged Ship", "Supernova"};
int roll;
int modifier = 0;

void setup(void) {

  M5.begin();
 titleScreen();
  while(!M5.BtnB.wasReleased()) {
    M5.update();
    if (M5.BtnA.wasReleased()) {
      tellStory();
    }
    else if (M5.BtnC.wasReleased()) {
      showCredits();
    }
  }
}

void loop() {
M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
M5.Lcd.setTextDatum(MC_DATUM);
M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
displayStatus();
M5.Lcd.setTextDatum(TL_DATUM);
M5.Lcd.drawString("2 Exp used to repair ship", 0, 130, GFXFF);
M5.Lcd.drawString("3 Exp +1 Weapons for 1 turn", 0, 150, GFXFF);
gameButtons();

M5.update();
  while(!M5.BtnB.wasReleased()) {
   M5.update();
   if (M5.BtnA.wasReleased()) {
      increaseWeapons();
      gameButtons();
    }
    else if (M5.BtnC.wasReleased()) {
      repair();
      gameButtons();
    }
  }
M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
switch(esp.random(1,7)) {
  case 1:
  encounter();
  break;
  case 2:
  rest();
  break;
  case 3:
  treasure();
  break;
  case 4:
  trap();
  break;
  case 5:
  //special
  rest();
  break;
  case 6:
  encounter();
  break;
}
days --;
if (days == 0 && resistance <= 0 ) { 
  M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setFreeFont(FF20);
  M5.Lcd.drawString("Game Over", 160, 100, GFXFF);
  while(1);
} else if (days == 0 && resistance > 0) {
  M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
   M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setFreeFont(FF7);
  M5.Lcd.drawString("You saved", 160, 80, GFXFF);
  M5.Lcd.drawString("the Universe!", 160, 120, GFXFF);
  while(1);
} else if (resistance <= 0) {
  M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setFreeFont(FF20);
  M5.Lcd.drawString("You lost", 160, 80, GFXFF);
  M5.Lcd.drawString("Game Over", 160, 120, GFXFF);
  while(1);
  }
}

void gameButtons() {
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
M5.Lcd.setFreeFont(FF1);
M5.Lcd.setTextDatum(MC_DATUM);
M5.Lcd.drawString("Repair", 260, 220, GFXFF); //Button C
M5.Lcd.setTextColor(TFT_RED, TFT_BLACK); 
M5.Lcd.drawString("Weapon+", 60, 220, GFXFF); //Button A
M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
M5.Lcd.drawString("Roll", 160,220, GFXFF); //Button B
}

void increaseWeapons() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
M5.Lcd.setFreeFont(FF1);
M5.Lcd.setTextDatum(MC_DATUM);
if (experience >= 3 && modifier == 0) { experience = experience - 3; modifier ++; M5.Lcd.drawString("Weapon Power Increased",160,140,GFXFF);} 
else if (modifier > 0) {M5.Lcd.drawString("Weapons at Max",160,140,GFXFF);} else if (experience < 3) { M5.Lcd.drawString("Need Experience.",160,140,GFXFF);}
displayStatus();
//buttonContinue();
}

void repair() {
   M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
M5.Lcd.setFreeFont(FF1);
M5.Lcd.setTextDatum(MC_DATUM);
if (experience >= 2 && resistance < 10) { experience = experience - 2; resistance++; M5.Lcd.drawString("Resistance Increased",160,140,GFXFF);} 
else if (resistance == 10) {M5.Lcd.drawString("Resistance at Max",160,140,GFXFF);} else if (experience < 2) { M5.Lcd.drawString("Need Experience.",160,140,GFXFF);}
displayStatus();
//buttonContinue();
}

void displayStatus() {
  M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setFreeFont(FF5);
    M5.Lcd.drawString("Resistance: ", 0,70, GFXFF);
    M5.Lcd.drawString((String)resistance,130, 70, GFXFF);
    M5.Lcd.drawString("Experience: ", 160, 70,GFXFF); //0,20
    M5.Lcd.drawString((String)experience, 290, 70, GFXFF); //160,20
    M5.Lcd.drawString("Days Left : ", 0, 85, GFXFF);
    M5.Lcd.drawString((String)days, 130,85, GFXFF);
    M5.Lcd.drawString("Modifier  : ", 160,85, GFXFF);
    M5.Lcd.drawString((String)modifier, 290, 85, GFXFF);
    M5.Lcd.setFreeFont(FF1);
    M5.Lcd.setTextDatum(MC_DATUM);
}

void encounter() {
  
  int ranEncounter = esp.random(1,7);
 
  do {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
    M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
    M5.Lcd.setFreeFont(FF1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString(encounterList[ranEncounter], 160,120, GFXFF);
    roll = esp.random(1,7);
    if (roll + modifier + encounterModifiers[ranEncounter] < encounterHits[ranEncounter]) {M5.Lcd.drawString("You took a hit", 160,140, GFXFF); resistance --; modifier = 0; encounterModifiers[ranEncounter] = 0;} else if (roll + modifier + encounterModifiers[ranEncounter] >= encounterHits[ranEncounter]) {M5.Lcd.drawString("Enemy Destroyed",160,140, GFXFF); experience++; modifier = 0; encounterModifiers[ranEncounter] = 0;}
    displayStatus();
    buttonContinue();
   if (resistance <= 0) {break;}
  } while (roll < encounterHits[ranEncounter]);

}

void treasure() {
  
  int ranEncounter = esp.random(1,7);
 
  
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
    M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
    M5.Lcd.setFreeFont(FF1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString("You found: ",160,100, GFXFF);
    M5.Lcd.drawString(treasureTable[ranEncounter],160,120,GFXFF);
    //M5.Lcd.setTextDatum(TL_DATUM);
    if (ranEncounter == 1 || ranEncounter == 2 || ranEncounter == 5 || ranEncounter == 6) {M5.Lcd.drawString("Weapons Increased", 160, 140, GFXFF); encounterModifiers[ranEncounter] = 1;}
    if (ranEncounter == 3 && resistance <= 8) {M5.Lcd.drawString("Resistance Increased", 160, 140, GFXFF); resistance = resistance + 2;} else if (ranEncounter == 3 && resistance >=10) {M5.Lcd.drawString("Resistance already at Max", 160, 140, GFXFF); }
    if (ranEncounter == 4) {M5.Lcd.setTextDatum(TL_DATUM); M5.Lcd.drawString("You gained 2 more days, & Exp", 0, 140, GFXFF); days--; experience=experience+2;}
 M5.Lcd.setTextDatum(MC_DATUM);
  buttonContinue();
}

void trap() {
  
  int ranEncounter = esp.random(1,7);
 
  
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
    M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
//M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setFreeFont(FF1);
  //M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    //M5.Lcd.drawString("Encounter with", 160, 105, GFXFF);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString("Trap Encountered: ", 160,100, GFXFF);
    M5.Lcd.drawString(trapTable[ranEncounter], 160, 120, GFXFF);
    if (ranEncounter ==  1 || ranEncounter == 3 || ranEncounter == 5 ) {M5.Lcd.drawString("Hard Hit Resistance Decreased by 2", 0, 140, GFXFF); resistance = resistance -2;}
    if (ranEncounter == 2 ) {M5.Lcd.drawString("Bad Hit Resistance Decreased by 1", 0, 140, GFXFF); resistance--;}
    if (ranEncounter == 4) {M5.Lcd.drawString("Lose All Treasures", 0, 140, GFXFF); for (int i=0; i<7; i++) { encounterModifiers[i] = 0; }}
    if (ranEncounter == 6) {M5.Lcd.drawString("Critcal Hit Lose 5 resistance", 0, 140, GFXFF); resistance = resistance - 5; if (resistance < 0) {resistance = 0;}}
 M5.Lcd.setTextDatum(MC_DATUM);
  buttonContinue();
}


void rest() {
  M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
    M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
  M5.Lcd.setFreeFont(FF1);
  displayStatus();
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.drawString("Nothing happen today", 160, 130, GFXFF);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  //if (resistance < 10) {resistance++; M5.Lcd.drawString("Resistance Increase", 160, 120, GFXFF);}
  buttonContinue();
}

void titleScreen() {
   M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
  M5.Lcd.setFreeFont(FF20); //FF7);                 // Select the font
  M5.Lcd.drawString("Space Journey", 160, 40, GFXFF);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("by LeRoy Miller", 160,70,GFXFF);
  M5.Lcd.drawString("Based on the solo game", 0, 100, GFXFF);
  M5.Lcd.drawString("by Tarcisio Lucas", 150, 130, GFXFF);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  M5.Lcd.setFreeFont(FF1);
  M5.Lcd.drawString("Story", 60, 220, GFXFF);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.drawString("Start", 160,220, GFXFF);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  M5.Lcd.drawString("Credits", 260, 220, GFXFF);
}

void tellStory() {
  M5.Lcd.fillScreen(TFT_BLACK);  // Clear screen
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("Space Journey",160,20, GFXFF);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawString("The Great Galactic War", 100,40, GFXFF);
  M5.Lcd.drawString("has destroyed almost", 160, 60, GFXFF);
  M5.Lcd.drawString("every corner of the", 160, 80, GFXFF);
  M5.Lcd.drawString("universe. You are", 160,100, GFXFF);
  M5.Lcd.drawString("part of the resistance,", 160,120, GFXFF);
  M5.Lcd.drawString("soldiers who are loyal", 160, 140, GFXFF);
  M5.Lcd.drawString("to the Great Galactic", 160,160, GFXFF);
  M5.Lcd.drawString("Kingdom. ", 160,180,GFXFF);
  M5.Lcd.drawString("", 160, 200, GFXFF);
  buttonContinue();
  M5.Lcd.fillScreen(TFT_BLACK);  // Clear screen
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("The war is not going",160,20, GFXFF);
  M5.Lcd.drawString("well for the resistance", 100,40, GFXFF);
  M5.Lcd.drawString("Unexpectedly you have", 160, 60, GFXFF);
  M5.Lcd.drawString("received information", 160, 80, GFXFF);
  M5.Lcd.drawString("that could change the", 160,100, GFXFF);
  M5.Lcd.drawString("out come of the war.", 160,120, GFXFF);
  M5.Lcd.drawString("", 160, 140, GFXFF);
  M5.Lcd.drawString("It is believed that", 160,160, GFXFF);
  M5.Lcd.drawString("subspace communications", 160,180,GFXFF);
  M5.Lcd.drawString("have been compromised,", 160, 200, GFXFF);
  buttonContinue();
  M5.Lcd.fillScreen(TFT_BLACK);  // Clear screen
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("And you have been",160,20, GFXFF);
  M5.Lcd.drawString("tasked to deliever the", 100,40, GFXFF);
  M5.Lcd.drawString("valuable information to", 160, 60, GFXFF);
  M5.Lcd.drawString("the leader personally.", 160, 80, GFXFF);
  M5.Lcd.drawString("", 160,100, GFXFF);
  M5.Lcd.drawString("You have 30 days to", 160,120, GFXFF);
  M5.Lcd.drawString("make your journey to", 160, 140, GFXFF);
  M5.Lcd.drawString("the capital planet of", 160,160, GFXFF);
  M5.Lcd.drawString("Haratha.", 160,180,GFXFF);
  M5.Lcd.drawString("", 160, 200, GFXFF);
  buttonContinue();
  M5.Lcd.fillScreen(TFT_BLACK);  // Clear screen
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("",160,20, GFXFF);
  M5.Lcd.drawString("", 100,40, GFXFF);
  M5.Lcd.drawString("Good Luck.....", 160, 60, GFXFF);
  M5.Lcd.drawString("", 160, 80, GFXFF);
  M5.Lcd.drawString("", 160,100, GFXFF);
  M5.Lcd.drawString("The fate of the", 160,120, GFXFF);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.drawString("Universe", 160, 140, GFXFF);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawString("depends on you.....", 160,160, GFXFF);
  M5.Lcd.drawString("", 160,180,GFXFF);
  M5.Lcd.drawString("", 160, 200, GFXFF);
  buttonContinue();
  titleScreen();
}

void showCredits() {
 M5.Lcd.fillScreen(TFT_BLACK);  // Clear screen
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("Space Journey",160,20, GFXFF);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawString("Based on the solo game", 160,40, GFXFF);
  M5.Lcd.drawString("with the same name", 160, 60, GFXFF);
  M5.Lcd.drawString("by Tarcisio Lucas", 160, 80, GFXFF);
  M5.Lcd.drawString("& Inspired by", 160,120, GFXFF);
  M5.Lcd.drawString("\"Journey through", 160,140, GFXFF);
  M5.Lcd.drawString("Mirkwood\" by 1KM1KT", 160, 160, GFXFF);
  buttonContinue();
  titleScreen();
}

void buttonContinue() {
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.setFreeFont(FF1);
  M5.Lcd.drawString("Continue", 260, 220, GFXFF);
  M5.update();
  while(!M5.BtnC.wasReleased()) {
   M5.update();
  }
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}
// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enbabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
