/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCD UI demo in macro programmation mode>
*************************************************************/

//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include "Commanders.h"
#include "LcdUi.h"

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
const char  str_loconew[] PROGMEM = "Nouvelle Loco";
const char  str_locodel[] PROGMEM = "Supprimer loco";
const char  str_locoedit[] PROGMEM = "Editer loco";
const char  str_splash1[] PROGMEM = "LcdUI Demo 0.1";
const char  str_splash2[] PROGMEM = "By ... you !";
const char  str_freq[] PROGMEM = "DC Freq.";

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
	str_loconew,
	str_locodel,
	str_locoedit,
	str_splash1,
	str_splash2,
	str_freq
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
#define STR_LOCONEW			22
#define STR_LOCOREMOVE		23
#define STR_LOCOEDIT		24
#define STR_SPLASH1			25
#define STR_SPLASH2			26
#define STR_FREQ			27

//////////////// Local specific window to choose default Dc frequency !	   
#define WINDOWTYPE_FREQCHOICE	12

// List of available frequencies
const char* options[] = { "8k", "16k", "32k", "60k" };

// the class is inherited from a basic choice between texts
class WindowChooseDcFreq : public WindowChoiceText
{
public:
	WindowChooseDcFreq(int inFirstLine, int inTag = 0) : WindowChoiceText(inFirstLine, inTag) {}

	inline byte GetType() const { return WINDOWTYPE_FREQCHOICE; }
	inline byte GetChoiceTextNumber() const { return 4; }
	inline char *GetChoiceTextValue(unsigned int indexValue) const { return (char *)(options[indexValue]); }
};
/////////////////

// Main object and its screen
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal lcd(0, 4, 5, 6, 7, 3);
ScreenLiquid screen;

DECLARE_LCDUI

DECLARE_BUTTONS_COMMANDER

// Local values the UI must be able to update...
int incValue;
bool backlight;
char name[80];
int freq = 0;

void setup()
{
	START_LCDUI_SETUP;

	screen.Setup(20, 4, string_table, &lcd);
	Screen::YesMsg = STR_YES;
	Screen::NoMsg = STR_NO;

	BEGIN_UI(screen, 11);

	ADDWINDOWSPLASH(STR_SPLASH1, STR_SPLASH2, 500);	// Splash screen
	WindowChoice *pChoiceMain = ADDWINDOWCHOICE(STR_MODEMODECHOICE, 3, false);	// menu
	ADDCHOICE(pChoiceMain, STR_MODECONFIG);
		WindowChoice *pChoiceConfig = ADDWINDOWCHOICE(STR_MODECONFIG, 4, false, pChoiceMain, 0);	// config
		ADDCHOICE(pChoiceConfig, STR_INCCFG);
			ADDWINDOWINT(STR_INCCFG, 0, 128, pChoiceConfig, 0);	// config incValue
		ADDCHOICE(pChoiceConfig, STR_NAMECFG);
			ADDWINDOWTEXT(STR_NAMECFG, 10, pChoiceConfig, 1);	// config name
		ADDCHOICE(pChoiceConfig, STR_BACKLIGHTCFG);
			ADDWINDOWYESNO(STR_BACKLIGHTCFG, pChoiceConfig, 2);	// config backlight
		ADDCHOICE(pChoiceConfig, STR_RESETCONFIG);
			ADDWINDOWCONFIRM(STR_RESETCONFIG, STR_CONFIRM, pChoiceConfig, 3);	// reset config
	ADDCHOICE(pChoiceMain, STR_MODELOCOCTRL);
		ADDWINDOWCUSTOM(new Window(STR_MODELOCOCTRL), pChoiceMain, 1); // run
	ADDCHOICE(pChoiceMain, STR_FREQ);
		ADDWINDOWCUSTOM(new WindowChooseDcFreq(STR_FREQ), pChoiceMain, 2); // vhange freq

	ADDWINDOWINTERRUPT(STR_STOP, STR_STOP2); // Emergency stop
	ADDWINDOWINTERRUPT(STR_DCDCC, STR_DCDCC2); // Mode Dc/DCC change

	// Initial values of local variables.
	incValue = 10;
	backlight = false;
	strcpy(name, "Locoduino");

	END_LCDUI_SETUP;

	START_BUTTONS_COMMANDER_SETUP;

	// Buttons on a 'LCD Keypad shield', handled in analog way.
	int values[] = { 0, 145, 329, 505, 741 };
	// Each button as an EVENT number as ID ! Easier for the loop !
	// One button has no meaning with 1004 (why not ?) as ID.
	unsigned long ids[] = { EVENT_SELECT, EVENT_MORE, EVENT_LESS, 1004, EVENT_CANCEL };

	ANALOGPUSH_BUTTON(pPushes, A0, 5, ids, values);

	END_COMMANDER_SETUP;
}

void loop()
{
	unsigned long event = COMMANDER_LOOP;

	// For LcdUi, UNDEFINED_ID has no meaning. And because it is necessary 
	// to execute lcdui.event() at each call of the main loop, 
	// do it with an empty event : EVENT_NONE.

	if (event == UNDEFINED_ID)
		event = EVENT_NONE;

	// Use events to update the screen...
	if (LCDUI_LOOP((int) event))
	{
		WINDOWINTEVENT(STR_INCCFG, incValue);
		WINDOWTEXTEVENT(STR_NAMECFG, name);
		WINDOWYESNOEVENT(STR_BACKLIGHTCFG, backlight);
		WINDOWCHOICETEXTEVENT(STR_FREQ, freq);
	}
}
