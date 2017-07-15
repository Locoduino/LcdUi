/*************************************************************
project: <LCD User Interface>
author: <Thierry PARIS/Locoduino>
description: <LCDUI DcDcc demo>
*************************************************************/

#include <LiquidCrystal.h>
#include "LcdUi.h"
#include "ScreenLiquid.hpp"
#include "WindowLocoControl.hpp"
#include "UI.hpp"

// Declarations des textes
const char  str_stop[] PROGMEM = "Arret Urgence";
const char  str_stop2[] PROGMEM = "Appuyer Annuler";
const char  str_choixprincipal[] PROGMEM = "Menu principal:";
const char  str_controleloco[] PROGMEM = "Controle loco";
const char  str_choixconfig[] PROGMEM = "Configuration";
const char  str_resetconfig[] PROGMEM = "Reset Config";
const char  str_oui[] PROGMEM = "oui";
const char  str_non[] PROGMEM = "non";
const char  str_confirmer[] PROGMEM = "Sur ?";
const char  str_retroeclairage[] PROGMEM = "Retro eclairage";
const char  str_increment[] PROGMEM = "Vit Increment";
const char  str_nomloco[] PROGMEM = "Nom";
const char  str_adresseloco[] PROGMEM = "Adresse Loco";
const char  str_splash1[] PROGMEM = "LcdUI Demo";
const char  str_splash2[] PROGMEM = "For ... you !";
const char  str_supprimer[] PROGMEM = "Supprimer";

// Liste des textes
const char * const string_table[] PROGMEM =
{
	str_stop,
	str_stop2,
	str_choixprincipal,
	str_controleloco,
	str_choixconfig,
	str_resetconfig,
	str_oui,
	str_non,
	str_confirmer,
	str_retroeclairage,
	str_increment,
	str_nomloco,
	str_adresseloco,
	str_splash1,
	str_splash2,
	str_supprimer
};

// Indices dans la liste des textes
#define STR_STOP			0
#define STR_STOP2			1
#define STR_CHOIXPRINCIPAL	2
#define STR_CONTROLELOCO	3
#define STR_CHOIXCONFIG		4
#define STR_RESETCONFIG		5
#define STR_OUI				6
#define STR_NON				7
#define STR_CONFIRMER		8
#define STR_RETROECLAIRAGE	9
#define STR_INCREMENT		10
#define STR_NOM				11
#define STR_ADRESSELOCO		12
#define STR_SPLASH1			13
#define STR_SPLASH2			14
#define STR_SUPPRIMER		15

// Objets principaux
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
LcdUi lcdui;
ScreenLiquid ecran;

// Variables mises à jour par les fenêtres.
bool retroEclairage;
Choice choixPrincipal;
Choice choixConfiguration;
bool reset;

WindowSplash winSplash;
WindowChoice winChoixPrincipal;
WindowChoice winChoixConfiguration;
WindowInt winAdresse;
WindowInt winIncrement;
WindowText winNom;
WindowYesNo winRetroEclairage;
WindowConfirm winReset;
WindowLocoControl winLoco;
WindowInterrupt winStop;

void setupUI()
{
	ecran.begin(20, 4, string_table, &lcd);
	lcdui.begin(&ecran);
	
	LcdScreen::YesMsg = STR_OUI;
	LcdScreen::NoMsg = STR_NON;
	LcdScreen::BackspaceMsg = STR_SUPPRIMER;

	winSplash.begin(STR_SPLASH1, STR_SPLASH2, 2000);
	winChoixPrincipal.begin(STR_CHOIXPRINCIPAL, &choixPrincipal);
	winChoixConfiguration.begin(STR_CHOIXCONFIG, &choixConfiguration);
	winAdresse.begin(STR_ADRESSELOCO, &winLoco.Adresse);
	winIncrement.begin(STR_INCREMENT, &winLoco.Increment128);
	winNom.begin(STR_NOM, winLoco.Nom, 14);
	winRetroEclairage.begin(STR_RETROECLAIRAGE, &retroEclairage);
	winReset.begin(STR_RESETCONFIG, STR_CONFIRMER, &reset);
	winLoco.begin(STR_CONTROLELOCO);
	winStop.begin(STR_STOP, STR_STOP2, EVENT_STOP);

	lcdui.AddWindow(&winSplash);
	lcdui.AddWindow(&winChoixPrincipal);
		lcdui.AddWindow(&winChoixConfiguration);
			lcdui.AddWindow(&winAdresse);
			lcdui.AddWindow(&winIncrement);
			lcdui.AddWindow(&winNom);
			lcdui.AddWindow(&winRetroEclairage);
			lcdui.AddWindow(&winReset);
		lcdui.AddWindow(&winLoco);
	lcdui.AddWindow(&winStop);

	winChoixPrincipal.AddChoice(STR_CHOIXCONFIG, &winChoixConfiguration);
		winChoixConfiguration.AddChoice(STR_ADRESSELOCO, &winAdresse);
		winChoixConfiguration.AddChoice(STR_INCREMENT, &winIncrement);
		winChoixConfiguration.AddChoice(STR_NOM, &winNom);
		winChoixConfiguration.AddChoice(STR_RETROECLAIRAGE, &winRetroEclairage);
		winChoixConfiguration.AddChoice(STR_RESETCONFIG, &winReset);
	winChoixPrincipal.AddChoice(STR_CONTROLELOCO, &winLoco);

	// Valeurs de départ des variables globales
	retroEclairage = false;
	reset = false;
	winChoixPrincipal.SetCurrentChoiceById(STR_CONTROLELOCO);
}

void loopUI(byte inEvent)
{
	if (inEvent == EVENT_STOP)
	{
		Serial.println("Arret d'urgence !");
	}

	if (lcdui.loop(inEvent))
	{
		Window *pCurrent = lcdui.GetGlobalCurrentWindow();

		// Faire ce qui doit etre fait apres une confirmation
		if (lcdui.GetState() == STATE_CONFIRMED)
		{
			switch (pCurrent->GetWindowId())
			{
			case STR_ADRESSELOCO:
				Serial.print("Nouvelle adresse: ");
				Serial.println(winLoco.Adresse);
				break;
			case STR_INCREMENT:
				Serial.print("Nouvel incrément: ");
				Serial.println(winLoco.Increment128);
				break;
			case STR_NOM:
				Serial.print("Nouveau nom: ");
				Serial.println(winLoco.Nom);
				break;
			case STR_RETROECLAIRAGE:
				Serial.print("Retro-eclairage: ");
				Serial.println(retroEclairage);
				break;
			case STR_RESETCONFIG:
				if (reset == true)
				{
					Serial.println("Reset Config.");
					retroEclairage = false;
					winLoco.Adresse = 3;
					winLoco.FormatAdresse = 3;
					winLoco.Vitesse = 0;
					winLoco.VitesseMax = 128;
					winLoco.Increment128 = 10;
					winLoco.Direction = true;
					strcpy(winLoco.Nom, "Locoduino");
				}
				break;
			case STR_STOP:
				Serial.println("Fin de l'urgence");
				break;
			}
		}
	}
}
