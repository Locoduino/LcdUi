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
#include "WindowLocoControl.hpp"
#include "UI.hpp"

// Strings declaration
const char  str_dc[] PROGMEM = "Dc";
const char  str_dcc[] PROGMEM = "Dcc";
const char  str_stop[] PROGMEM = "Arret Alim";
const char  str_stop2[] PROGMEM = "Appuyer Rouge";
const char  str_dcdcc[] PROGMEM = "Change DC/DCC";
const char  str_dcdcc2[] PROGMEM = "Redemarrer/annul";
const char  str_modemodechoice[] PROGMEM = "Choix du mode :";
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
const char  str_splash1[] PROGMEM = "LcdUI Demo 0.1";
const char  str_splash2[] PROGMEM = "By ... you !";
const char  str_freq[] PROGMEM = "Frequence PWM";
const char  str_backspace[] PROGMEM = "Supprimer";

// Array of declared strings
const char * const string_table[] PROGMEM =
{
	str_dc,
	str_dcc,
	str_stop,
	str_stop2,
	str_dcdcc,
	str_dcdcc2,
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
	str_freq,
	str_backspace
};

// Indexes of strings in the array !
#define STR_DC				0
#define STR_DCC				1
#define STR_STOP			2
#define STR_STOP2			3
#define STR_DCDCC			4
#define STR_DCDCC2			5
#define STR_MODEMODECHOICE	6
#define STR_MODELOCOCTRL	7
#define STR_MODECONFIG		8
#define STR_RESETCONFIG		9
#define STR_YES				10
#define STR_NO				11
#define STR_CONFIRM			12
#define STR_BACKLIGHTCFG	13
#define STR_INCCFG			14
#define STR_NAMECFG			15
#define STR_LOCOADDRESS		16
#define STR_SPLASH1			17
#define STR_SPLASH2			18
#define STR_FREQ			19
#define STR_BACKSPACE		20

//////////////// Local specific window to choose default Dc frequency !
#define WINDOWTYPE_FREQCHOICE	12

// List of available frequencies
const char* options[] = { "8k", "16k", "32k", "60k" };

// the class is inherited from a basic choice between texts
class WindowChooseDcFreq : public WindowChoiceText
{
public:
	inline WindowChooseDcFreq() {}
	void begin(int inFirstLine, byte *inpFreq) { WindowChoiceText::begin(inFirstLine, inpFreq); }

	inline byte GetType() const { return WINDOWTYPE_FREQCHOICE; }
	inline byte GetChoiceTextNumber() const { return 4; }
	inline char *GetChoiceTextValue(unsigned int indexValue) const { return (char *) (options[indexValue]); }
};
/////////////////

// Main object and its screen
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//LiquidCrystal lcd(0, 4, 5, 6, 7, 3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LcdUi lcdui;
ScreenLiquid screen;

#define EVENT_DCDCC		10
#define EVENT_EMERGENCY	11

// Local values the UI must be able to update...
bool backlight;
Choice modeChoice;
Choice configChoiceDc;
Choice configChoiceDcc;

/*WindowSplash winSplash;
WindowChoice winMainChoice;
WindowChoice winDcSettingsChoice;
WindowChoice winDccSettingsChoice;
WindowInt winAddress;
WindowInt winIncrement;
WindowText winName;
WindowYesNo winBacklight, winBacklightDc;
WindowConfirm winReset, winResetDc;
WindowLocoControl winLoco;
WindowChooseDcFreq winFreq;
WindowInterrupt winStop;
WindowInterrupt winDcDcc;*/

