/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS/Locoduino>
description: <LCDUI Dcc demo>
*************************************************************/

#include "Commanders.h"
#include "LcdUi.h"
#include "UI.hpp"

ButtonsCommanderEncoder vitesse;
ButtonsCommanderPush boutonOk;
ButtonsCommanderPush boutonEchap;
ButtonsCommanderSwitch boutonFonction0;
ButtonsCommanderSwitch boutonFonction1;
ButtonsCommanderPush boutonStop;

#define ENCODEUR	1000
#define FONCTION0	1010
#define FONCTION1	1011

void setup()
{
	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);

	vitesse.begin(ENCODEUR, 12, 8);
	boutonOk.begin(EVENT_SELECT, A0);
	boutonEchap.begin(EVENT_CANCEL, A3);
	
	boutonFonction0.AddEvent(FONCTION0, A1, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	boutonFonction0.begin();

	boutonFonction1.AddEvent(FONCTION1, A2, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	boutonFonction1.begin();

	boutonStop.begin(EVENT_STOP, A4);
	setupUI();
}

void loop()
{
	unsigned long evenement = Commanders::loop();
	byte evenementUI = EVENT_NONE;

	switch (evenement)
	{
	case EVENT_MORE:
	case EVENT_LESS:
	case EVENT_SELECT:
	case EVENT_CANCEL:
	case EVENT_STOP:
		evenementUI = (byte)evenement;
		break;
	case ENCODEUR:
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == +1)
				evenementUI = EVENT_MORE;
			else
				evenementUI = EVENT_LESS;
		}
		break;
	case FONCTION0:
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == COMMANDERS_MOVE_ON)
				Serial.println("Function 0 activated");
			else
				Serial.println("Function 0 disactivated");
		}
		break;
	case FONCTION1:
		if (Commanders::GetLastEventType() == COMMANDERS_EVENT_MOVE)
		{
			if (Commanders::GetLastEventData() == COMMANDERS_MOVE_ON)
				Serial.println("Function 1 activated");
			else
				Serial.println("Function 1 disactivated");
		}
		break;
	}

	loopUI(evenementUI);
}
