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

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		// AddString(items, "id", "%4d", id);
		AddString(items, "sector", "%i", m_channel.sector);
		AddString(items, "x", "%i", m_channel.x);
		AddString(items, "y", "%i", m_channel.y);
		AddString(items, "E", "%f", E);

	}

	double E;
	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

	ClassDef(CalorimeterMCRealHit,1);
};

#endif // _CalorimeterMCRealHit_

