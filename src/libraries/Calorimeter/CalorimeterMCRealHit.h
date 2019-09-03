// $Id$
//
//    File: CalorimeterMCRealHit.h
// Created: Wed Feb 22 15:46:35 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterMCRealHit_
#define _CalorimeterMCRealHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>

#include "TObject.h"

class CalorimeterMCRealHit: public JObject, public TObject {
public:
	JOBJECT_PUBLIC(CalorimeterMCRealHit);

	void Summarize(JObjectSummary& summary) const final {
		summary.add(m_channel.sector, "sector", "%i");
		summary.add(m_channel.x, "x", "%i");
		summary.add(m_channel.y, "y", "%i");
		summary.add(E, "E", "%f");
	}

	double E;
	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

	ClassDef(CalorimeterMCRealHit,1);
};

#endif // _CalorimeterMCRealHit_

