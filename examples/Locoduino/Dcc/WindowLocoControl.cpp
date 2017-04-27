/*************************************************************
project: <Dcc Controler>
author: <Thierry PARIS/Locoduino>
description: <Class for a loco control window>
*************************************************************/

#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl()
{
	this->Adresse = 3;
	this->FormatAdresse = 3;
	this->Vitesse = 0;
	this->VitesseMax = 128;
	this->Increment128 = 10;
	this->Direction = true;
	strcpy(this->Nom, "Name");
}

void WindowLocoControl::Event(byte inEvenement, LcdUi *inpLcd)
{
	bool afficheValeur = false;
	LcdScreen *pEcran = inpLcd->GetScreen();

	if (this->state == STATE_START)
	{
		pEcran->clear();
		LcdScreen::BuildString(this->Adresse, LcdScreen::buffer, this->FormatAdresse);
		pEcran->DisplayText(LcdScreen::buffer, 0, 0);
		byte len = LcdScreen::BuildString(this->Nom, pEcran->GetSizeX() - (this->FormatAdresse + 1), LcdScreen::buffer);
		pEcran->DisplayText(LcdScreen::buffer, pEcran->GetSizeX() - len, 0);
		this->state = STATE_NONE;
		afficheValeur = true;
	}

	byte inc = 1;

	if (this->VitesseMax == 128)
		inc = this->Increment128;
 
	switch (inEvenement)
	{
		case EVENT_MORE:
			{
			unsigned int nouvelleValeur = this->Vitesse + inc;
			if (nouvelleValeur > this->VitesseMax)
				nouvelleValeur = this->VitesseMax;
			this->Vitesse = nouvelleValeur;
			}
			afficheValeur = true;
			break;

		case EVENT_LESS:
			{
			int nouvelleValeur = this->Vitesse - inc;
			if (nouvelleValeur < 0)
				nouvelleValeur = 0;
			this->Vitesse = nouvelleValeur;
			}
			afficheValeur = true;
			break;

		case EVENT_SELECT:
			this->Direction = !this->Direction;
			afficheValeur = true;
			break;
		case EVENT_CANCEL:
			this->state = STATE_ABORTED;
			break;
	}

	if (afficheValeur)
	{
		//   01234567879012345
		// 0 Dcc 003
		// 1 +>>>>>			 -
		//   01234567879012345
		int vitesse = this->Vitesse;
		if (vitesse == 1)
			vitesse = 0;
		LcdScreen::BuildProgress(vitesse, this->Vitesse, this->Direction, pEcran->GetSizeX(), LcdScreen::buffer);
		pEcran->DisplayText(LcdScreen::buffer, 0, 1);
	}
}

