/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCD UI demo>
*************************************************************/

//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include "Commanders.h"
#include "LcdUi.h"
#include "ScreenLiquid.hpp"
#include "UI.hpp"

const char str_titre[] PROGMEM = "Titre";
const char str_copyright[] PROGMEM = "Copyright";
const char str_choix[] PROGMEM = "Choix principal";
const char str_option1[] PROGMEM = "Option 1";
const char str_option2[] PROGMEM = "Option 2";
const char str_oui[] PROGMEM = "Oui";
const char str_non[] PROGMEM = "Non";
const char str_confirme[] PROGMEM = "Confirmer";
const char str_sur[] PROGMEM = "Sur ?";

// Array of declared strings
const char * const string_table[] PROGMEM =
{
	str_titre,
	str_copyright,
	str_choix,
	str_option1,
	str_option2,
	str_oui,
	str_non,
	str_confirme,
	str_sur
};

// Indices des chaines dans la liste 
#define STR_TITRE		0
#define STR_COPYRIGHT	1
#define STR_CHOIX		2
#define STR_OPTION1	3
#define STR_OPTION2	4
#define STR_OUI		5
#define STR_NON		6	
#define STR_CONFIRME	7
#define STR_SUR			8

// Main object and its screen
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LcdUi lcdui;
ScreenLiquid screen;

// Local values the UI must be able to update...
int option1;
bool option2;
Choice choix;

WindowSplash winSplash;
WindowChoice winChoix;
WindowInt winOption1;
WindowYesNo winOption2;
WindowConfirm winConfirme;

void setupUI()
{
	screen.begin(16, 2, string_table, &lcd);
	lcdui.begin(&screen);
	
	LcdScreen::YesMsg = STR_OUI;
	LcdScreen::NoMsg = STR_NON;

	winSplash.begin(STR_TITRE, STR_COPYRIGHT, 500);
	winChoix.begin(STR_CHOIX, &choix);
	winOption1.begin(STR_OPTION1, &option1);
	winOption2.begin(STR_OPTION2, &option2);
	winConfirme.begin(STR_CONFIRME, STR_SUR);

	lcdui.AddWindow(&winSplash);
	lcdui.AddWindow(&winChoix);
	lcdui.AddWindow(&winOption1);
	lcdui.AddWindow(&winOption2);
	lcdui.AddWindow(&winConfirme);

	winChoix.AddChoice(STR_OPTION1);
		winOption1.SetFather(&winChoix, STR_OPTION1);
	winChoix.AddChoice(STR_OPTION2);
		winOption2.SetFather(&winChoix, STR_OPTION2);
		winConfirme.SetFather(&winChoix, STR_OPTION2);

	// Initial values of local variables.
	option1 = 10;
	option2 = false;
}

void loopUI(unsigned long inEvent)
{
	// Use events to update the screen...
	if (lcdui.loop(inEvent))
	{
		Window *pCurrent = lcdui.GetGlobalCurrentWindow();

		// Do things when a window is confirmed:
		if (lcdui.GetState() == STATE_CONFIRMED)
		{
			switch (pCurrent->GetWindowId())
			{
			case STR_OPTION1:
				Serial.print("Option1: ");
				Serial.println(option1);
				break;
			case STR_OPTION2:
				Serial.print("Option2: ");
				Serial.println(option2);
				break;
			}
		}
	}
}
