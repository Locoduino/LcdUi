/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCD UI demo>
*************************************************************/

#include "Commanders.h"
#include "LcdUi.h"
#include "UI.hpp"

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

void setup()
{
	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);

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
	unsigned long ids[] = { EVENT_DCDCC, EVENT_MORE, EVENT_LESS, EVENT_CANCEL, EVENT_SELECT };

	pushes.begin(A0, 5, ids, values, 50);
#endif

	//Commanders::printCommanders();
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
