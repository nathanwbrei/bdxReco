// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JIntVetoMCHit_
#define _JIntVetoMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <MC/VetoMCHit.h>

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the raw and digitized banks
 */
class IntVetoMCHit: public VetoMCHit {
public:
	JOBJECT_PUBLIC(IntVetoMCHit)
	;

	//dgtz banks
	int adc1, adc2, adc3, adc4,adc5,adc6,adc7,adc8;
	int tdc1, tdc2, tdc3, tdc4,tdc5,tdc6,tdc7,tdc8;

	//raw banks
	double totEdep;
	int sector,channel,system;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(sector, "sector", "%i");
		summary.add(system, "system", "%i");
		summary.add(channel, "channel", "%i");
		summary.add(totEdep, "totEdep", "%f");
		summary.add(adc1, "adc1", "%d");
		summary.add(adc2, "adc2", "%d");
		summary.add(adc3, "adc3", "%d");
		summary.add(adc4, "adc4", "%d");
		summary.add(adc5, "adc5", "%d");
		summary.add(adc6, "adc6", "%d");
		summary.add(adc7, "adc7", "%d");
		summary.add(adc8, "adc8", "%d");
		summary.add(tdc1, "tdc1", "%d");
		summary.add(tdc2, "tdc2", "%d");
		summary.add(tdc3, "tdc3", "%d");
		summary.add(tdc4, "tdc4", "%d");
		summary.add(tdc5, "tdc5", "%d");
		summary.add(tdc6, "tdc6", "%d");
		summary.add(tdc7, "tdc7", "%d");
		summary.add(tdc8, "tdc8", "%d");
	}
};

#endif

