/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCDUI DcDcc demo>
*************************************************************/

//#include <NewLiquidCrystal_I2C.h>
//#include <NewLiquidCrystal.h>
//#include <LiquidCrystal.h>
#include "LcdUi.h"
//#include "ScreenLiquidNew.hpp"
#include "ScreenLiquid.hpp"
#include "WindowLocoControl.hpp"
#include "UI.hpp"

// Strings declaration
const char  str_stop[] PROGMEM = "Arret Urgence";
const char  str_stop2[] PROGMEM = "Appuyer Annuler";
const char  str_modemodechoice[] PROGMEM = "Menu principal:";
const char  str_modelococtrl[] PROGMEM = "Controle loco";
const char  str_modeconfig[] PROGMEM = "Configuration";
const char  str_resetconfig[] PROGMEM = "Reset Config";
const char  str_yes[] PROGMEM = "oui";
const char  str_no[] PROGMEM = "non";
const char  str_confirm[] PROGMEM = "Sur ?";
const char  str_bkltconfig[] PROGMEM = "Retro eclairage";
const char  str_incconfig[] PROGMEM = "Vit Increment";
const char  str_nameconfig[] PROGMEM = "Nom";
const char  str_locoAddress[] PROGMEM = "Adresse Loco";
const char  str_splash1[] PROGMEM = "LcdUI Demo";
const char  str_splash2[] PROGMEM = "For ... you !";
const char  str_backspace[] PROGMEM = "Supprimer";

// Array of declared strings
const char * const string_table[] PROGMEM =
{
	str_stop,
	str_stop2,
	str_modemodechoice,
	str_modelococtrl,
	str_modeconfig,
	str_resetconfig,
	str_yes,
	str_no,
	str_confirm,
	str_bkltconfig,
	str_incconfig,
	str_nameconfig,
	str_locoAddress,
	str_splash1,
	str_splash2,
	str_backspace
};

// Indexes of strings in the array !
#define STR_STOP			0
#define STR_STOP2			1
#define STR_MODEMODECHOICE	2
#define STR_MODELOCOCTRL	3
#define STR_MODECONFIG		4
#define STR_RESETCONFIG		5
#define STR_YES				6
#define STR_NO				7
#define STR_CONFIRM			8
#define STR_BACKLIGHTCFG	9
#define STR_INCCFG			10
#define STR_NAMECFG			11
#define STR_LOCOADDRESS		12
#define STR_SPLASH1			13
#define STR_SPLASH2			14
#define STR_BACKSPACE		15

// Main object and its screen
//NewLiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//NewLiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
LcdUi lcdui;
//ScreenLiquidNew screen;
ScreenLiquid screen;

// Local values the UI must be able to update...
bool backlight;
Choice modeChoice;
Choice configChoice;
bool reset;

WindowSplash winSplash;
WindowChoice winMainChoice;
WindowChoice winSettingsChoice;
WindowInt winAddress;
WindowInt winIncrement;
WindowText winName;
WindowYesNo winBacklight;
WindowConfirm winReset;
WindowLocoControl winLoco;
WindowInterrupt winStop;

void setupUI()
{
	//screen.begin(20, 4, string_table, &lcd);
	screen.begin(16, 2, string_table, &lcd);
	lcdui.begin(&screen);
	
	LcdScreen::YesMsg = STR_YES;
	LcdScreen::NoMsg = STR_NO;
	LcdScreen::BackspaceMsg = STR_BACKSPACE;

	winSplash.begin(STR_SPLASH1, STR_SPLASH2, 2000);
	winMainChoice.begin(STR_MODEMODECHOICE, &modeChoice);
	winSettingsChoice.begin(STR_MODECONFIG, &configChoice);
	winAddress.begin(STR_LOCOADDRESS, &winLoco.Address);
	winIncrement.begin(STR_INCCFG, &winLoco.Speed128Inc);
	winName.begin(STR_NAMECFG, winLoco.Name, 14);
	winBacklight.begin(STR_BACKLIGHTCFG, &backlight);
	winReset.begin(STR_RESETCONFIG, STR_CONFIRM, &reset);
	winLoco.begin(STR_MODELOCOCTRL);
	winStop.begin(STR_STOP, STR_STOP2, EVENT_EMERGENCY);

	lcdui.AddWindow(&winSplash);
	lcdui.AddWindow(&winMainChoice);
		lcdui.AddWindow(&winSettingsChoice);
			lcdui.AddWindow(&winAddress);
			lcdui.AddWindow(&winIncrement);
			lcdui.AddWindow(&winName);
			lcdui.AddWindow(&winBacklight);
			lcdui.AddWindow(&winReset);
		lcdui.AddWindow(&winLoco);
	lcdui.AddWindow(&winStop);

	winMainChoice.AddChoice(STR_MODECONFIG, &winSettingsChoice);
		winSettingsChoice.AddChoice(STR_LOCOADDRESS, &winAddress);
		winSettingsChoice.AddChoice(STR_INCCFG, &winIncrement);
		winSettingsChoice.AddChoice(STR_NAMECFG, &winName);
		winSettingsChoice.AddChoice(STR_BACKLIGHTCFG, &winBacklight);
		winSettingsChoice.AddChoice(STR_RESETCONFIG, &winReset);
	winMainChoice.AddChoice(STR_MODELOCOCTRL, &winLoco);

	// Initial values of local variables.
	backlight = false;
	reset = false;
	winMainChoice.SetCurrentChoiceById(STR_MODELOCOCTRL);
}

void loopUI(byte inEvent)
{
	if (inEvent == EVENT_EMERGENCY)
	{
		Serial.println("Arret d'urgence !");
	}

	if (lcdui.loop(inEvent))
	{
		Window *pCurrent = lcdui.GetGlobalCurrentWindow();

		// Do things when a window is confirmed:
		if (lcdui.GetState() == STATE_CONFIRMED)
		{
			switch (pCurrent->GetWindowId())
			{
			case STR_LOCOADDRESS:
				Serial.print("New Dcc address: ");
				Serial.println(winLoco.Address);
				break;
			case STR_INCCFG:
				Serial.print("New incremental value: ");
				Serial.println(winLoco.Speed128Inc);
				break;
			case STR_NAMECFG:
				Serial.print("New name: ");
				Serial.println(winLoco.Name);
				break;
			case STR_BACKLIGHTCFG:
				Serial.print("New back light value: ");
				Serial.println(backlight);
				break;
			case STR_RESETCONFIG:
				if (reset == true)
				{
					Serial.println("Reset Config.");
					backlight = false;
					winLoco.Address = 3;
					winLoco.AddressSize = 3;
					winLoco.Speed = 0;
					winLoco.SpeedMax = 128;
					winLoco.Speed128Inc = 10;
					winLoco.Direction = true;
					strcpy(winLoco.Name, "Locoduino");
				}
				break;
			case STR_STOP:
				Serial.println("Fin de l'urgence");
				break;
			}
		}
	}
}
