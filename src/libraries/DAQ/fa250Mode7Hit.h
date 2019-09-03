// $Id$
//
//    File: fa250Mode7Hit.h
// Created: Mon Dec 28 15:05:37 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250Mode7Hit_
#define _fa250Mode7Hit_

#include <JANA/JObject.h>

#include "fa250Hit.h"

class fa250Mode7Hit : public fa250Hit {

public:
	typedef struct {
		int tdc;
		int adc;
		int ped;
		int max;
	} pulse_t;

public:
	JOBJECT_PUBLIC(fa250Mode7Hit);

	vector<pulse_t> pulses;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.rocid, "crate", "%4d");
		summary.add(m_channel.slot, "slot", "%4d");
		summary.add(m_channel.channel, "channel", "%4d");
	}
};

#endif // _fa250Mode7Hit_

