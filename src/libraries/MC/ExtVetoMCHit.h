// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JExtVetoMCHit_
#define _JExtVetoMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <MC/VetoMCHit.h>

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the raw and digitized banks
 */
class ExtVetoMCHit: public VetoMCHit {
public:
	JOBJECT_PUBLIC(ExtVetoMCHit);

	void Summarize(JObjectSummary& summary) const final {
		summary.add(sector, "sector", "%i");
		summary.add(channel, "channel", "%i");
		summary.add(system, "system", "%i");
		summary.add(totEdep, "totEdep", "%f");
		summary.add(id, "id", "%4d");
	}

	//dgtz banks
	int adc1, tdc1;
	int adc2, tdc2;
	int adc3, tdc3;
	int adc4, tdc4;

	//raw banks
	double totEdep;
};

#endif

