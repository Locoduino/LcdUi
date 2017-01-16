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
#include "crtdbg.h"

// Strings declaration
const char  str_dc[] PROGMEM = "Dc";
const char  str_dcc[] PROGMEM = "Dcc";
const char  str_cv[] PROGMEM = "Cv";
const char  str_stop[] PROGMEM = "Arret Alim";
const char  str_stop2[] PROGMEM = "Appuyer Rouge";
const char  str_dcdcc[] PROGMEM = "Change DC/DCC";
const char  str_dcdcc2[] PROGMEM = "Redemarrer/annul";
const char  str_modemodechoice[] PROGMEM = "Choix du mode :";
const char  str_modelococtrl[] PROGMEM = "Controle loco";
const char  str_modelocoedit[] PROGMEM = "Edition loco";
const char  str_modelocoprogram[] PROGMEM = "Program loco";
const char  str_modeacccontrol[] PROGMEM = "Controle acces";
const char  str_modeaccedit[] PROGMEM = "Edition access";
const char  str_modeconfig[] PROGMEM = "Config DDC";
const char  str_locoselect[] PROGMEM = "Choix de loco";
const char  str_resetconfig[] PROGMEM = "Reset Config";
const char  str_yes[] PROGMEM = "oui";
const char  str_no[] PROGMEM = "non";
const char  str_confirm[] PROGMEM = "Sur ?";
const char  str_bkltconfig[] PROGMEM = "Retro eclairage";
const char  str_incconfig[] PROGMEM = "Vit Increment";
const char  str_nameconfig[] PROGMEM = "Nom";
const char  str_locoAddress[] PROGMEM = "Adresse Loco";
const char  str_locodel[] PROGMEM = "Supprimer loco";
const char  str_locoedit[] PROGMEM = "Editer loco";
const char  str_splash1[] PROGMEM = "LcdUI Demo 0.1";
const char  str_splash2[] PROGMEM = "By ... you !";
const char  str_freq[] PROGMEM = "DC Freq.";
const char  str_backspace[] PROGMEM = "Suppr";

// Array of declared strings
const char * const string_table[] PROGMEM =
{
	str_dc,
	str_dcc,
	str_cv,
	str_stop,
	str_stop2,
	str_dcdcc,
	str_dcdcc2,
	str_modemodechoice,
	str_modelococtrl,
	str_modelocoedit,
	str_modelocoprogram,
	str_modeacccontrol,
	str_modeaccedit,
	str_modeconfig,
	str_locoselect,
	str_resetconfig,
	str_yes,
	str_no,
	str_confirm,
	str_bkltconfig,
	str_incconfig,
	str_nameconfig,
	str_locoAddress,
	str_locodel,
	str_locoedit,
	str_splash1,
	str_splash2,
	str_freq,
	str_backspace
};

// Indexes of strings in the array !
#define STR_DC				0
#define STR_DCC				1
#define STR_CV				2
#define STR_STOP			3
#define STR_STOP2			4
#define STR_DCDCC			5
#define STR_DCDCC2			6
#define STR_MODEMODECHOICE	7
#define STR_MODELOCOCTRL	8
#define STR_MODELOCOEDIT	9
#define STR_MODELOCOPROGRAM	10
#define STR_MODEACCCONTROL	11
#define STR_MODEACCEDIT		12
#define STR_MODECONFIG		13
#define STR_LOCOSELECT		14
#define STR_RESETCONFIG		15
#define STR_YES				16
#define STR_NO				17
#define STR_CONFIRM			18
#define STR_BACKLIGHTCFG	19
#define STR_INCCFG			20
#define STR_NAMECFG			21
#define STR_LOCOADDRESS		22
#define STR_LOCOREMOVE		23
#define STR_LOCOEDIT		24
#define STR_SPLASH1			25
#define STR_SPLASH2			26
#define STR_FREQ			27
#define STR_BACKSPACE		28

//////////////// Local specific window to choose default Dc frequency !
#define WINDOWTYPE_FREQCHOICE	12

// List of available frequencies
const char* options[] = { "8k", "16k", "32k", "60k" };

// the class is inherited from a basic choice between texts
class WindowChooseDcFreq : public WindowChoiceText
{
public:
	WindowChooseDcFreq(int inFirstLine, byte *inpFreq, int inTag = 0) : WindowChoiceText(inFirstLine, inpFreq, inTag) {}

