/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCD UI demo>
*************************************************************/

#include "Commanders.h"
#include "LcdUi.h"
#include "UI.hpp"

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard pushSelect;
ButtonsCommanderKeyboard pushMore;
ButtonsCommanderKeyboard pushLess;
ButtonsCommanderKeyboard pushCancel;
ButtonsCommanderKeyboard pushEmergency;
#else
ButtonsCommanderAnalogPushes pushes;
#endif

void setup()
{
	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);

#ifdef VISUALSTUDIO
	pushSelect.begin(EVENT_SELECT, '*');
	pushMore.begin(EVENT_MORE, '+');
	pushLess.begin(EVENT_LESS, '-');
	pushCancel.begin(EVENT_CANCEL, '/');
	pushEmergency.begin(EVENT_EMERGENCY, '0');
#else
	// Buttons on a 'LCD Keypad shield', handled in analog way.
	int values[] = { 0, 145, 329, 505, 741 };
	// Each button as an EVENT number as ID ! Easier for the loop !
	// One button has no meaning with 1004 (why not ?) as ID.
	unsigned long ids[] = { EVENT_EMERGENCY, EVENT_MORE, EVENT_LESS, EVENT_CANCEL, EVENT_SELECT };

	pushes.begin(A0, 5, ids, values, 50);
#endif

	setupUI();
}

void loop()
{
	unsigned long event = Commanders::loop();

	// For LcdUi, UNDEFINED_ID of Commanders has no meaning. And because it is necessary 
	// to execute lcdui.event() at each call of the main loop, 
	// do it with an empty event : EVENT_NONE.

	if (event == UNDEFINED_ID)
		event = EVENT_NONE;

	loopUI(event);
}
