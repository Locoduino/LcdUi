//-------------------------------------------------------------------
#ifndef __windowLocoControl_H__
#define __windowLocoControl_H__
//-------------------------------------------------------------------
#include "LcdUi.h"
//-------------------------------------------------------------------

#define WINDOWTYPE_CONTROLELOCO	100

class WindowLocoControl : public Window
{
public:
	int Adresse;
	byte FormatAdresse;	// nombre de caractères pour l'affichage : 3 pour '025'
	byte Vitesse;		// Entre 0 et vitesseMax
	byte VitesseMax;	// 14, 28 ou 128
	int Increment128;	// un entier juste parce que la fenêtre ne sait pas gérer autre chose
	bool Direction;
	char Nom[20];

public:
	WindowLocoControl();

	inline byte GetType() const { return WINDOWTYPE_CONTROLELOCO; }
	void Event(byte inEvenement, LcdUi *inpLcd);

	void printWindow() { Serial.println("WindowLocoControl"); }
};

#endif
//-------------------------------------------------------------------