	inline byte GetType() const { return WINDOWTYPE_FREQCHOICE; }
	inline byte GetChoiceTextNumber() const { return 4; }
	inline char *GetChoiceTextValue(unsigned int indexValue) const { return (char *) (options[indexValue]); }
};
/////////////////

// Main object and its screen
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal lcd(0, 4, 5, 6, 7, 3);
LcdUi lcdui;
ScreenLiquid screen;

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard push0;
ButtonsCommanderKeyboard push1;
ButtonsCommanderKeyboard push2;
ButtonsCommanderKeyboard push3;
ButtonsCommanderKeyboard pushDcDcc;
ButtonsCommanderKeyboard pushEmergency;
#else
ButtonsCommanderAnalogPushes pushes;
#endif

#define EVENT_DCDCC		10
#define EVENT_EMERGENCY	11

// Local values the UI must be able to update...
bool backlight;
byte freq = 0;
Choice modeChoice;
Choice configChoice;

/*WindowSplash winSplash;
WindowChoice winMainChoice;
WindowChoice winSettingsChoice;
WindowInt winAddress;
WindowInt winIncrement;
WindowText winName;
WindowYesNo winBacklight;
WindowConfirm winReset;
WindowLocoControl winLoco;
WindowChooseDcFreq winFreq;
WindowInterrupt winStop;
WindowInterrupt winDcDcc;*/

WindowLocoControl WinLoco;

void setup()
{
	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);

	screen.begin(20, 4, string_table, &lcd);
	LcdScreen::YesMsg = STR_YES;
	LcdScreen::NoMsg = STR_NO;
	LcdScreen::BackspaceMsg = STR_BACKSPACE;
	lcdui.begin(&screen);
	/*
	winSplash.begin(&lcdui, STR_SPLASH1, STR_SPLASH2, 500);
	winMainChoice.begin(&lcdui, STR_MODEMODECHOICE, &modeChoice, false);
		winMainChoice.AddChoice(STR_MODECONFIG);
		winSettingsChoice.begin(&lcdui, STR_MODECONFIG, &configChoice, false, &winMainChoice, STR_MODECONFIG);
			winSettingsChoice.AddChoice(STR_LOCOADDRESS);
				winAddress.begin(&lcdui, STR_LOCOADDRESS, &winLoco.Address, &winSettingsChoice, STR_LOCOADDRESS);
			winSettingsChoice.AddChoice(STR_INCCFG);
				winIncrement.begin(&lcdui, STR_INCCFG, &winLoco.Speed128Inc, &winSettingsChoice, STR_INCCFG);
			winSettingsChoice.AddChoice(STR_NAMECFG);
				winName.begin(&lcdui, STR_NAMECFG, winLoco.Name, &winSettingsChoice, STR_NAMECFG);
			winSettingsChoice.AddChoice(STR_BACKLIGHTCFG);
				winBacklight.begin(&lcdui, STR_BACKLIGHTCFG, &backlight, &winSettingsChoice, STR_BACKLIGHTCFG);
			winSettingsChoice.AddChoice(STR_RESETCONFIG);
				winReset.begin(&lcdui, STR_RESETCONFIG, STR_CONFIRM, winLoco.Name, &winSettingsChoice, STR_RESETCONFIG);
		winMainChoice.AddChoice(STR_MODELOCOCTRL);
			winLoco.begin(&lcdui, &winMainChoice, STR_MODELOCOCTRL);
		winMainChoice.AddChoice(STR_FREQ);
			winfreq.begin(&lcdui, STR_FREQ, &freq, &winMainChoice, STR_FREQ);

	winStop.begin(&lcdui, STR_STOP, STR_STOP2, EVENT_EMERGENCY);
	winDcDcc.begin(&lcdui, STR_DCDCC, STR_DCDCC2, EVENT_DCDCC);
	*/
	
	lcdui.AddWindow(new WindowSplash(STR_SPLASH1, STR_SPLASH2, 500));	// Splash screen
	WindowChoice *pChoiceMain = (WindowChoice *)lcdui.AddWindow(new WindowChoice(STR_MODEMODECHOICE, &modeChoice, false));	// menu
	pChoiceMain->AddChoice(STR_MODECONFIG);
		WindowChoice *pChoiceConfig = (WindowChoice *)lcdui.AddWindow(new WindowChoice(STR_MODECONFIG, &configChoice, false), pChoiceMain, 0);	// config
		pChoiceConfig->AddChoice(STR_LOCOADDRESS);
			lcdui.AddWindow(new WindowInt(STR_LOCOADDRESS, &WinLoco.Address), pChoiceConfig, 0);// config loco adresse
		pChoiceConfig->AddChoice(STR_INCCFG);
			lcdui.AddWindow(new WindowInt(STR_INCCFG, &WinLoco.Speed128Inc), pChoiceConfig, 1);	// config inc value
		pChoiceConfig->AddChoice(STR_NAMECFG);
			lcdui.AddWindow(new WindowText(STR_NAMECFG, WinLoco.Name, 10), pChoiceConfig, 2);	// config name
		pChoiceConfig->AddChoice(STR_BACKLIGHTCFG);
			lcdui.AddWindow(new WindowYesNo(STR_BACKLIGHTCFG, &backlight), pChoiceConfig, 3);	// config backlight
		pChoiceConfig->AddChoice(STR_RESETCONFIG);
			lcdui.AddWindow(new WindowConfirm(STR_RESETCONFIG, STR_CONFIRM), pChoiceConfig, 4);	// reset config
	pChoiceMain->AddChoice(STR_MODELOCOCTRL);
		lcdui.AddWindow(&WinLoco, pChoiceMain, 1); // run
	pChoiceMain->AddChoice(STR_FREQ);
		lcdui.AddWindow(new WindowChooseDcFreq(STR_FREQ, &freq), pChoiceMain, 2); // change freq

	lcdui.AddWindowInterrupt(new WindowInterrupt(STR_STOP, STR_STOP2, EVENT_EMERGENCY)); // Emergency stop
	lcdui.AddWindowInterrupt(new WindowInterrupt(STR_DCDCC, STR_DCDCC2, EVENT_DCDCC)); // Mode Dc/DCC change

	// Initial values of local variables.
	backlight = false;

