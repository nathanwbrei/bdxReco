// $Id$
//
//    File: CalorimeterDigiHit.h
// Created: Wed Jan 27 16:39:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterDigiHit_
#define _CalorimeterDigiHit_

#include <JANA/JObject.h>
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterSiPMHit.h>

#include "TObject.h"

class CalorimeterDigiHit : public TObject, public JObject {
public:

	JOBJECT_PUBLIC(CalorimeterDigiHit);

	// This method is used primarily for pretty printing
	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.sector, "sector", "%i");
		summary.add(m_channel.x, "x", "%i");
		summary.add(m_channel.y, "y", "%i");
		summary.add(m_channel.readout, "readout", "%i");
		summary.add(Q, "Q", "%f");
		summary.add(T, "T", "%f");
		summary.add(A, "A", "%f");
		summary.add(type, "type", "%i");
	}

	double Q,T,A; //Charge (u.a.), time (ns), Amplitude (mV)
	double pedMean,pedRMS;
	bool RMSflag;
	CalorimeterSiPMHit::hit_type type;
	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be 0



	ClassDef(CalorimeterDigiHit,1);
};

#endif // _CalorimeterDigiHit_