void setupUI()
{
	screen.begin(16, 2, string_table, &lcd);
	lcdui.begin(&screen);
	
	LcdScreen::YesMsg = STR_YES;
	LcdScreen::NoMsg = STR_NO;
	LcdScreen::BackspaceMsg = STR_BACKSPACE;

/*	winSplash.begin(STR_SPLASH1, STR_SPLASH2, 500);
	winMainChoice.begin(STR_MODEMODECHOICE, &modeChoice);
	winDcSettingsChoice.begin(STR_MODECONFIG, &configChoiceDc);
	winDccSettingsChoice.begin(STR_MODECONFIG, &configChoiceDcc);
	winAddress.begin(STR_LOCOADDRESS, &winLoco.Address);
	winIncrement.begin(STR_INCCFG, &winLoco.Speed128Inc);
	winName.begin(STR_NAMECFG, winLoco.Name, 12);
	winBacklight.begin(STR_BACKLIGHTCFG, &backlight);
	winBacklightDc.begin(STR_BACKLIGHTCFG, &backlight);
	winReset.begin(STR_RESETCONFIG, STR_CONFIRM);
	winResetDc.begin(STR_RESETCONFIG, STR_CONFIRM);
	winLoco.begin(STR_MODELOCOCTRL);
	winFreq.begin(STR_FREQ, &winLoco.dcFrequency);
	winStop.begin(STR_STOP, STR_STOP2, EVENT_EMERGENCY);
	winDcDcc.begin(STR_DCDCC, STR_DCDCC2, EVENT_DCDCC);

	lcdui.AddWindow(&winSplash);
	lcdui.AddWindow(&winMainChoice);
		lcdui.AddWindow(&winDccSettingsChoice);
			lcdui.AddWindow(&winAddress);
			lcdui.AddWindow(&winIncrement);
			lcdui.AddWindow(&winName);
			lcdui.AddWindow(&winBacklight);
			lcdui.AddWindow(&winReset);
		lcdui.AddWindow(&winDcSettingsChoice);
			lcdui.AddWindow(&winFreq);
			lcdui.AddWindow(&winBacklightDc);
			lcdui.AddWindow(&winResetDc);
		lcdui.AddWindow(&winLoco);
	lcdui.AddWindow(&winStop);
	lcdui.AddWindow(&winDcDcc);

	winMainChoice.AddChoice(STR_MODECONFIG);
		winDccSettingsChoice.SetFather(&winMainChoice, STR_MODECONFIG);
			winDccSettingsChoice.AddChoice(STR_LOCOADDRESS);
			winAddress.SetFather(&winDccSettingsChoice, STR_LOCOADDRESS);
			winDccSettingsChoice.AddChoice(STR_INCCFG);
			winIncrement.SetFather(&winDccSettingsChoice, STR_INCCFG);
			winDccSettingsChoice.AddChoice(STR_NAMECFG);
			winName.SetFather(&winDccSettingsChoice, STR_NAMECFG);
			winDccSettingsChoice.AddChoice(STR_BACKLIGHTCFG);
			winBacklight.SetFather(&winDccSettingsChoice, STR_BACKLIGHTCFG);
			winDccSettingsChoice.AddChoice(STR_RESETCONFIG);
			winReset.SetFather(&winDccSettingsChoice, STR_RESETCONFIG);
		winDcSettingsChoice.SetFather(&winMainChoice, STR_MODECONFIG);
			winDcSettingsChoice.AddChoice(STR_FREQ);
			winFreq.SetFather(&winDcSettingsChoice, STR_FREQ);
			winDcSettingsChoice.AddChoice(STR_BACKLIGHTCFG);
			winBacklightDc.SetFather(&winDcSettingsChoice, STR_BACKLIGHTCFG);
			winDcSettingsChoice.AddChoice(STR_RESETCONFIG);
			winResetDc.SetFather(&winDcSettingsChoice, STR_RESETCONFIG);
	winMainChoice.AddChoice(STR_MODELOCOCTRL);
		winLoco.SetFather(&winMainChoice, STR_MODELOCOCTRL);

	// Initial values of local variables.
	backlight = false;
	winLoco.dcFrequencyText = options[winLoco.dcFrequency];
	winLoco.isDcc = true;
	winDccSettingsChoice.SetActive(winLoco.isDcc);
	winDcSettingsChoice.SetActive(!winLoco.isDcc);

	//lcdui.printWindows();
	*/
}

void loopUI(unsigned long inEvent)
{
	// Use events to update the screen...
	if (lcdui.loop(inEvent))
	{
/*		Window *pCurrent = lcdui.GetGlobalCurrentWindow();

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
			case STR_FREQ:
				Serial.print("New frequency: ");
				Serial.println(options[winLoco.dcFrequency]);
				winLoco.dcFrequencyText = options[winLoco.dcFrequency];
				break;
			case STR_RESETCONFIG:
				Serial.println("Reset Config.");
				backlight = false;
				winLoco.dcFrequency = 0;
				winLoco.Address = 3;
				winLoco.AddressSize = 3;
				winLoco.Speed = 0;
				winLoco.SpeedMax = 128;
				winLoco.Speed128Inc = 10;
				winLoco.Direction = true;
				strcpy(winLoco.Name, "Locoduino");
				break;
			}
		}*/
	}
}
