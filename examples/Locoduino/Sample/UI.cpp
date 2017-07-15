/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCD UI demo>
*************************************************************/

#include <NewLiquidCrystal_I2C.h>
#include <NewLiquidCrystal.h>
#include "Commanders.h"
#include "LcdUi.h"
#include "ScreenLiquid.hpp"
#include "UI.hpp"

const char str_titre[] PROGMEM = "LcdUi Test";
const char str_copyright[] PROGMEM = "Locoduino 2017";
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
const char str_int3[] PROGMEM = "Interrupt";

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
	str_int2,
	str_int3
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
#define STR_INT3		13

// Ecran 16x2
NewLiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LcdUi lcdui;
ScreenLiquidNew screen;

// Local values the UI must be able to update...
int option1;
bool option2;
char nom[20];
Choice choix;
bool confirme;

WindowSplash ecranSplash;
WindowChoice ecranChoix;
WindowInt ecranOption1;
WindowYesNo ecranOption2;
WindowText ecranOption3;
WindowConfirm ecranConfirme;
WindowInterrupt ecranInterrupt;
WindowInterruptConfirm ecranInterruptConfirm;

// Ecran 20x4
NewLiquidCrystal_I2C lcd_4(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LcdUi lcdui_4;
ScreenLiquidNew screen_4;

// Local values the UI must be able to update...
int option1_4;
bool option2_4;
char nom_4[20];
Choice choix_4;
bool confirme_4;

WindowSplash ecranSplash_4;
WindowChoice ecranChoix_4;
WindowInt ecranOption1_4;
WindowYesNo ecranOption2_4;
WindowText ecranOption3_4;
WindowConfirm ecranConfirme_4;
WindowInterrupt ecranInterrupt_4;
WindowInterruptConfirm ecranInterruptConfirm_4;

void setupUI()
{
	LcdScreen::YesMsg = STR_OUI;
	LcdScreen::NoMsg = STR_NON;
	LcdScreen::BackspaceMsg = STR_RECULE;

	// 16x2

	screen.begin(16, 2, string_table, &lcd);
	lcdui.begin(&screen);

	ecranSplash.begin(STR_TITRE, STR_COPYRIGHT, 5000);
	ecranChoix.begin(STR_CHOIX, &choix);
	ecranOption1.begin(STR_OPTION1, &option1);
	ecranOption2.begin(STR_OPTION2, &option2);
	ecranOption3.begin(STR_OPTION3, nom, 16);
	ecranConfirme.begin(STR_CONFIRME, STR_SUR, &confirme);
	ecranInterrupt.begin(STR_INT1, STR_INT2, EVENT_EMERGENCY);
	ecranInterruptConfirm.begin(STR_INT3, STR_SUR, EVENT_EMERGENCY_CONFIRM);

	lcdui.AddWindow(&ecranSplash);
	lcdui.AddWindow(&ecranChoix);
	lcdui.AddWindow(&ecranOption1);
	lcdui.AddWindow(&ecranOption2);
	lcdui.AddWindow(&ecranOption3);
	lcdui.AddWindow(&ecranConfirme);
	lcdui.AddWindow(&ecranInterrupt);
	lcdui.AddWindow(&ecranInterruptConfirm);

	ecranChoix.AddChoice(STR_OPTION1, &ecranOption1);
	ecranChoix.AddChoice(STR_OPTION2);
	ecranOption2.SetFather(STR_CHOIX, STR_OPTION2);
	ecranConfirme.SetFather(STR_CHOIX, STR_OPTION2);
	ecranOption3.SetFather(STR_CHOIX, STR_OPTION2);

	// Initial values of local variables.
	option1 = 10;
	option2 = false;
	confirme = false;
	strcpy(nom, "LOCODUINO");

	// 20x4

	screen_4.begin(20, 4, string_table, &lcd_4);
	lcdui_4.begin(&screen_4);

	ecranSplash_4.begin(STR_TITRE, STR_COPYRIGHT, 5000);
	ecranChoix_4.begin(STR_CHOIX, &choix_4);
	ecranOption1_4.begin(STR_OPTION1, &option1_4);
	ecranOption2_4.begin(STR_OPTION2, &option2_4);
	ecranOption3_4.begin(STR_OPTION3, nom_4, 16);
	ecranConfirme_4.begin(STR_CONFIRME, STR_SUR, &confirme_4);
	ecranInterrupt_4.begin(STR_INT1, STR_INT2, EVENT_EMERGENCY);
	ecranInterruptConfirm_4.begin(STR_INT3, STR_SUR, EVENT_EMERGENCY_CONFIRM);

	lcdui_4.AddWindow(&ecranSplash_4);
	lcdui_4.AddWindow(&ecranChoix_4);
	lcdui_4.AddWindow(&ecranOption1_4);
	lcdui_4.AddWindow(&ecranOption2_4);
	lcdui_4.AddWindow(&ecranOption3_4);
	lcdui_4.AddWindow(&ecranConfirme_4);
	lcdui_4.AddWindow(&ecranInterrupt_4);
	lcdui_4.AddWindow(&ecranInterruptConfirm_4);

	ecranChoix_4.AddChoice(STR_OPTION1, &ecranChoix_4);
	ecranChoix_4.AddChoice(STR_OPTION2);
	ecranOption2_4.SetFather(STR_CHOIX, STR_OPTION2);
	ecranConfirme_4.SetFather(STR_CHOIX, STR_OPTION2);
	ecranOption3_4.SetFather(STR_CHOIX, STR_OPTION2);

	// Initial values of local variables.
	option1_4 = 10;
	option2_4 = false;
	confirme_4 = false;
	strcpy(nom_4, "LOCODUINO");

	PRINT_WINDOWS();
}

void loopUI(unsigned long inEvent)
{
	lcdui_4.loop(inEvent);

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
			case STR_OPTION3:
				Serial.print("Option3: ");
				Serial.println(nom);
				break;
			case STR_INT1:
				Serial.println("Interruption terminee");
				break;
			case STR_INT3:
				Serial.print("Interrupt terminee");
				Serial.println(((WindowInterruptConfirm*)pCurrent)->GetValue() == STR_OUI ? "Yes":"No");
				break;
			}
		}

		if (lcdui.GetState() == STATE_INITIALIZE)
		{
			switch (pCurrent->GetWindowId())
			{
			case STR_INT1:
				Serial.println("Interruption commenc�e");
				break;
			case STR_INT3:
				Serial.println("Interruption/confirmation commence");
				break;
			}
		}
	}
}
