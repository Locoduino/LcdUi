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
const char str_option3[] PROGMEM = "Option 3";
const char str_oui[] PROGMEM = "Oui";
const char str_non[] PROGMEM = "Non";
const char str_confirme[] PROGMEM = "Confirmer";
const char str_recule[] PROGMEM = "Reculer";
const char str_sur[] PROGMEM = "Sur ?";
const char str_int1[] PROGMEM = "Interruption";
const char str_int2[] PROGMEM = "Sortir: Annuler";

// Array of declared strings
const char * const string_table[] PROGMEM =
{
	str_titre,
	str_copyright,
	str_choix,
	str_option1,
	str_option2,
	str_option3,
	str_oui,
	str_non,
	str_confirme,
	str_recule,
	str_sur, 
	str_int1,
	str_int2
};

// Indices des chaines dans la liste 
#define STR_TITRE		0
#define STR_COPYRIGHT	1
#define STR_CHOIX		2
#define STR_OPTION1		3
#define STR_OPTION2		4
#define STR_OPTION3		5
#define STR_OUI			6
#define STR_NON			7
#define STR_CONFIRME	8
#define STR_RECULE		9
#define STR_SUR			10
#define STR_INT1		11
#define STR_INT2		12

// Main object and its screen
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LcdUi lcdui;
ScreenLiquid screen;

// Local values the UI must be able to update...
int option1;
bool option2;
char buffer[20];
Choice choix;

WindowSplash winSplash;
WindowChoice winChoix;
WindowInt winOption1;
WindowYesNo winOption2;
WindowText winOption3;
WindowConfirm winConfirme;
WindowInterrupt winInterrupt;
WindowInterruptConfirm winInterruptConfirm;

void setupUI()
{
	screen.begin(16, 2, string_table, &lcd);
	lcdui.begin(&screen);
	
	LcdScreen::YesMsg = STR_OUI;
	LcdScreen::NoMsg = STR_NON;
	LcdScreen::BackspaceMsg = STR_RECULE;

	winSplash.begin(STR_TITRE, STR_COPYRIGHT, 500);
	winChoix.begin(STR_CHOIX, &choix);
	winOption1.begin(STR_OPTION1, &option1);
	winOption2.begin(STR_OPTION2, &option2);
	winOption3.begin(STR_OPTION3, buffer, 20);
	winConfirme.begin(STR_CONFIRME, STR_SUR);
	winInterrupt.begin(STR_INT1, STR_INT2, EVENT_EMERGENCY);
	winInterruptConfirm.begin(STR_INT1, STR_SUR, EVENT_EMERGENCY);

	lcdui.AddWindow(&winSplash);
	lcdui.AddWindow(&winChoix);
	lcdui.AddWindow(&winOption1);
	lcdui.AddWindow(&winOption2);
	lcdui.AddWindow(&winOption3);
	lcdui.AddWindow(&winConfirme);
	//lcdui.AddWindow(&winInterrupt);
	lcdui.AddWindow(&winInterruptConfirm);

	winChoix.AddChoice(STR_OPTION1);
		winOption1.SetFather(&winChoix, STR_OPTION1);
	winChoix.AddChoice(STR_OPTION2);
		winOption2.SetFather(&winChoix, STR_OPTION2);
		winConfirme.SetFather(&winChoix, STR_OPTION2);
		winOption3.SetFather(&winChoix, STR_OPTION2);

	// Initial values of local variables.
	option1 = 10;
	option2 = false;
	strcpy(buffer, "LOCODUINO");

	//lcdui.printWindows();
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
			case STR_INT1:
				Serial.println("Interrupt finished: ");
				break;
/*			case STR_INT1:
				Serial.print("Interrupt finished: ");
				Serial.println(((WindowInterruptConfirm*)pCurrent)->GetValue() == STR_OUI ? "Yes":"No");
				break;*/
			}
		}

		if (lcdui.GetState() == STATE_INITIALIZE)
		{
			switch (pCurrent->GetWindowId())
			{
			case STR_INT1:
				Serial.println("Interrupt started: ");
				break;
			}
		}
	}
}
