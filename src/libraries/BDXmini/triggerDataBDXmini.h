// $Id$
//
//    File: triggerData.h
// Created: Fri May  3 11:44:17 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _triggerData_
#define _triggerData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TObject.h"

using namespace std;

class triggerDataBDXmini: public JObject, public TObject {
public:
	JOBJECT_PUBLIC(triggerDataBDXmini)
	;

	// Add data members here. For example:
	// int id;
	// double E;

	triggerDataBDXmini();

	vector<unsigned int> rawWords;

	static const int nTriggersMAX = 32;
	static const int nChansMAX = 64;

	unsigned int nTriggers;
	vector<vector<int>> trgTimes;

	unsigned int getNTriggers() const;
	void setNTriggers(unsigned int n);
	unsigned int getNtriggers_single(int itrg) const;

	void addTriggerTime_single(int itrg, int data);
	int getTriggerTime_single(int itrg, int n) const;

	bool channels[nChansMAX];
	bool channelsTRG[nChansMAX];

	bool hasChannel(int ich) const;
	bool hasChannelTRG(int ich) const;

	void setChannel(int ich);
	void setChannelTRG(int ich);
	void unsetChannel(int ich);
	void unsetChannelTRG(int ich);

	unsigned long int getLongWordChannels() const;
	unsigned long int getLongWordChannelsTRG() const;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(getLongWordChannels(), "chan", "0x%lx");
		summary.add(getLongWordChannelsTRG(), "chanTRG", "0x%lx");
	}

	ClassDef(triggerDataBDXmini,1)
	;
};

#endif // _triggerData_

