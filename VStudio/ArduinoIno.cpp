// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>

#include "Arduino.h"
#include "DIO2.h"
#include "NamedPipes.h" 

/*#include "../examples/Demo/UI.cpp"
#include "../examples/Demo/WindowLocoControl.cpp"
#include "../examples/Demo/Demo.ino"*/
/*#include "../examples/Locoduino/Sample/UI.cpp"
#include "../examples/Locoduino/Sample/Sample.ino"*/
#include "../examples/Locoduino/Dcc/UI.cpp"
#include "../examples/Locoduino/Dcc/WindowLocoControl.cpp"
#include "../examples/Locoduino/Dcc/Dcc.ino"

int main()
{
#ifdef VISUALSTUDIO
	_pinName(12, 0, "Encoder1");
	_pinName(8, 0, "Encoder2");
	//_pinName(13, 0, "Led");
	_pinName(A0, 0, "Ok");
	_pinName(A1, 0, "F0");
	_pinName(A2, 0, "F1");
	_pinName(A3, 0, "Cancel");
	_pinName(A4, 0, "Stop");
#endif

	NamedPipesBegin(L"\\\\.\\pipe\\ToArduino", L"\\\\.\\pipe\\FromArduino");
	
	NamedPipesSend(MessagesTypes::Reset, _T(""));

	pinSetup();

	setup();

	while (1)
	{
		NamedPipesIdle();
		loop();
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
