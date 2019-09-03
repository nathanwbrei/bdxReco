// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JCalorimeterMCHit_
#define _JCalorimeterMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the calorimeter raw and digitized banks
 */
class CalorimeterMCHit: public JObject, public TObject {
public:
	JOBJECT_PUBLIC(CalorimeterMCHit);



	int x, y, sector;

	//dgtz banks
	int adcl, adcr, tdcl, tdcr;
	int adcb, adcf, tdcb, tdcf;

	//raw banks
	double totEdep;
	int pid;

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(sector, "sector(MC)", "%i");
		summary.add(x, "x(MC)", "%i");
		summary.add(y, "y(MC)", "%i");
		summary.add(totEdep, "E", "%f");
		summary.add(pid, "pid", "%i");
		summary.add(adcl, "adcl", "%d");
		summary.add(adcr, "adcr", "%d");
		summary.add(tdcl, "tdcl", "%d");
		summary.add(tdcr, "tdcr", "%d");
	}
};

#endif // _genParticle_

