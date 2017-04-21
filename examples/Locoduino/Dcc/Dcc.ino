/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS>
description: <LCDUI Dcc demo>
*************************************************************/

#include "Commanders.h"
#include "LcdUi.h"
#include "UI.hpp"

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard push0;
ButtonsCommanderKeyboard push1;
ButtonsCommanderKeyboard push2;
ButtonsCommanderKeyboard push3;
ButtonsCommanderKeyboard pushEmergency;
#else
//ButtonsCommanderAnalogPushes pushes;

ButtonsCommanderEncoder speedLoco;
ButtonsCommanderPush buttonOk;
ButtonsCommanderPush buttonCancel;
ButtonsCommanderSwitch buttonFunction0;
ButtonsCommanderSwitch buttonFunction1;
ButtonsCommanderPush buttonEmergency;
#endif

#define ENCODER		1000
#define FUNCTION0	1010
#define FUNCTION1	1011

void setup()
{
	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);

#ifdef VISUALSTUDIO
	push0.begin(EVENT_SELECT, '*');
	push1.begin(EVENT_MORE, '+');
	push2.begin(EVENT_LESS, '-');
	push3.begin(EVENT_CANCEL, '/');
	pushEmergency.begin(EVENT_EMERGENCY, '0');
#else
	/*
	// Buttons on a 'LCD Keypad shield', handled in analog way.
	int values[] = { 0, 145, 329, 505, 741 };
	// Each button as an EVENT number as ID ! Easier for the loop !
	// One button has no meaning with 1004 (why not ?) as ID.
	unsigned long ids[] = { EVENT_DCDCC, EVENT_MORE, EVENT_LESS, EVENT_CANCEL, EVENT_SELECT };

	pushes.begin(A0, 5, ids, values, 50);
	*/
	speedLoco.begin(ENCODER, 12, 8);
	buttonOk.begin(EVENT_SELECT, A0);
	buttonCancel.begin(EVENT_CANCEL, A3);
	
	buttonFunction0.AddEvent(FUNCTION0, A1, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	buttonFunction0.begin();

	buttonFunction1.AddEvent(FUNCTION1, A2, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	buttonFunction1.begin();

	buttonEmergency.begin(EVENT_EMERGENCY, A4);
#endif

#ifdef VISUALSTUDIO
	pinName(A0, "OK");
	pinName(A1, "F0");
	pinName(A2, "F1");
	pinName(A3, "CANC");
	pinName(A4, "STOP");
#endif
	//Commanders::printCommanders();
	setupUI();
}

void loop()
{
	unsigned long event = Commanders::loop();
	byte eventUI = EVENT_NONE;

	switch (event)
	{
	case EVENT_MORE:
	case EVENT_LESS:
	case EVENT_SELECT:
	case EVENT_CANCEL:
	case EVENT_EMERGENCY:
		eventUI = (byte)event;
		break;
	case ENCODER:	// encoder
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == +1)
				eventUI = EVENT_MORE;
			else
				eventUI = EVENT_LESS;
		}
		break;
	case FUNCTION0:
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == COMMANDERS_MOVE_ON)
				Serial.println("Function 0 activated");
			else
				Serial.println("Function 0 disactivated");
		}
		break;
	case FUNCTION1:
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == COMMANDERS_MOVE_ON)
				Serial.println("Function 1 activated");
			else
				Serial.println("Function 1 disactivated");
		}
		break;
	}

	loopUI(eventUI);
}