#ifdef VISUALSTUDIO
	push0.begin(EVENT_SELECT, '*');
	push1.begin(EVENT_MORE, '+');
	push2.begin(EVENT_LESS, '-');
	push3.begin(EVENT_CANCEL, '/');
	pushDcDcc.begin(EVENT_DCDCC, '.');
	pushEmergency.begin(EVENT_EMERGENCY, '0');
#else
	// Buttons on a 'LCD Keypad shield', handled in analog way.
	int values[] = { 0, 145, 329, 505, 741 };
	// Each button as an EVENT number as ID ! Easier for the loop !
	// One button has no meaning with 1004 (why not ?) as ID.
	unsigned long ids[] = { EVENT_SELECT, EVENT_MORE, EVENT_LESS, 1004, EVENT_CANCEL };

	pushes.begin(A0, 5, ids, values, 50);
#endif
	//Commanders::printCommanders();
	lcdui.printWindows();
}

void loop()
{
	unsigned long event = Commanders::loop();

	// For LcdUi, UNDEFINED_ID has no meaning. And because it is necessary 
	// to execute lcdui.event() at each call of the main loop, 
	// do it with an empty event : EVENT_NONE.

	if (event == UNDEFINED_ID)
		event = EVENT_NONE;

	// Use events to update the screen...
	if (lcdui.loop(event))
	{
		Window *pCurrent = lcdui.GetGlobalCurrentWindow();

		int state = lcdui.GetState();
		switch (lcdui.GetState())
		{
		// If the current window is almost closed...
		// The associated edited value should have changed...
		case STATE_CONFIRMED:
			switch (pCurrent->GetWindowId())
			{
			case STR_LOCOADDRESS:
				Serial.print("New Dcc address: ");
				Serial.println(WinLoco.Address);
				break;
			case STR_INCCFG:
				Serial.print("New incremental value: ");
				Serial.println(WinLoco.Speed128Inc);
				break;
			case STR_NAMECFG:
				Serial.print("New name: ");
				Serial.println(WinLoco.Name);
				break;
			case STR_BACKLIGHTCFG:
				lcdui.SetChoiceValue(backlight ? LcdScreen::YesMsg : LcdScreen::NoMsg);
				break;
			case STR_FREQ:
				Serial.print("New frequency: ");
				Serial.println(freq);
				break;
			case STR_RESETCONFIG:
				Serial.println("Reset Config.");
				backlight = false;;
				freq = 0;
				WinLoco.Address = 3;
				WinLoco.AddressSize = 3;
				WinLoco.Speed = 0;
				WinLoco.SpeedMax = 128;
				WinLoco.Speed128Inc = 10;
				WinLoco.Direction = true;
				strcpy(WinLoco.Name, "Locoduino");
				break;
			}
			break;
		}
	}
}
