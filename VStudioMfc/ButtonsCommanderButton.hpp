//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#include "arduino.h"
#include "serial.hpp"

//-------------------------------------------------------------------
// A button is a hardware device allowing user to activate an accessory or a group.
// when this device is used, the Loop function returns true, and the user of the button
// can do the job.
//-------------------------------------------------------------------

class ButtonsCommanderButton
{
 private:
	 // Each button can be selected during one loop() call.
	 // All are set to false at the beginning, and set to true if necessary...
	 bool selectedLastLoop;

 public:
	 inline ButtonsCommanderButton() { this->selectedLastLoop = false; }
	 inline virtual bool IsAnalog() const { return false; }
	 inline virtual int GetPosition() const { return 0; }

	 inline virtual bool Loop() { return false; }
	 inline virtual void EndLoop() {}
	 inline bool IsSelectedLastLoop() const { return this->selectedLastLoop; }
	 inline void SelectLastLoop() { this->selectedLastLoop = true; }
	 inline void UnselectLastLoop() { this->selectedLastLoop = false; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
