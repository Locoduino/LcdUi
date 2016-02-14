//-------------------------------------------------------------------
#ifndef __commander_H__
#define __commander_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifdef DEBUG_MODE
#endif

class Commander;

// Describes static data only intialized once at the beginning of execution.

class StaticCommanderData
{
public:
	// List of all commanders declared.
	// For instance DCCCommander overrides a function PriorityLoop which must be called as often as possible.
	// Other loops (asscessories, groups) have just to call Commander::StaticData::CommanderPriorityLoop() to be sure
	// to call priority functions very often.
	Commander* *pCommanderList;
	int CommanderSize;
	int CommanderAddCounter;

	StaticCommanderData();

	void CommanderPriorityLoop();
};

class Commander
{
	public:
		static StaticCommanderData StaticData;

	public:
		inline Commander() { AddCommander(this); }
		
	public:
		virtual void Setup() {}
		inline virtual bool Loop() { return false; }
		inline virtual void PriorityLoop() { }

		static void AddCommander(Commander *inCommander);

#ifdef DEBUG_MODE
	public:
		void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
