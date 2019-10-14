// $Id$
//
//    File: IntVetoHit.h
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoHit_
#define _IntVetoHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
#include <system/BDXObject.h>

class TH1D;
class TCanvas;

class IntVetoHit: public BDXObject{
private:
	mutable vector<TH1D*> hWaves; //!  //Exclude from root dictionary

public:
	JOBJECT_PUBLIC(IntVetoHit);


	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.sector, "sector", "%4d");
		summary.add(m_channel.layer, "layer", "%4d");
		summary.add(m_channel.component, "component", "%4d");
		summary.add(Q, "Q", "%4f");
		summary.add(T, "T", "%4f");
	}

	TranslationTable::INT_VETO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

	double Q,T; //Q is in phe

	virtual TCanvas* Draw(int id=0) const;//!  //Exclude from root dictionary


	ClassDef(IntVetoHit,1);

};

#endif // _IntVetoHit_

